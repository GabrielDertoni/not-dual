#include "includes/spmc.h"

using namespace spmc;

template <typename T>
bool Sender<T>::send(T val) {
    std::lock_guard<std::mutex> lock(shared->mutex);
    if (shared->isClosed) return false;
    shared->buf.push_back(Slot<T>(val, shared->rxCount));
    shared->cv.notify_all();
}

template <typename T>
Sender<T>::~Sender() {
    std::lock_guard<std::mutex> lock(shared->mutex);
    shared->isClosed = true;
}

template <typename T>
Receiver<T>::~Receiver() {
    std::lock_guard<std::mutex> lock(shared->mutex);
    shared->rxCount--;
    // This is the last receiver.
    if (shared->rxCount == 0) {
        shared->isClosed = true;
    }
}

template <typename T>
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

template <typename T>
std::optional<T> Receiver<T>::peek() {
    std::unique_lock<std::mutex> lock(shared->mutex);

    auto opt_slot = next_slot(lock, shared, next);
    if (opt_slot == std::nullopt) return std::nullopt;
    Slot<T>& slot = *opt_slot;
    return std::make_optional(slot->val);
}

template <typename T>
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

template <typename T>
Shared<T>::Shared(size_t rxCount) {
    this->rxCount = rxCount;
    offset = 0;
    isClosed = false;
}

template <typename T>
std::pair<Sender<T>, Receiver<T>> channel() {
    std::shared_ptr<Shared<T>> shared = std::make_shared(Shared<T>(1));
    Receiver<T> rx(shared, 0);
    Sender<T> tx(shared);

    return make_pair(tx, rx);
}
