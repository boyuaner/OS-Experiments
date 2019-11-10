#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ipc.h"

int main(int argc, char *argv[]) {
    int sec = argv[1] == NULL ? 2 : atoi(argv[1]);

    key_t buff_key = 101;
    key_t pput_key = 102;
    int buff_num = 8;
    int pput_num = sizeof(int) * 1;
    int shm_flag = IPC_CREAT | 0644;

    char *buff_ptr = (char *)set_shm(buff_key, buff_num, shm_flag);
    int *pput_ptr = (int *)set_shm(pput_key, pput_num, shm_flag);

    key_t prod_key = 201;
    key_t pmtx_key = 202;
    key_t cons_key = 301;
    key_t cmtx_key = 302;
    int sem_flag = IPC_CREAT | 0644;

    int prod_sem = set_sem(prod_key, buff_num, sem_flag);
    int cons_sem = set_sem(cons_key, 0, sem_flag);
    int pmtx_sem = set_sem(pmtx_key, 1, sem_flag);

    while (1) {
        P(prod_sem);
        P(pmtx_sem);

        buff_ptr[*pput_ptr] = 'A' + *pput_ptr;
        sleep(sec);
        printf("Producer PID=%d put '%c' to Buffer[%d]\n", getpid(), buff_ptr[*pput_ptr], *pput_ptr);
        *pput_ptr = (*pput_ptr + 1) % buff_num;

        V(pmtx_sem);
        V(cons_sem);
    }

    return EXIT_SUCCESS;
}