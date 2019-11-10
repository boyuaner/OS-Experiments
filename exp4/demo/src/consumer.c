#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ipc.h"

int main(int argc, char *argv[]) {
    int sec = argv[1] == NULL ? 2 : atoi(argv[1]);

    key_t buff_key = 101;
    key_t cget_key = 103;
    int buff_num = 8;
    int cget_num = sizeof(int) * 1;
    int shm_flag = IPC_CREAT | 0644;

    char *buff_ptr = (char *)set_shm(buff_key, buff_num, shm_flag);
    int *cget_ptr = (int *)set_shm(cget_key, cget_num, shm_flag);

    key_t prod_key = 201;
    key_t pmtx_key = 202;
    key_t cons_key = 301;
    key_t cmtx_key = 302;
    int sem_flag = IPC_CREAT | 0644;

    int prod_sem = set_sem(prod_key, buff_num, sem_flag);
    int cons_sem = set_sem(cons_key, 0, sem_flag);
    int cmtx_sem = set_sem(cmtx_key, 1, sem_flag);

    while (1) {
        P(cons_sem);
        P(cmtx_sem);

        char prod = buff_ptr[*cget_ptr];
        sleep(sec);
        printf("Consumer PID=%d get '%c' from Buffer[%d]\n", getpid(), prod, *cget_ptr);
        *cget_ptr = (*cget_ptr + 1) % buff_num;

        V(cmtx_sem);
        V(prod_sem);
    }

    return EXIT_SUCCESS;
}