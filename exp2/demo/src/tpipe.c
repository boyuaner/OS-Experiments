#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int p1[2], p2[2];
pthread_t thrd1, thrd2;

void task1(int *);
void task2(int *);

int main() {
    int num1 = 1, num2 = 2;
    if (pipe(p1) < 0) {
        perror("pipe1 NOT created");
        exit(-1);
    }
    if (pipe(p2) < 0) {
        perror("pipe2 NOT created");
        exit(-1);
    }
    if (pthread_create(&thrd1, NULL, (void *)task1, (void *)&num1)) {
        perror("thread1 NOT created");
        exit(-1);
    }
    if (pthread_create(&thrd2, NULL, (void *)task2, (void *)&num2)) {
        perror("thread2 NOT created");
        exit(-1);
    }
    pthread_join(thrd2, NULL);
    pthread_join(thrd1, NULL);
    return 0;
}

void task1(int *num) {
    int x = 1;
    do {
        printf("thread%d read: %d\n", *num, x++);
        write(p1[1], &x, sizeof(int));
        read(p2[0], &x, sizeof(int));
    } while (x < 10);
    close(p1[1]);
    close(p2[0]);
}

void task2(int *num) {
    int x;
    do {
        read(p1[0], &x, sizeof(int));
        printf("thread%d read: %d\n", *num, x++);
        write(p2[1], &x, sizeof(int));
    } while (x < 10);
    close(p1[0]);
    close(p2[1]);
}
