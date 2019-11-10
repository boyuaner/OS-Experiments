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

int main(int argc, char *argv[]) {
    int i, j;
    key_t sem_key[3], smoke_key = 100;
    int sem_id[3], id[3];
    int sem_flag = IPC_CREAT | 0644;
    int doneSmoke = set_sem(smoke_key, 0, sem_flag);
    for (i = 0; i < 3; ++i) {
        sem_key[i] = 101 + i;
        // 获取信号量
        sem_id[i] = set_sem(sem_key[i], 0, sem_flag);
    }
    for (i = 1; i <= 2; ++i) {
        int pid = fork();
        if (pid == 0) {
            srand(time(0));
            int sec = argv[i] == NULL ? 2 : atoi(argv[i]);
            while (1) {
                j = rand() % 3;
                V(sem_id[j]);
                printf("Producer(%d) provided {%s, %s}\n", i, materials[j][0], materials[j][1]);
                sleep(sec);
                P(doneSmoke);
            }
            exit(EXIT_SUCCESS);
        }
    }
    int pid, status;
    // 等待所有子进程
    while ((pid = wait(&status)) > 0);
    return EXIT_SUCCESS;
}