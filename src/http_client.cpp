#include "http_client.h"
#include "connection.h"

#include <LUrlParser.h>
#include <iostream>
#include <sstream>
#include <memory>

HttpClient::RepliedMessage HttpClient::sendGetCommand(const std::string& url, const std::string& header, bool verbose)
{
    using LUrlParser::clParseURL;
    clParseURL lurl = clParseURL::ParseURL(url);
    if (!lurl.IsValid())
    {
        std::cerr << "Cannot parse url " << url << std::endl;
        exit(1);
    }
    std::ostringstream oss;
    oss << "GET /";
    constructMessage(oss, lurl, header, "");
    auto message = oss.str();
    if (verbose)
        std::cout << "GET message:" << std::endl << message << "------------" << std::endl;
    auto connection = TcpConnectionFactory::createInstance(lurl.m_Host, 80);
    connection->connect();
    connection->write(message);
    auto reply = extractMessage(connection->read());
    if (verbose)
        std::cout << "Reply Header:" << std::endl << reply.header << "------------" << std::endl;
    return reply;
}

HttpClient::RepliedMessage HttpClient::sendPostCommand(const std::string& url, const std::string& data, const std::string& header, bool verbose)
{
    using LUrlParser::clParseURL;
    clParseURL lurl = clParseURL::ParseURL(url);
    if (!lurl.IsValid())
    {
        std::cerr << "Cannot parse url " << url << std::endl;
        exit(1);
    }
    std::ostringstream oss;
    oss << "POST /";
    constructMessage(oss, lurl, header, data);
    auto message = oss.str();
    if (verbose)
        std::cout << "POST message:" << std::endl << message << "------------" << std::endl;
    auto connection = TcpConnectionFactory::createInstance(lurl.m_Host, 80);
    connection->connect();
    connection->write(message);
    auto reply = extractMessage(connection->read());
    if (verbose)
        std::cout << "Reply Header:" << std::endl << reply.header << "------------" << std::endl;
    return reply;
}

HttpClient::RepliedMessage HttpClient::extractMessage(const std::string& message)
{
    using namespace std;
    stringstream ss(message);
    string line;
    RepliedMessage reply;
    bool read_status_code = false;
    //Handling different characters which represent newline in different operating systems can be tricky!
    while (getline(ss, line) && line != "\r" && !line.empty())
    {
        if (line.back() == '\r')
            line.pop_back();
        if (!read_status_code && line.substr(0, 4) == "HTTP")
        {
            read_status_code = true;
            std::stringstream ss(line.substr(5));
            ss >> reply.http_version >> reply.status_code;
        }
        else
        {
            auto index = line.find(':');
            if (index != line.npos)
            {
                std::string key, value;
                key = line.substr(0, index);
                if ((index + 1) < line.length() && line[index + 1] == ' ')
                    ++index;
                value = line.substr(index + 1);
                reply.http_header[key] = value;
            }
        }
        line.push_back('\n');
        reply.header += line;
    }
    char ch;
    while (ss.get(ch))
        reply.body += ch;

    return reply;
}

void HttpClient::constructMessage(std::ostringstream& oss, LUrlParser::clParseURL& lurl, const std::string& header, const std::string& data)
{
    oss << lurl.m_Path;
    if (!lurl.m_Query.empty())
        oss << '?' << lurl.m_Query;
    if (!lurl.m_Fragment.empty())
        oss << '#' << lurl.m_Fragment;
    oss << " HTTP/1.0\r\nHost: " << lurl.m_Host;
    if (!header.empty())
        oss << "\r\n" << header;
    if (!data.empty())
        oss << "\r\nContent-Length: " << data.length() << "\r\n\r\n" << data;
    oss << "\r\n\r\n";
}
