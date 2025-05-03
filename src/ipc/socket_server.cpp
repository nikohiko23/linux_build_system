#include "ipc/socket_server.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace ipc {

SocketServer::SocketServer(const char* socket_path)
    : socketPath(socket_path), serverFd(-1) {}

SocketServer::~SocketServer() {
    if (serverFd != -1) close(serverFd);
    unlink(socketPath);
}

void SocketServer::run() {
    serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverFd == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);
    unlink(socketPath);  // Удалим, если уже существует

    if (bind(serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        std::cerr << "Bind failed\n";
        return;
    }

    if (listen(serverFd, 5) == -1) {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Socket server listening on " << socketPath << "...\n";

    while (true) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd == -1) {
            std::cerr << "Accept failed\n";
            continue;
        }
        handleClient(clientFd);
        close(clientFd);
    }
}

void SocketServer::handleClient(int clientFd) {
    char buffer[256];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Received: " << buffer << std::endl;
    }
}

} // namespace ipc
