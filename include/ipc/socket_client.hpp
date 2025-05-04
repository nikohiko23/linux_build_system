#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include <string>

namespace ipc {

class SocketClient {
public:
    SocketClient(const char* socket_path);
    bool sendMessage(const std::string& message);
    void send(const std::string& message);
private:
    const char* socketPath;
};

} // namespace ipc

#endif // SOCKET_CLIENT_HPP
