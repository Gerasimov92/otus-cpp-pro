#include "IVisitable.h"
#include "Command.h"
#include "CommandBulk.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "SaveToFileVisitor.h"
#include "Server.h"

#include <condition_variable>
#include <iostream>
#include <list>
#include <queue>

namespace net = boost::asio;
using boost::asio::ip::tcp;

static std::atomic<bool> isStarted = false;

static std::mutex combineQueueMutex;
static std::condition_variable combineQueueCV;
static std::queue<std::shared_ptr<IVisitable>> combineQueue;

static std::mutex printQueueMutex;
static std::condition_variable printQueueCV;
static std::queue<std::shared_ptr<IVisitable>> printQueue;

static std::mutex saveToFileQueueMutex;
static std::condition_variable saveToFileQueueCV;
static std::queue<std::shared_ptr<IVisitable>> saveToFileQueue;


static void queueForProcessing(const std::shared_ptr<IVisitable> &cmd)
{
    {
        std::lock_guard lock{printQueueMutex};
        printQueue.push(cmd);
        printQueueCV.notify_one();
    }

    {
        std::lock_guard lock{saveToFileQueueMutex};
        saveToFileQueue.push(cmd);
        saveToFileQueueCV.notify_one();
    }
}

static void combineWorker(std::size_t maxBulkSize)
{
    std::list<Command> commands;

    while (1)
    {
        std::unique_lock lock{combineQueueMutex};
        combineQueueCV.wait(lock, [] { return !isStarted || !combineQueue.empty(); });

        while (!combineQueue.empty())
        {
            auto *cmd = dynamic_cast<Command*>(combineQueue.front().get());
            if (cmd)
            {
                commands.push_back(*cmd);

                if (commands.size() == maxBulkSize)
                {
                    queueForProcessing(std::make_shared<CommandBulk>(commands));
                    commands.clear();
                }
            }
            else
            {
                if (!commands.empty())
                {
                    queueForProcessing(std::make_shared<CommandBulk>(commands));
                    commands.clear();
                }

                queueForProcessing(combineQueue.front());
            }

            combineQueue.pop();
        }

        if (!commands.empty())
        {
            queueForProcessing(std::make_shared<CommandBulk>(commands));
            commands.clear();
        }

        if (!isStarted)
            break;
    }
}

void printWorker()
{
    PrintVisitor printVisitor(std::cout);

    while (1)
    {
        std::unique_lock lock{printQueueMutex};
        printQueueCV.wait(lock, [] { return !isStarted || !printQueue.empty(); });

        while (!printQueue.empty())
        {
            printQueue.front()->accept(printVisitor);
            printQueue.pop();
        }

        if (!isStarted)
            break;
    }
}

void saveToFileWorker()
{
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    SaveToFileVisitor saveToFileVisitor(ss.str());

    while (1)
    {
        std::unique_lock lock{saveToFileQueueMutex};
        saveToFileQueueCV.wait(lock, [] { return !isStarted || !saveToFileQueue.empty(); });

        while (!saveToFileQueue.empty())
        {
            saveToFileQueue.front()->accept(saveToFileVisitor);
            saveToFileQueue.pop();
        }

        if (!isStarted)
            break;
    }
}

ClientSession::ClientSession(boost::asio::ip::tcp::socket socket, std::size_t maxBulkSize)
    : m_socket(std::move(socket))
    , m_maxBulkSize(maxBulkSize)
{}

void ClientSession::start()
{
    auto self(shared_from_this());

    m_socket.async_read_some(net::buffer(m_buffer, bufferSize),
                             [this, self](boost::system::error_code error, std::size_t length)
    {
        if (error)
            return;

        std::istringstream input(std::string(m_buffer, length));
        Parser parser(input, m_maxBulkSize);

        while (1)
        {
            std::shared_ptr<IVisitable> cmd = parser.parse(false);
            if (!cmd)
                break;

            std::lock_guard lock{combineQueueMutex};
            combineQueue.push(cmd);
        }

        combineQueueCV.notify_one();
    });
}

Server::Server(boost::asio::io_context &ioContext, short port, std::size_t maxBulkSize)
    : m_acceptor(ioContext, tcp::endpoint(tcp::v4(), port))
    , m_maxBulkSize(maxBulkSize)
{
    isStarted = true;

    m_threads.emplace_back(std::thread(combineWorker, maxBulkSize));
    m_threads.emplace_back(std::thread(printWorker));
    m_threads.emplace_back(std::thread(saveToFileWorker));
    m_threads.emplace_back(std::thread(saveToFileWorker));

    accept();
}

Server::~Server()
{
    if (!isStarted)
        return;

    isStarted = false;
    combineQueueCV.notify_all();
    printQueueCV.notify_all();
    saveToFileQueueCV.notify_all();

    for (auto &t : m_threads)
    {
        if (t.joinable())
            t.join();
    }
}

void Server::accept()
{
    m_acceptor.async_accept([this](boost::system::error_code error, tcp::socket socket)
    {
        if (!error)
        {
            std::make_shared<ClientSession>(std::move(socket), m_maxBulkSize)->start();
        }

        accept();
    });
}
