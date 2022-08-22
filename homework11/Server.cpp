#include "Server.h"

#include <sstream>

namespace net = boost::asio;
using boost::asio::ip::tcp;

ClientSession::ClientSession(boost::asio::ip::tcp::socket socket, SimpleDatabase &db)
    : m_socket(std::move(socket))
    , m_db(db)
{}

void ClientSession::start()
{
    read();
}

void ClientSession::read()
{
    auto self(shared_from_this());

    m_socket.async_read_some(net::buffer(m_buffer, bufferSize),
                            [this, self](boost::system::error_code error, std::size_t length)
    {
        if (error)
            return;

        std::string reply = exec(std::string(m_buffer, length));
        write(reply);
    });
}

void ClientSession::write(std::string str)
{
    auto self(shared_from_this());

    net::async_write(m_socket, net::buffer(str.data(), str.size()),
                    [this, self](boost::system::error_code error, std::size_t)
    {
        if (error)
            return;

        read();
    });
}

std::string ClientSession::exec(std::string request)
{
    std::string reply = "OK\n";

    std::istringstream ss(request);
    std::string cmd;
    ss >> cmd;

    if (cmd == "INSERT")
    {
        std::string tableName, name;
        int id;
        ss >> tableName >> id >> name;
        if (ss.fail())
            return makeError("invalid command format");

        auto res = m_db.insert(tableName, id, name);
        if (!res.empty())
            reply = makeError(res);
    }
    else if (cmd == "TRUNCATE")
    {
        std::string tableName;
        ss >> tableName;

        auto res = m_db.truncate(tableName);
        if (!res.empty())
            reply = makeError(res);
    }
    else if (cmd == "INTERSECTION")
    {
        auto res = m_db.intersection();
        reply = toString(res) + reply;
    }
    else if (cmd == "SYMMETRIC_DIFFERENCE")
    {
        auto res = m_db.symmetricDifference();
        reply = toString(res) + reply;
    }
    else
    {
        reply = makeError("unknown command " + cmd);
    }

    return reply;
}

std::string ClientSession::makeError(std::string what)
{
    return "ERR " + what + "\n";
}

std::string ClientSession::toString(const std::map<int, std::pair<std::string, std::string>> &m)
{
    std::string str;

    for (const auto &v : m)
    {
        str += std::to_string(v.first) + "," + v.second.first + "," + v.second.second + "\n";
    }

    return str;
}

Server::Server(boost::asio::io_context &ioContext, short port)
    : m_acceptor(ioContext, tcp::endpoint(tcp::v4(), port))
{
    accept();
}

Server::~Server()
{
}

void Server::accept()
{
    m_acceptor.async_accept([this](boost::system::error_code error, tcp::socket socket)
    {
        if (!error)
        {
            std::make_shared<ClientSession>(std::move(socket), m_db)->start();
        }

        accept();
    });
}
