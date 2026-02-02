#pragma once
#include<string>
#include<unordered_map>

namespace Request {
    struct HTTPrequest{
        std::string method;
        std::string body;
        std::unordered_map<std::string,std::string> headers;
        std::string version;
        std::string path;  
    };

    class Parser{
    public:
        Parser(const std::string &rawstring);
        bool parse();
        HTTPrequest getrequest() const;

        //Validation
        bool ValidationVersion(const std::string &version);
        bool ValidationHeader(const std::string &header);
        bool ValidationMethod(const std::string &method);
        bool ValidationPath(const std::string &path);
        bool ValidationBody(const std::string &body);

    private:
        HTTPrequest m_request;
        std::string m_rawstring;
        
        //helpers
        void parseRequestLine(const std::string &line);
        void parseHeader(const std::string &line);
        void parseBody(const std::string &bodySection);

    };
}
