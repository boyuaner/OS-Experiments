#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void getSum(int *);
void getFibo();
void getFac();

int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
pthread_t thrd1, thrd2, thrd3;
// 多线程
int main() {
    if (pipe(pipe1) < 0) {
        perror("pipe1 NOT created");
        exit(-1);
    }
    if (pipe(pipe2) < 0) {
        perror("pipe2 NOT created");
        exit(-1);
    }
    if (pipe(pipe3) < 0) {
        perror("pipe3 NOT created");
        exit(-1);
    }
    if (pipe(pipe4) < 0) {
        perror("pipe4 NOT created");
        exit(-1);
    }
    int x[2];
    scanf("%d%d", &x[0], &x[1]);
    if (pthread_create(&thrd1, NULL, (void *)getSum, (void *)x)) {
        perror("thread1 NOT created");
        exit(-1);
    }
    if (pthread_create(&thrd2, NULL, (void *)getFibo, NULL)) {
        perror("thread2 NOT created");
        exit(-1);
    }
    if (pthread_create(&thrd3, NULL, (void *)getFac, NULL)) {
        perror("thread3 NOT created");
        exit(-1);
    }
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    return 0;
}

void getSum(int *args) {
    int fac, fibo;
    write(pipe2[1], &args[0], sizeof(int));
    write(pipe4[1], &args[1], sizeof(int));
    read(pipe1[0], &fac, sizeof(int));
    read(pipe3[0], &fibo, sizeof(int));
    printf("thread1 cal f(x=%d, y=%d)=%d\n", args[0], args[1], fac + fibo);
    close(pipe2[1]);
    close(pipe4[1]);
    close(pipe1[0]);
    close(pipe3[0]);
}

void getFibo() {
    int y, fibo1 = 1, fibo2 = 1;
    read(pipe4[0], &y, sizeof(int));
    for(int i = 3; i <= y; ++i) {
        int fibo = fibo1 + fibo2;
        fibo2 = fibo1;
        fibo1 = fibo;
    }
    printf("thread2 cal fibonacci f(y=%d)=%d\n", y, fibo1);
    write(pipe3[1], &fibo1, sizeof(int));
    close(pipe4[0]);
    close(pipe3[1]);
}

void getFac() {
    int x, fac = 1;
    read(pipe2[0], &x, sizeof(int));
    for(int i = 2; i <= x; ++i) fac *= i;
    printf("thread3 cal factor f(x=%d)=%d\n", x, fac);
    write(pipe1[1], &fac, sizeof(int));
    close(pipe2[0]);
    close(pipe1[1]);
}