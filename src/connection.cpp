#include <asio.hpp>
#include <sstream>
#include <string>

#include "connection.h"

TcpConnection::TcpConnection(const std::string& server_address, const int port) :
    _port{port}, _server_address{server_address}
{
}

TcpConnection::~TcpConnection()
{
}

AsioTcpConnection::AsioTcpConnection(asio::io_service& io_service, const std::string& server_address, const int port) :
    TcpConnection(server_address, port), _socket{asio::ip::tcp::socket(io_service)}, _io_service{io_service}
{
}

void AsioTcpConnection::connect()
{
    std::ostringstream iss;
    asio::ip::tcp::resolver resolver(_io_service);
    iss << _port;
    asio::connect(_socket, resolver.resolve({_server_address, iss.str()}));
}

void AsioTcpConnection::write(const std::string& message)
{
    asio::write(_socket, asio::buffer(message));
}

std::string AsioTcpConnection::read()
{
    std::string reply;
    reply.resize(1000);
    asio::read(_socket, asio::buffer(reply));
    return reply;
}

AsioTcpConnection::~AsioTcpConnection()
{
}

