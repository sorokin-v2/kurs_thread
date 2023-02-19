#pragma once
#include <list>
#include <mutex>
#include <condition_variable>
#include <functional>

template <typename T>
class Safe_queue {

public:

    Safe_queue() = default;
    Safe_queue(const Safe_queue&) = delete;                 //Запрещаем конструктор копирования
    Safe_queue& operator= (const Safe_queue&) = delete;     //Запрещаем оператор присваивания

    void push(const T& new_val) {
        std::lock_guard<std::mutex> lg (_mut);
        _list.push_back(std::move(new_val));
        _cond_var.notify_one();
    };

    T pop()
    {
        std::unique_lock<std::mutex> ul(_mut);
        _cond_var.wait(ul, [this] {return !_list.empty(); });
        T res = std::move(_list.front());
        _list.pop_front();
        return res;
    }

     bool empty() const {
        std::lock_guard<std::mutex> lk(_mut);
        return _list.empty();
    }

private:

    mutable std::mutex _mut;
    std::condition_variable _cond_var;
    std::list<T> _list;
};

