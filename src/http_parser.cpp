#include "http_request.h"
#include <iostream>
#include <sstream>

namespace {
    void Exitwitherror(const std::string &message){
        std::cout<<message<<std::endl;
    }
}

namespace Request{
    Parser::Parser(const std::string &rawstring):
        m_rawstring(rawstring),
        m_request()
    {}

    //stringstream is used mainly in parser that reads from and writes to a string in memory
    //isstringstream is used to write or parse but can't read back
    bool Parser::parse(){
        // Split the raw request into lines
        std::istringstream stream(m_rawstring);
        std::string line;
        std::string bodySection;
        
        // First line is the request line
        if (std::getline(stream, line)) {
            // Remove \r if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            parseRequestLine(line);
        }
        
        // Parse headers - single pass, no intermediate string building
        while (std::getline(stream, line)) {
            // Remove \r if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            // Empty line marks end of headers
            if (line.empty()) {
                break;
            }
            // Parse header line immediately (no double parsing)
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                // Trim leading whitespace from value
                size_t start = value.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    value = value.substr(start);
                }
                m_request.headers[key] = value;
            }
        }
        
        // Remaining is body
        std::string bodyLine;
        while (std::getline(stream, bodyLine)) {
            if (!bodyLine.empty() && bodyLine.back() == '\r') {
                bodyLine.pop_back();
            }
            bodySection += bodyLine + "\n";
        }
        if (!bodySection.empty()) {
            parseBody(bodySection);
        }
        
        // Validate all components
        return ValidationMethod(m_request.method) &&
               ValidationPath(m_request.path) &&
               ValidationVersion(m_request.version) &&
               ValidationBody(m_request.body);
    }

    HTTPrequest Parser::getrequest() const {
        return m_request;
    }

    void Parser::parseRequestLine(const std::string &line){
        std::istringstream lineStream(line);
        lineStream >> m_request.method >> m_request.path >> m_request.version;
    }

    void Parser::parseHeader(const std::string &headersSection){
        std::string line;
        std::istringstream stream(headersSection);
        while (std::getline(stream, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                // Trim leading whitespace from value
                size_t start = value.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    value = value.substr(start);
                }
                m_request.headers[key] = value;
            }
        }
    }

    void Parser::parseBody(const std::string &bodySection){
        m_request.body = bodySection;
    }

    bool Parser::ValidationMethod(const std::string &method){
        return method == "GET" || method == "POST" || method == "PUT" || 
               method == "DELETE" || method == "HEAD" || method == "OPTIONS" ||
               method == "PATCH";
    }

    bool Parser::ValidationPath(const std::string &path){
        return !path.empty() && path[0] == '/';
    }

    bool Parser::ValidationVersion(const std::string &version){
        return version == "HTTP/1.0" || version == "HTTP/1.1";
    }

    bool Parser::ValidationHeader(const std::string &header){
        size_t colonPos = header.find(':');
        return colonPos != std::string::npos && colonPos > 0;
    }

    bool Parser::ValidationBody(const std::string &body){
        return true;
    }
}
