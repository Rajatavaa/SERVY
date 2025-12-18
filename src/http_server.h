#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifndef HTTP_TCP_SERVER
#define HTTP_TCP_SERVER

namespace http{
class TcpServer{
    public:
    TcpServer(std::string IP_Adress,int port);
    ~TcpServer();
    private:
    std::string m_ip_address;
    int m_port;
    SOCKET m_socket; //Server socket,Over here in shit windows we use SOCKET cause it is considered a separate API i.e winsock 
    SOCKET m_new_socket; //Client socket
    long m_incomingmessage;
    struct sockaddr_in m_socketAdress; //As it is a C header we use struct(kind),sockaddr_in(type)
    int m_socketadress_len;
    std::string m_serverMessage;
    std::string buildResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello from Servy!</h1></body></html>";
    WSADATA m_wsaData;

    int start_server();
    void close_server();
    int listening();
    int acceptConnections();

};
}
#endif
