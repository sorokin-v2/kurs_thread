#pragma once
#include <vector>
#include <mutex>
#include <thread>
#include "safequeue.h"

template <typename T>
class Thread_pool {

public:

    Thread_pool(const Thread_pool&) = delete;                 //Запрещаем конструктор копирования
    Thread_pool& operator= (const Thread_pool&) = delete;     //Запрещаем оператор присваивания

    Thread_pool() : _done{ false } {
        const int thread_count = std::thread::hardware_concurrency();
        try {
            for (int i = 0; i < thread_count; ++i) {
                _threads.push_back(std::thread(&Thread_pool::work, this));
            }
        }
        catch (...)
        {
            _done = true;
            throw;
        }
    }

    ~Thread_pool()
    {
        _done = true;
        for (int i = 0; i < _threads.size(); ++i)
        {
            if (_threads[i].joinable()) { _threads[i].join(); }
        }
    }

    void submit(T f)
    {
        _queue.push(T(f));
    }

private:

    void work() {
        while (!_done)
        {
            if (!_queue.empty())
            {
                T task = _queue.pop();
                try
                {
                    task();
                }
                catch (std::exception& ex)
                {
                    _done = true;
                    std::cout << std::this_thread::get_id() << "  " << ex.what() << std::endl;
                }
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    std::vector <std::thread> _threads;
    Safe_queue <T> _queue;
    std::atomic_bool _done{ false };
};

