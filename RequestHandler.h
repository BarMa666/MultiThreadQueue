#pragma once

#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <atomic>

#include "Request.h"

// Обработчик для классов Request
class RequestHandler
{
public:
#ifdef _TEST
    RequestHandler( std::function<Request* () throw()> _get_request, 
                    std::function<void(Request*) throw()> _process_request) noexcept;
#else
    RequestHandler() noexcept;
#endif
    ~RequestHandler();

    RequestHandler(const RequestHandler& _request_worker) = delete;
    RequestHandler operator=(const RequestHandler& _request_worker) = delete;
    RequestHandler(RequestHandler&& _request_worker) = delete;
    RequestHandler operator=(RequestHandler&& _request_worker) = delete;

    void init();
    void start();
    bool alive() const;
private:
    static constexpr int NumberOfThreads = 2;
    std::unique_ptr<std::thread> threads[NumberOfThreads];

    std::mutex m_mutex;
    std::queue<std::unique_ptr<Request>> m_data;
    std::condition_variable m_condition;

    std::atomic<bool> m_alive;
    void doWork();

#ifdef _TEST
    std::function<Request* () throw()> GetRequest;
    std::function<void (Request*) throw()> ProcessRequest;
#endif
};


