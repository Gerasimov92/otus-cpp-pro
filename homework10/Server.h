#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <vector>

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::ip::tcp::socket socket, std::size_t maxBulkSize);

    void start();

private:
    boost::asio::ip::tcp::socket m_socket;
    std::size_t m_maxBulkSize;
    static const std::size_t bufferSize = 1024;
    char m_buffer[bufferSize];
};

class Server
{
public:
    Server(boost::asio::io_context &ioContext, short port, std::size_t maxBulkSize);
    ~Server();

private:
    void accept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    std::size_t m_maxBulkSize;

    std::vector<std::thread> m_threads;
};
