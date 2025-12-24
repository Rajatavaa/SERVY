#ifndef HTTP_TCP_SERVER_H
#define HTTP_TCP_SERVER_H

#include <winsock2.h>
#include <string>

namespace http {

class TcpServer {
public:
    TcpServer(std::string ip_address, int port);
    ~TcpServer();
    void run();

private:
    std::string m_ip_address;
    int m_port;
    SOCKET m_socket;
    SOCKET m_new_socket;
    long m_incomingmessage;
    struct sockaddr_in m_socketAdress;
    int m_socketadress_len;
    std::string m_serverMessage;
    WSADATA m_wsaData;

    int start_server();
    void close_server();
    int listening();
    int acceptConnections(SOCKET &new_socket);
    std::string buildResponse();
    void handleConnection(SOCKET client_socket);
};

} // namespace http

#endif
