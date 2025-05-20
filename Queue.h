#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <limits>
#include <stdexcept>

template<typename T> class Queue
{
private:
    struct Node
    {
        T data;
        std::shared_ptr<Node> next;

        explicit Node(const T &value) : data(value), next(nullptr)
        {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    mutable std::mutex _mutex;
    std::condition_variable _condVar;
    bool _isRunning = true;

public:
    explicit Queue() : head(nullptr), tail(nullptr)
    {}

    ~Queue()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        while (head) {
            head = head->next;
        }
    }

    void push(const T &value)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto newNode = std::make_shared<Node>(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        _condVar.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condVar.wait(lock, [this]() {
            return head != nullptr || !_isRunning;
        });
        if (!_isRunning && !head) {
            throw std::runtime_error("Queue is stopped");
        }
        T value = head->data;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        return value;
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _isRunning = false;
        _condVar.notify_all();
    }

    T findMax() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!head) {
            throw std::runtime_error("Queue is empty");
        }
        T max_value = std::numeric_limits<T>::min();
        auto current = head;
        while (current) {
            if (current->data > max_value) {
                max_value = current->data;
            }
            current = current->next;
        }
        return max_value;
    }

    T findMin() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!head) {
            throw std::runtime_error("Queue is empty");
        }
        T min_value = std::numeric_limits<T>::max();
        auto current = head;
        while (current) {
            if (current->data < min_value) {
                min_value = current->data;
            }
            current = current->next;
        }
        return min_value;
    }

    void print() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!head) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }
        std::cout << "Queue contents: ";
        auto current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    bool isEmpty() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return head == nullptr;
    }
};

#endif // QUEUE_H
