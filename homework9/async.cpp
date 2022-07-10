#include "async.h"
#include "IVisitable.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "SaveToFileVisitor.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

namespace async
{

struct Handle
{
    Handle(std::size_t maxBulkSize) :
        maxBulkSize(maxBulkSize)
    {}
    std::size_t maxBulkSize;
};

static std::mutex mutex;
static std::atomic<bool> isInitialized = false;
static std::vector<std::thread> threads;
static std::list<std::unique_ptr<Handle>> handles;

static std::mutex printQueueMutex;
static std::condition_variable printQueueCV;
static std::queue<std::shared_ptr<IVisitable>> printQueue;

static std::mutex saveToFileQueueMutex;
static std::condition_variable saveToFileQueueCV;
static std::queue<std::shared_ptr<IVisitable>> saveToFileQueue;

void printWorker()
{
    PrintVisitor printVisitor(std::cout);

    while (1)
    {
        std::unique_lock lock{printQueueMutex};
        printQueueCV.wait(lock, [] { return !isInitialized; });

        while (!printQueue.empty())
        {
            printQueue.front()->accept(printVisitor);
            printQueue.pop();
        }

        if (!isInitialized)
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
        saveToFileQueueCV.wait(lock, [] { return !isInitialized; });

        while (!saveToFileQueue.empty())
        {
            saveToFileQueue.front()->accept(saveToFileVisitor);
            saveToFileQueue.pop();
        }

        if (!isInitialized)
            break;
    }
}

static void initialize()
{
    if (isInitialized)
        return;

    isInitialized = true;

    threads.emplace_back(std::thread(printWorker));
    threads.emplace_back(std::thread(saveToFileWorker));
    threads.emplace_back(std::thread(saveToFileWorker));
}

static void deinitialize()
{
    if (!isInitialized)
        return;

    isInitialized = false;
    {
        std::unique_lock l{printQueueMutex};
        printQueueCV.notify_all();
    }
    {
        std::unique_lock l{saveToFileQueueMutex};
        saveToFileQueueCV.notify_all();
    }

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }
}

handle_t connect(std::size_t bulk)
{
    std::unique_lock lock{mutex};

    if (handles.empty())
        initialize();

    handles.emplace_back(std::make_unique<Handle>(bulk));
    return handles.back().get();
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    std::unique_lock lock{mutex};

    if (std::find_if(handles.cbegin(), handles.cend(), [handle](auto &ptr) { return ptr.get() == handle; }) == handles.end())
        throw std::logic_error("Handle doesn't exist");

    auto h = reinterpret_cast<Handle*>(handle);
    std::istringstream input(std::string(data, size));
    Parser parser(input, h->maxBulkSize);

    while (1)
    {
        std::shared_ptr<IVisitable> bulk = parser.parse();
        if (!bulk)
            break;

        {
            std::unique_lock l{printQueueMutex};
            printQueue.push(bulk);
            printQueueCV.notify_one();
        }

        {
            std::unique_lock l{saveToFileQueueMutex};
            saveToFileQueue.push(bulk);
            saveToFileQueueCV.notify_one();
        }
    }
}

void disconnect(handle_t handle)
{
    std::unique_lock lock{mutex};

    handles.remove_if([handle](auto &ptr) { return ptr.get() == handle; });

    if (handles.empty())
        deinitialize();
}

} // namespace async
