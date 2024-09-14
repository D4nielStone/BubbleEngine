#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include "becommons.hpp"

class BECOMMONS_DLL_API ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<void>;

    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
};

#include "ThreadPool.inl" // Inclua o arquivo de implementação inline aqui

#endif // THREADPOOL_HPP
