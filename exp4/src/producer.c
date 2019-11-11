#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include "ipc.h"

const char materials[3][10] = {"tobacco", "paper", "match"};

// 共享内存
key_t buff_key = 200;
key_t put_key = 202;
int *buff_ptr;
int *put_ptr;

// 互斥锁
key_t pmtx_key = 302;
int pmtx_id;

// 信号量
key_t sem_key[3], smoke_key = 100;
int sem_id[3], doneSmoke;

int pid;

int main(int argc, char *argv[]) {
    int i, j, sec, status;
    int m1, m2;
    int sem_flag = IPC_CREAT | 0644;
    buff_ptr = (int *)set_shm(buff_key, sizeof(int) * 4, sem_flag);
    put_ptr = (int *)set_shm(put_key, sizeof(int), sem_flag);
    pmtx_id = set_sem(pmtx_key, 1, sem_flag);
    doneSmoke = set_sem(smoke_key, 0, sem_flag);
    for (i = 0; i < 3; ++i) {
        sem_key[i] = 101 + i;
        // 获取信号量
        sem_id[i] = set_sem(sem_key[i], 0, sem_flag);
    }
    for (i = 1; i <= 2; ++i) {
        pid = fork();
        if (pid == 0) {
            srand(time(0));
            sec = argv[i] == NULL ? 2 : atoi(argv[i]);
            while (1) {
                P(pmtx_id);
                j = rand() % 3;
                if (j == 0) {
                    m1 = 0;
                    m2 = 1;
                } else if (j == 1) {
                    m1 = 0;
                    m2 = 2;
                } else {
                    m1 = 1;
                    m2 = 2;
                }
                buff_ptr[*put_ptr] = m1;
                *put_ptr = (*put_ptr + 1) % 4;
                buff_ptr[*put_ptr] = m2;
                *put_ptr = (*put_ptr + 1) % 4;
                V(sem_id[j]);
                printf("Producer(%d) provided {%s, %s}\n", i, materials[m1], materials[m2]);
                sleep(sec);
                V(pmtx_id);
                P(doneSmoke);
            }
            exit(EXIT_SUCCESS);
        }
    }
    // 等待所有子进程
    while ((pid = wait(&status)) > 0);
    Sem_uns sem_arg;
    // 删除信号量
    semctl(pmtx_id, 0, IPC_RMID, sem_arg);
    // 删除共享内存
    shmdt(put_ptr);
    shmctl(get_ipc_id("/proc/sysvipc/shm", put_key), IPC_RMID, NULL);
    return EXIT_SUCCESS;
}