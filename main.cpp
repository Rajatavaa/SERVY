#include "http_server.h"

int main()
{
    using namespace http;
    TcpServer server("0.0.0.0", 8080);
    server.run();
    return 0;
}