#include "ipc/socket_server.hpp"

int main() {
    ipc::SocketServer server("/tmp/build_socket");
    server.run();
    return 0;
}
