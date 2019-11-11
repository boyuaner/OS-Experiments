#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include "ipc.h"

const char materials[3][10] = {"tobacco", "paper", "match"};

// 共享内存
key_t buff_key = 200;
key_t get_key = 201;
int *buff_ptr;
int *get_ptr;

// 互斥锁
key_t smtx_key = 301;
int smtx_id;

// 信号量
key_t sem_key, smoke_key = 100;
int sem_id, doneSmoke;

int pid;

void detach(int sig) {
    Sem_uns sem_arg;
    // 删信号量
    if (pid == 0) {
        semctl(sem_id, 0, IPC_RMID, sem_arg);
    } else {
        semctl(doneSmoke, 0, IPC_RMID, sem_arg);
        semctl(smtx_id, 0, IPC_RMID, sem_arg);
        // 删除共享内存
        shmdt((char *)buff_ptr);
        shmdt((char *)get_ptr);
        shmctl(get_ipc_id("/proc/sysvipc/shm", buff_key), IPC_RMID, NULL);
        shmctl(get_ipc_id("/proc/sysvipc/shm", get_key), IPC_RMID, NULL);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int i, status;
    int m1, m2;
    int sem_flag = IPC_CREAT | 0644;
    buff_ptr = (int *)set_shm(buff_key, sizeof(int) * 4, sem_flag);
    get_ptr = (int *)set_shm(get_key, sizeof(int) * 1, sem_flag);
    smtx_id = set_sem(smtx_key, 1, sem_flag);
    doneSmoke = set_sem(smoke_key, 0, sem_flag);
    signal(SIGINT, detach);
    for (i = 0; i < 3; ++i) {
        pid = fork();
        if (pid == 0) {
            sem_key = 101 + i;
            // 等待信号量
            sem_id = set_sem(sem_key, 0, sem_flag);
            int sec = argv[i] == NULL ? 2 : atoi(argv[i]);
            while (1) {
                P(sem_id);
                P(smtx_id);
                sleep(sec);
                m1 = buff_ptr[*get_ptr];
                *get_ptr = (*get_ptr + 1) % 4;
                m2 = buff_ptr[*get_ptr];
                *get_ptr = (*get_ptr + 1) % 4;
                printf("Smoker(%d) got {%s, %s}\n", i + 1, materials[m1], materials[m2]);
                V(smtx_id);
                V(doneSmoke);
            }
        }
    }
    // 等待所有子进程结束
    while ((pid = wait(&status)) > 0);
    detach(SIGINT);
    return EXIT_SUCCESS;
}