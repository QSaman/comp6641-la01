#pragma once

#include <string>
#include <sstream>

namespace LUrlParser{ class clParseURL; }

class HttpClient
{
public:
    struct RepliedMessage
    {
        std::string header, body;
    };
public:
    std::string sendGetCommand(const std::string& url, const std::string& header = "", bool verbose = false);
    std::string sendPostCommand(const std::string& url, const std::string& data = "",
                                const std::string& header = "", bool verbose = false);
private:
    RepliedMessage extractMessage(const std::string& message);
    void constructMessage(std::ostringstream& oss, LUrlParser::clParseURL& lurl, const std::string& header, const std::string& data);
};
