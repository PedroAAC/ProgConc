#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t read_cond, write_cond;
    int readers, writers, active_writers;
} rw_lock;

void rw_init(rw_lock *lock) {
    lock->readers = lock->writers = lock->active_writers = 0;
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->read_cond, NULL);
    pthread_cond_init(&lock->write_cond, NULL);
}

void read_lock(rw_lock *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->writers > 0 || lock->active_writers > 0) {
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }
    lock->readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void read_unlock(rw_lock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->readers--;
    if (lock->readers == 0) {
        pthread_cond_signal(&lock->write_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void write_lock(rw_lock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->writers++;
    while (lock->readers > 0 || lock->active_writers > 0) {
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }
    lock->writers--;
    lock->active_writers++;
    pthread_mutex_unlock(&lock->mutex);
}

void write_unlock(rw_lock *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->active_writers--;
    if (lock->writers > 0) {
        pthread_cond_signal(&lock->write_cond);
    } else {
        pthread_cond_broadcast(&lock->read_cond);
    }
    pthread_mutex_unlock(&lock->mutex);
}
