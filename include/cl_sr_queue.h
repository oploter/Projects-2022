#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class threadsafe_queue {
    std::mutex m;
    std::queue<T> q;
    std::condition_variable q_non_empty;

public:
    void push(T value) {
        std::unique_lock l(m);
        q.emplace(std::move(value));
        q_non_empty.notify_one();
    }

    T pop() {
        std::unique_lock l(m);
        q_non_empty.wait(l, [this]() {
            return !q.empty();
        });
        T res = q.front();
        q.pop();
        return res;
    }

    bool empty(){
        std::unique_lock l(m);
        return q.empty();
    }
};

#endif