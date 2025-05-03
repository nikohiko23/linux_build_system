#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

namespace ipc {

class SocketServer {
public:
    SocketServer(const char* socket_path);
    ~SocketServer();
    void run();

private:
    const char* socketPath;
    int serverFd;
    void handleClient(int clientFd);
};

} // namespace ipc

#endif // SOCKET_SERVER_HPP
