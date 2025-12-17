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
    std::string m_ip_adress;
    int m_port;
    SOCKET m_socket; //Over here in shit windows we use SOCKET cause it is considered a separate API i.e winsock 
    SOCKET m_new_socket;
    long m_incomingmessage;
    struct sockaddr_in m_socketAdress;
    int m_socketadress_len;
    std::string m_serverMessage;
    WSADATA m_wsaData;

    int start_server();
    void close_server();
};
}
#endif
