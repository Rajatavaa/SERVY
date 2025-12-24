#include "http_server.h"
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
    TcpServer::TcpServer(std::string ip_address, int port)
        : m_ip_address(ip_address),
          m_port(port),
          m_socket(INVALID_SOCKET),
          m_new_socket(INVALID_SOCKET),
          m_incomingmessage(0),
          m_socketAdress(),
          m_socketadress_len(sizeof(m_socketAdress)),
          m_serverMessage(),
          m_wsaData()
    {
        start_server();
    }
    TcpServer::~TcpServer()

    {
        close_server();
    }
    int TcpServer::start_server()
    {
        if (WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0) //Loads the version of winsock version in 16 bits and we pass in the address for windows to fillup WSAdata giving access to the location
        {
            Exitwitherror("WSAstartup failed");
        }
        m_socket = socket(AF_INET,SOCK_STREAM,0); //Creating the socket
        if (m_socket == INVALID_SOCKET){
            Exitwitherror("Cannot create socket");
            return 1;}
        m_socketAdress.sin_family = AF_INET;
        m_socketAdress.sin_port = htons(m_port);
        m_socketAdress.sin_addr.s_addr = INADDR_ANY; //Listen on any iterface
        if (bind(m_socket,(sockaddr *)&m_socketAdress,m_socketadress_len) == SOCKET_ERROR)
        {
            Exitwitherror("Socket cannot be connected");
            return 1;
        }
        return 0;
    }
    
    int TcpServer::listening()
    {
        if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR){
            Exitwitherror("Cannot listen to the socket");
            return 1;
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << m_ip_address
           << " PORT: " << ntohs(m_socketAdress.sin_port) << " ***\n\n";
        log(ss.str());
        return 0;
    }

    int TcpServer::acceptConnections(SOCKET &new_socket){
        new_socket = accept(m_socket, (sockaddr *)&m_socketAdress,
                        &m_socketadress_len);
        if (new_socket == INVALID_SOCKET) {
            Exitwitherror("Accept failed!");
            return 1;
        }
        return 0;
    }
    
    


    void TcpServer::close_server(){
        closesocket(m_socket);
        closesocket(m_new_socket);
        WSACleanup();
    }

    void TcpServer::run() {
        listening();
        log("====== Waiting for a new connection ======\n");

        while (true) {
            acceptConnections(m_new_socket);
            handleConnection(m_new_socket);
            closesocket(m_new_socket);
        }
    }

    std::string TcpServer::buildResponse() {
        std::string htmlContent =
            "<html>"
            "<head><title>Servy</title></head>"
            "<body><h1>Hello from Servy!</h1></body>"
            "</html>";

        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << htmlContent.size() << "\r\n"
           << "Connection: close\r\n"
           << "\r\n"
           << htmlContent;

        return ss.str();
    }

    void TcpServer::handleConnection(SOCKET client_socket) {
        const int BUFFER_SIZE = 30720;
        char buffer[BUFFER_SIZE] = {0};

        int bytesReceived = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0) {
            log("Failed to receive data from client");
            return;
        }

        log("------ Received Request ------");
        log(buffer);

        std::string response = buildResponse();
        int bytesSent = send(client_socket, response.c_str(),
                             static_cast<int>(response.size()), 0);

        if (bytesSent == SOCKET_ERROR) {
            log("Error sending response to client");
        } else {
            log("------ Response sent to client ------\n");
        }
    }

} // namespace http