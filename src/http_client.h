#pragma once

#include <string>
#include <sstream>
#include <map>

namespace LUrlParser{ class clParseURL; }
using HttpHeader = std::map<std::string, std::string>;

class HttpClient
{
public:
    struct RepliedMessage
    {
        std::string header, body, http_version;
        int status_code;
        HttpHeader http_header;
    };
public:
    RepliedMessage sendGetCommand(const std::string& url, const std::string& header = "", bool verbose = false);
    RepliedMessage sendPostCommand(const std::string& url, const std::string& data = "",
                                const std::string& header = "", bool verbose = false);
private:
    RepliedMessage extractMessage(const std::string& message);
    void constructMessage(std::ostringstream& oss, LUrlParser::clParseURL& lurl, const std::string& header, const std::string& data);
};
