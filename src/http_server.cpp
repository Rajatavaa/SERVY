#include <http_server.h>
#include <iostream>
#include <sstream>

namespace{
    void log(const std::string &message){ //& is used for the prohibition of any copy of the object
    std::cout<<message<<std::endl;
        }
    void Exitwitherror(const std::string &errormessage){
    std::cout<<WSAGetLastError<<std::endl;
    log ("ERROR" + errormessage);
    exit(1);
        } 
}

namespace http
{
    TcpServer::TcpServer(std::string IP_Adress,int port)
    : m_ip_adress(IP_Adress),m_port(port),m_socket(),
      m_new_socket(),m_incomingmessage(),m_socketAdress(),
      m_socketadress_len(sizeof(m_socketAdress)),
      m_serverMessage(buildResponse)

    {
    }
    TcpServer::~TcpServer()
    {
    }
} // namespace http