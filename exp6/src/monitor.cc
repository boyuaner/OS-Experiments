#include "monitor.h"
#include <stdio.h>
#include <unistd.h>

char dirction[2][10] = {"left", "right"};

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

Condition::Condition() {
    int ipcFlag = IPC_CREAT | 0644;
    int semKey = 220;
    int semVal = 0;

    sema[0] = new Sema(set_sem(semKey++, semVal, ipcFlag));
    sema[1] = new Sema(set_sem(semKey++, semVal, ipcFlag));
}

Condition::~Condition() {
    delete sema[0];
    delete sema[1];
}

void Condition::wait(Sema *lock, int dirc) {
    lock->signal();
    sema[dirc]->wait();
    lock->wait();
}

void Condition::signal(int dirc) {
    sema[dirc]->signal();
}

Monitor::Monitor(int rate) {
    this->rate = rate;

    int ipcFlag = IPC_CREAT | 0644;
    int shmKey = 120;
    int semKey = 210;
    int shmVal = 4 * sizeof(int);
    int semVal = 1;

    condition = new Condition();
    lock = new Sema(set_sem(semKey++, semVal, ipcFlag));

    shmPtr = set_shm(shmKey++, shmVal, ipcFlag);
    cnt[0] = (int *)shmPtr;
    cnt[1] = (int *)shmPtr + 1;
    currentDirc = (int *)shmPtr + 2;
    *currentDirc = -1;
}

Monitor::~Monitor() {
    delete lock;
    delete condition;

    int shmKey = 120;
    shmdt(shmPtr);
    shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
}

void Monitor::arrive(int dirc) {
    lock->wait();
    printf("%d to %s arrived\n", getpid(), dirction[dirc]);
    *cnt[dirc] = *cnt[dirc] + 1;
    if ((*currentDirc != -1 && *currentDirc != dirc) || *cnt[dirc] > MAXNUM) condition->wait(lock, dirc);
    lock->signal();
}

void Monitor::cross(int dirc) {
    lock->wait();
    *cnt[dirc] = *cnt[dirc] - 1;
    *currentDirc = dirc;
    sleep(rate);
    lock->signal();
}

void Monitor::quit(int dirc) {
    lock->wait();
    printf("%d to %s quit\n", getpid(), dirction[dirc]);
    printf("left: %d, right: %d\n", *cnt[0], *cnt[1]);
    if (*cnt[dirc ^ 1] > 0) {
        condition->signal(dirc ^ 1);
    } else if (*cnt[dirc] > 0) {
        condition->signal(dirc);
    } else {
        *currentDirc = -1;
    }
    lock->signal();
}