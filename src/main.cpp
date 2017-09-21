#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include <cxxopts.hpp>
#include <asio.hpp>

#include "connection.h"

enum class CommandType {None, Get, Post, Help};

static bool verbose = false;
static std::string header;
static CommandType command_type = CommandType::None;
static std::string url;
static std::string post_data;

[[noreturn]] void print_all_helps(cxxopts::Options& options)
{
    std::cout << options.help({"", "get", "post"});
    exit(0);
}

void handle_post_options(cxxopts::Options& options)
{
    bool has_data = false;
    if (options.count("data"))
    {
        if (command_type != CommandType::Post)
        {
            std::cerr << "You cannot use --data for a command other than POST" << std::endl;
            exit(1);
        }
        post_data = options["data"].as<std::string>();
        has_data = true;
    }
    if (options.count("file"))
    {
        if (has_data)
        {
            std::cerr << "You cannot use both --data and --file" << std::endl;
            exit(1);
        }
        if (command_type != CommandType::Post)
        {
            std::cerr << "You cannot use --file for a command other than POST" << std::endl;
            exit(1);
        }
        auto file_path = options["file"].as<std::string>();
        std::ifstream fin;
        std::ostringstream oss;
        fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try
        {
            fin.open(file_path);
            oss << fin.rdbuf();
        }
        catch(std::ifstream::failure e)
        {
            std::cerr << "Error in reading file " << file_path << std::endl;
            exit(1);
        }
        post_data = oss.str();
    }
}

void process_input_args(int argc, char* argv[])
{
    using namespace std;
    cxxopts::Options options("httpc", "httpc is a curl-like application but supports HTTP only.");
    options.positional_help("\n  httpc [Option...] [get|post] [URL]\n  httpc [help] [get|post]");
    options.add_options()
            ("help", "Show all help menus")
            ("v,verbose", "Prints the detail of the response such as protocol, status, and headers")
            ("h,header", "Associate headers to HTTP Request with the format 'key:value'", cxxopts::value<string>(), "key:value");
    //The following options are invisible to the user we only use them for positional arguments
    options.add_options("positional")
            ("c, command", "get is a HTTP GET and post is HTTP POST and help is this text", cxxopts::value<string>(), "get|post|help")
            ("u,url", "A valid URL address", cxxopts::value<string>());
    options.add_options("get")
            ("g,get", "executes a HTTP GET request for a given url", cxxopts::value<string>(), "URL");
    options.add_options("post")
            ("p,post", "executes a HTTP POST request for a given URL with inline data or from file", cxxopts::value<string>(), "URL")
            ("d,data", "Associate an inline data to the body HTTP POST request", cxxopts::value<string>(), "string")
            ("f,file", "Associate the content of a file to the body HTTP POST request", cxxopts::value<string>(), "file");

    options.parse_positional(vector<string>{"command", "url"});
    options.parse(argc, argv);

    if (options.count("help"))
        print_all_helps(options);
    if (options.count("verbose"))
        verbose = true;
    if (options.count("header"))
        header = options["header"].as<string>();
    if (options.count("command"))
    {
        auto cmd_str = options["command"].as<string>();
        if (cmd_str == "get")
            command_type = CommandType::Get;
        else if (cmd_str == "post")
            command_type = CommandType::Post;
        else if (cmd_str == "help")
            command_type = CommandType::Help;
        else
        {
            cerr << "Invalid command " << cmd_str << endl;
            exit(1);
        }
        if (options.count("url"))
            url = options["url"].as<string>();
        else
        {
            cerr << "You forgot to provide the second argument" << endl;
            exit(1);
        }
    }
    if (options.count("get"))
    {
        if (command_type != CommandType::None)
        {
            cerr << "You need to provide at most one command" << endl;
            exit(1);
        }
        command_type = CommandType::Get;
        url = options["get"].as<string>();
    }
    if (options.count("post"))
    {
        if (command_type != CommandType::None)
        {
            cerr << "You need to provide at most one command" << endl;
            exit(1);
        }
        command_type = CommandType::Post;
        url = options["post"].as<string>();
    }
    handle_post_options(options);
}

int main(int argc, char* argv[])
{
    int a = 7;
    //process_input_args(argc, argv);
    asio::io_service io_service;
    TcpConnection* con = new AsioTcpConnection(io_service, "127.0.0.1", 7777);
    con->connect();
    con->write("Hello There!");
    auto reply = con->read();
    std::cout << "reply: " << reply << ". length: " << reply.length() << std::endl;
}
