#ifndef _INPUT_H_
#define _INPUT_H_

#include <iostream>
#include <deque>
#include <memory>
#include <experimental/optional>

#include <pthread.h>

namespace Input {

    // Single producer multiple consumer channel.
    template <typename T>
    class Slot {
    private:
        Slot(T val, size_t rem) : val(val), rem(rem) {}

        // Value in the slot.
        T val;

        // Number of threads yet to read.
        size_t rem;
    };

    template <typename T>
    class Shared {
    private:
        Shared(size_t rxCount);

        pthread_mutex_t mutex;
        std::deque<Slot<T>> buf;
        size_t rxCount;
        bool isClosed;
        size_t offset;
    };

    template <typename T>
    class Sender {
    public:
        bool send(T val);

    private:
        Sender(std::shared_ptr<Shared<T>> shared) : shared(shared) {}
        ~Sender();
        std::shared_ptr<Shared<T>> shared;
    };


    template <typename T>
    class Receiver {
    public:
        std::experimental::optional<T> recv();

    private:
        Receiver(std::shared_ptr<Shared<T>> shared, size_t next) :
            shared(shared),
            next(next) {}

        std::shared_ptr<Shared<T>> shared;
        size_t next;
    };

}

#endif
