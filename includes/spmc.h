#ifndef _SPMC_H_
#define _SPMC_H_

#include <iostream>
#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <optional>

// Single Producer Multiple Consumer channel
namespace spmc {

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
        ~Shared();

        // In order to access other fields of the `Shared`, the user must first
        // acquire the mutex.
        std::mutex mutex;

        // Notifies all `Receiver` threads when new values are sent by a
        // `Sender`.
        std::condition_variable cv;
        std::deque<Slot<T>> buf;
        
        // Number of receivers.
        size_t rxCount;
        bool isClosed;
        size_t offset;
    };

    template <typename T>
    class Receiver {
    public:
        // This type can be copied since there can be any number of receivers.
        // The copied receiver will only receive data newer then the last seen
        // message from the original receiver.
        Receiver(const Receiver<T>& self);
        ~Receiver();

        // Receive a value and mark as seen. Returns std::nullopt if the channel
        // is already closed i.e. the `Sender` has been destroyed.
        std::optional<T> recv();

        // Receive a value but not mark as seen. Returns std::nullopt if the
        // channel is already closed i.e. the `Sender` has been destroyed.
        std::optional<T> peek();

    private:
        Receiver(std::shared_ptr<Shared<T>> shared, size_t next) :
            shared(shared),
            next(next) {}

        std::shared_ptr<Shared<T>> shared;
        size_t next;
    };

    template <typename T>
    class Sender {
    public:
        ~Sender();

        // Sends a value throgh the channel. Returns `true` if succeeds and
        // `false` if the channel is already closed, i.e. all `Receive`rs have
        // ben destroyed.
        bool send(T val);

        // Create a receive from a sender. The newly created receiver will only
        // be notified of values that haven't been seen by all threads yet.
        Receiver<T> subscribe();

    private:
        Sender(std::shared_ptr<Shared<T>> shared) :
            shared(shared) {}

        std::shared_ptr<Shared<T>> shared;
    };


    template <typename T>
    std::pair<Sender<T>, Receiver<T>> channel();
}

#endif
