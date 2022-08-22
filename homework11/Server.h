#pragma once

#include "SimpleDatabase.h"

#include <boost/asio.hpp>
#include <memory>
#include <string>

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::ip::tcp::socket socket, SimpleDatabase &db);

    void start();

private:
    void read();
    void write(std::string str);
    std::string exec(std::string request);
    std::string makeError(std::string what);
    std::string toString(const std::map<int, std::pair<std::string, std::string>> &m);

    boost::asio::ip::tcp::socket m_socket;
    static const std::size_t bufferSize = 1024;
    char m_buffer[bufferSize];
    SimpleDatabase &m_db;
};

class Server
{
public:
    Server(boost::asio::io_context &ioContext, short port);
    ~Server();

private:
    void accept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    SimpleDatabase m_db;
};
