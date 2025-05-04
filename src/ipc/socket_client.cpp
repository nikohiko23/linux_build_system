#include "../../include/ipc/socket_client.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace ipc {

SocketClient::SocketClient(const char* socket_path)
    : socketPath(socket_path) {}

bool SocketClient::sendMessage(const std::string& message) {
    int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockFd == -1) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);

    if (connect(sockFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        std::cerr << "Connect failed\n";
        close(sockFd);
        return false;
    }

    write(sockFd, message.c_str(), message.size());
    close(sockFd);
    return true;
}

} // namespace ipc
