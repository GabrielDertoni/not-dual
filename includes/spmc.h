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
    public:
        Slot(T val, size_t rem) : val(val), rem(rem) {}

        // Value in the slot.
        T val;

        // Number of threads yet to read.
        size_t rem;
    };

    template <typename T>
    class Shared {
    public:
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


    /* Implementation */

    template <class T>
    bool Sender<T>::send(T val) {
        std::lock_guard<std::mutex> lock(shared->mutex);
        if (shared->isClosed) return false;
        shared->buf.push_back(Slot<T>(val, shared->rxCount));
        shared->cv.notify_all();
        return true;
    }

    template <class T>
    Sender<T>::~Sender() {
        std::lock_guard<std::mutex> lock(shared->mutex);
        shared->isClosed = true;
    }

    template <class T>
    Receiver<T>::~Receiver() {
        std::lock_guard<std::mutex> lock(shared->mutex);
        shared->rxCount--;
        // This is the last receiver.
        if (shared->rxCount == 0) {
            shared->isClosed = true;
        }
    }

    template <class T>
    static inline std::optional<Slot<T>&> next_slot(
        std::unique_lock<std::mutex>& lock,
        std::shared_ptr<Shared<T>> shared,
        size_t next
    ) {
        if (shared->isClosed)
            return std::nullopt;

        // Must wait until some therad puts something there.
        while (shared->buf.size() == 0) {
            shared->cv.wait(lock);
        }

        Slot<T>& slot = &shared->buf[next - shared->offset];
        return std::make_optional(slot);
    }

    template <class T>
    std::optional<T> Receiver<T>::peek() {
        std::unique_lock<std::mutex> lock(shared->mutex);

        auto opt_slot = next_slot(lock, shared, next);
        if (opt_slot == std::nullopt) return std::nullopt;
        Slot<T>& slot = *opt_slot;
        return std::make_optional(slot->val);
    }

    template <class T>
    std::optional<T> Receiver<T>::recv() {
        std::unique_lock<std::mutex> lock(shared->mutex);

        auto opt_slot = next_slot(lock, shared, next);
        if (opt_slot == std::nullopt) return std::nullopt;
        Slot<T>& slot = *opt_slot;

        T val = slot->val;

        // Every thread already consumed the value, this is the last one.
        if (slot->rem == 1) {
            shared->buf.pop_front();
            shared->offset++;
        } else {
            slot->rem--;
        }

        next++;
        return val;
    }

    template <class T>
    Shared<T>::Shared(size_t rxCount) {
        this->rxCount = rxCount;
        offset = 0;
        isClosed = false;
    }

    template <class T>
    std::pair<Sender<T>, Receiver<T>> channel() {
        std::shared_ptr<Shared<T>> shared = std::make_shared(Shared<T>(1));
        Receiver<T> rx(shared, 0);
        Sender<T> tx(shared);

        return make_pair(tx, rx);
    }
}

#endif
