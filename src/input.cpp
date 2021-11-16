#include <pair>

#include "../includes/input.h"


bool Sender::send(T val) {
    pthread_mutex_lock(&shared->mutex);
    if (shared->isClosed) return false;
    shared->buf.push_back(Slot(val, shared->rxCount));
    pthread_mutex_unlock(&shared->mutex);
}

Sender::~Sender() {
    pthread_mutex_lock(&shared->mutex);
    shared->isClosed = true;
    pthread_mutex_unlock(&shared->mutex);
}

std::optional<T> Receiver::recv() {
    pthread_mutex_lock(&shared->mutex);
    if (shared->isClosed) return std::nullopt;
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

Shared::Shared(size_t rxCount) {
    pthread_mutex_init(&mutex, NULL);
    this->rxCount = rxCount;
    offset = 0;
    isClosed = false;
}

template <class T>
std::pair<Sender<T>, Receiver<T>> channel() {
    std::shared_ptr<Shared<T>> shared = std::make_shared(Shared(1));
    Receiver rx(shared, 0);
    Sender tx(shared);

    return make_pair(tx, rx);
}
