#ifndef __IPC__
#define __IPC__

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define BUFSZ 256

int get_ipc_id(char *proc_file, int key);

void *set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);

int P(int sem_id);
int V(int sem_id);

typedef union semuns {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    #if defined(_linux_)
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
    #endif
} Sem_uns;

// typedef struct msgbuf {
//     long mtype;
//     char mtext[1];
// } Msg_buf;

#endif  //__IPC__