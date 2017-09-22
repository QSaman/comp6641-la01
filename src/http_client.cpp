#include "http_client.h"
#include "connection.h"

#include <LUrlParser.h>
#include <iostream>
#include <sstream>
#include <memory>

std::string HttpClient::sendGetCommand(const std::string& url, const std::string& header, bool verbose)
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
    return reply.body;
}

std::string HttpClient::sendPostCommand(const std::string& url, const std::string& data, const std::string& header, bool verbose)
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
    return reply.body;
}

HttpClient::RepliedMessage HttpClient::extractMessage(const std::string& message)
{
    using namespace std;
    stringstream ss(message);
    string line;
    RepliedMessage reply;
    //Handling different characters which represent newline in different operating systems can be tricky!
    while (getline(ss, line) && line != "\r" && !line.empty())
    {
        if (line.back() == '\r')
            line.pop_back();
        line.push_back('\n');
        reply.header += line;
    }
    while (getline(ss, line))
    {
        if (line.back() == '\r')
            line.pop_back();
        line.push_back('\n');
        reply.body += line;
    }

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
