#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include "ipc.h"

char materials[3][2][10] = {
    {"tobacco", "paper"},
    {"tobacco", "match"},
    {"paper", "match"},
};

key_t sem_key, smoke_key = 100;
int pid, sem_id, doneSmoke;

void detach(int sig) {
    Sem_uns sem_arg;
    // 删信号量
    if (pid == 0) {
        semctl(sem_id, 0, IPC_RMID, sem_arg);
    } else {
        semctl(doneSmoke, 0, IPC_RMID, sem_arg);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int i, status;
    int sem_flag = IPC_CREAT | 0644;
    // 完成信号量
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
                sleep(sec);
                printf("Smoker(%d) got {%s, %s}\n", i + 1, materials[i][0], materials[i][1]);
                V(doneSmoke);
            }
        }
    }
    // 等待所有子进程结束
    while ((pid = wait(&status)) > 0);
    detach(SIGINT);
    return EXIT_SUCCESS;
}