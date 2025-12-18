#include <http_server.h>
#include <iostream>
#include <sstream>

namespace{
    
    void log(const std::string &message){ //& is used for the prohibition of any copy of the object
    std::cout<<message<<std::endl;
        }
    
    void Exitwitherror(const std::string &errormessage){
    std::cout<<WSAGetLastError()<<std::endl;
    log ("ERROR: " + errormessage);
    exit(1);
        } 
}

namespace http
{
    TcpServer::TcpServer(std::string IP_Adress,int port)
    :   m_ip_adress(IP_Adress),
        m_port(port),
        m_socket(),
        m_new_socket(),
        m_incomingmessage(),
        m_socketAdress(),
        m_socketadress_len(sizeof(m_socketAdress)),
        m_serverMessage(buildResponse)

    {
        start_server();
    }
    TcpServer::~TcpServer()

    {
        close_server();
    }
    int TcpServer::start_server()
    {
        if (WSAStartup(MAKEWORD(2,0), &m_wsaData) != 0) //Loads the version of winsock version in 16 bits and we pass in the address for windows to fillup WSAdata giving access to the location
        {
            Exitwitherror("WSAstartup failed");
        }
        m_socket = socket(AF_INET,SOCK_STREAM,0); //Creating the socket
        if (m_socket == INVALID_SOCKET){
            Exitwitherror("Cannot create socket");
            return 1;}
        m_socketAdress.sin_family = AF_INET;
        m_socketAdress.sin_port = htons(m_port);
        m_socketAdress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        if (bind(m_socket,(sockaddr *)&m_socketAdress,m_socketadress_len) == SOCKET_ERROR)
        {
            Exitwitherror("Socket cannot be connected");
            return 1;
        }
        return 0;
    }

    void TcpServer::close_server(){
        closesocket(m_socket);
        closesocket(m_new_socket);
        WSACleanup();
        exit(0);
    }

} // namespace http