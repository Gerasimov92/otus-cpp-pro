#include "Server.h"
#include <iostream>
#include <vector>

boost::asio::io_context ioContext;

void signalHandler(const boost::system::error_code &error, int signalNumber)
{
    if (!error)
    {
        ioContext.stop();
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
        return 1;
    }

    short port = std::atoi(argv[1]);
    std::size_t maxBulkSize = std::atoi(argv[2]);

    std::vector<std::thread> threads;

    try
    {
        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM, SIGTSTP);
        signals.async_wait(signalHandler);

        Server server(ioContext, port, maxBulkSize);

        for (int i = 0; i < 2; i++)
            threads.emplace_back(std::thread([](){ ioContext.run(); }));
        ioContext.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return 0;
}
