#include "monitor.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

Sema::Sema(int semId) {
    this->semId = semId;
}

Sema::~Sema() {
    Sem_uns sem_arg;
    semctl(semId, 0, IPC_RMID, sem_arg);
}

int Sema::wait() {
    return P(semId);
}

int Sema::signal() {
    return V(semId);
}

Condition::Condition(char *state[5], Sema *sema) {
    this->state = state;
    this->sema = sema;
}

Condition::~Condition() {
    delete sema;
}

void Condition::wait(Sema *lock, int i) {
    if (*state[(i + 4) % 5] != eating && *state[i] == hungry && *state[(i + 1) % 5] != eating) {
        // 如果相邻两个哲学家都不在吃饭且当前哲学家饿了，则当前哲学家可以拿起筷子吃饭
        *state[i] = eating;
    } else {
        printf("p%d: %d hungry\n", i + 1, getpid());
        lock->signal();
        sema->wait();   // 当前哲学家等待筷子
        lock->wait();
    }
}

void Condition::signal(int i) {
    if (*state[(i + 4) % 5] != eating && *state[i] == hungry && *state[(i + 1) % 5] != eating) {
        // 如果相邻两个哲学家都不在吃饭且当前哲学家饿了，则当前哲学家可以拿起筷子吃饭
        sema->signal(); // 唤起等待筷子的哲学家
        *state[i] = eating;
    }
}

Monitor::Monitor(int rate) {
    int ipcFlag = IPC_CREAT | 0644;
    int shmKey = 220;
    int semKey = 120;

    this->rate = rate;

    lock = new Sema(set_sem(semKey++, 2, ipcFlag)); // 互斥锁 最多有两个哲学家可以同时吃饭

    for (int i = 0; i < 5; ++i) {
        state[i] = (char *)set_shm(shmKey++, 1 * sizeof(char), ipcFlag);
        *state[i] = thinking;
    }

    for (int i = 0; i < 5; ++i) {
        Sema *sema = new Sema(set_sem(semKey++, 0, ipcFlag));
        condition[i] = new Condition(state, sema);
    }
}

void Monitor::pickUp(int i) {
    lock->wait();
    *state[i] = hungry;
    condition[i]->wait(lock, i);    // 当前哲学家尝试拿筷子吃饭
    printf("p%d: %d eating\n", i + 1, getpid());
    sleep(rate);
    lock->signal();
}

void Monitor::putDown(int i) {
    lock->wait();
    *state[i] = thinking;
    printf("p%d: %d thinking\n", i + 1, getpid());
    // 尝试让左右两个哲学家吃饭
    condition[(i + 4) % 5]->signal((i + 4) % 5);    
    condition[(i + 1) % 5]->signal((i + 1) % 5);
    lock->signal();
    sleep(rate);
}

Monitor::~Monitor() {
    int shmKey = 220;
    delete lock;
    for (int i = 0; i < 5; ++i) {
        delete condition[i];
        shmdt((char *)state[i]);
        shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
    }
}