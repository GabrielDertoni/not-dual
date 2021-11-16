#include <utility>

#include "../includes/input.h"

using namespace Input;

template <typename T>
bool Sender<T>::send(T val) {
    pthread_mutex_lock(&shared->mutex);
    if (shared->isClosed) return false;
    shared->buf.push_back(Slot<T>(val, shared->rxCount));
    pthread_mutex_unlock(&shared->mutex);
}


template <typename T>
Sender<T>::~Sender() {
    pthread_mutex_lock(&shared->mutex);
    shared->isClosed = true;
    pthread_mutex_unlock(&shared->mutex);
}

template <typename T>
std::experimental::optional<T> Receiver<T>::recv() {
    pthread_mutex_lock(&shared->mutex);
    if (shared->isClosed) return std::experimental::nullopt;
    Slot<T> *slot = &shared->buf[next - shared->offset];

    T val = slot->val;

    // Every thread already consumed the value, this is the last one.
    if (slot->rem == 1) {
        shared->buf.pop_front();
        shared->offset++;
    } else {
        slot->rem--;
    }

    shared->buf.pop_front();
    pthread_mutex_unlock(&shared->mutex);

    next++;
    return val;
}

template <typename T>
Shared<T>::Shared(size_t rxCount) {
    pthread_mutex_init(&mutex, NULL);
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
