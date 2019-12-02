#include "monitor.h"
#include <stdio.h>
#include <unistd.h>
#include "ipc.h"

const char dirction[2][10] = {"left", "right"};

Sema::Sema(int semId) {
    this->semId = semId;
}

Sema::~Sema() {
    semctl(semId, 0, IPC_RMID);
}

int Sema::wait() {
    return P(semId);
}

int Sema::signal() {
    return V(semId);
}

Condition::Condition() {
    int ipcFlag = IPC_CREAT | 0644;
    int semKey = 2000;

    sema[0] = new Sema(set_sem(semKey++, 0, ipcFlag));
    sema[1] = new Sema(set_sem(semKey++, 0, ipcFlag));
    printf("Condition sema[0]=%d val=%d sema[1]=%d val=%d\n", sema[0]->semId, semctl(sema[0]->semId, 0, GETVAL), sema[1]->semId, semctl(sema[1]->semId, 0, GETVAL));
}

Condition::~Condition() {
    delete sema[0];
    delete sema[1];
}

void Condition::wait(Sema *mutex, int dirc) {
    mutex->signal();  // 开锁
    // printf("Condition (wait %d) %d sema[0].val=%d sema[1].val=%d mutex.val=%d\n", dirc, getpid(), semctl(sema[0]->semId, 0, GETVAL), semctl(sema[1]->semId, 0, GETVAL), semctl(mutex->semId, 0, GETVAL));
    sema[dirc]->wait();  // 在队列中等待
    // printf("Condition %d mutex.val=%d\n", getpid(), semctl(mutex->semId, 0, GETVAL));
    mutex->wait();  // 上锁
}

void Condition::signal(int dirc) {
    // printf("%d signal %d\n", getpid(), dirc);
    sema[dirc]->signal();  // 离开车道
    // printf("Condition (signal %d) %d sema[0].val=%d sema[1].val=%d\n", dirc, getpid(), semctl(sema[0]->semId, 0, GETVAL), semctl(sema[1]->semId, 0, GETVAL));
}

Monitor::Monitor(int maxNum) {
    this->maxNum = maxNum;

    int ipcFlag = IPC_CREAT | 0644;
    int shmKey = 1000;
    int shmVal = 1;
    int semKey = 1100;

    condition = new Condition();
    mutex1 = new Sema(set_sem(semKey++, 1, ipcFlag));
    mutex2 = new Sema(set_sem(semKey++, maxNum, ipcFlag));
    // printf("Monitor mutex=%d val=%d\n", mutex->semId, semctl(mutex->semId, 0, GETVAL));

    cnt[0] = (int *)set_shm(shmKey++, shmVal * sizeof(int), ipcFlag);
    cnt[1] = (int *)set_shm(shmKey++, shmVal * sizeof(int), ipcFlag);
    printf("Monitor cnt[0]=%p cnt[1]=%p\n", cnt[0], cnt[1]);
    currentDirc = (int *)set_shm(shmKey++, shmVal * sizeof(int), ipcFlag);
    crossing = (int *)set_shm(shmKey++, shmVal * sizeof(int), ipcFlag);
    *currentDirc = -1;  // 当前方向初始化为-1
    (*cnt[0]) = (*cnt[1]) = (*crossing) = 0;
}

Monitor::~Monitor() {
    delete mutex1;
    delete mutex2;
    delete condition;

    int shmKey = 1000;
    shmdt(cnt[0]);
    shmdt(cnt[1]);
    shmdt(currentDirc);
    shmdt(crossing);
    shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
    shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
    shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
    shmctl(get_ipc_id("/proc/sysvipc/shm", shmKey++), IPC_RMID, NULL);
}

void Monitor::arrive(int dirc) {
    printf("%d to %s arrived\n", getpid(), dirction[dirc]);
    mutex1->wait();
    *cnt[dirc] += 1;
    if (*currentDirc != -1 && (*currentDirc != dirc || *crossing >= maxNum)) {
        mutex1->signal();
        condition->sema[dirc]->wait();
        mutex2->wait();
    } else {
        mutex1->signal();
        mutex2->wait();
    }
    *cnt[dirc] -= 1;
    *crossing += 1;
    *currentDirc = dirc;

    printf("%d to %s is crossing, tot trains %d\n", getpid(), dirction[dirc], *crossing);
    sleep(1);

    *crossing -= 1;
    printf("%d to %s quit, tot trains %d\n", getpid(), dirction[dirc], *crossing);
    // printf("cnt[0]=%d cnt[1]=%d\n", *cnt[0], *cnt[1]);
    if (*crossing == 0) {
        if ((*cnt[dirc ^ 1]) > 0) {
            condition->signal(dirc ^ 1);
        } else if ((*cnt[dirc]) > 0) {
            condition->signal(dirc);
        } else {
            // printf("%d do nothing\n", getpid());
        }
    }
    mutex2->signal();
}