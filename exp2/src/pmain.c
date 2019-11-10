#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// 多进程
int main() {
    int pid1, pid2;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
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
    pid1 = fork();
    if (pid1 < 0) {
        perror("child1 NOT created");
        exit(-1);
    }
    if (pid1 == 0) {
        // cal Fac
        int x, fac = 1;
        close(pipe2[1]);
        close(pipe1[0]);
        read(pipe2[0], &x, sizeof(int));
        for (int i = 2; i <= x; ++i) fac *= i;
        printf("child1 cal factor f(x=%d)=%d\n", x, fac);
        write(pipe1[1], &fac, sizeof(int));
        close(pipe2[0]);
        close(pipe1[1]);
    } else {
        pid2 = fork();
        if (pid2 < 0) {
            perror("child2 NOT created");
            exit(-1);
        }
        if (pid2 == 0) {
            // cal Fibo
            int y, fibo1 = 1, fibo2 = 1;
            close(pipe4[1]);
            close(pipe3[0]);
            read(pipe4[0], &y, sizeof(int));
            for (int i = 3; i <= y; ++i) {
                int fibo = fibo1 + fibo2;
                fibo2 = fibo1;
                fibo1 = fibo;
            }
            printf("child2 cal fibonacci f(y=%d)=%d\n", y, fibo1);
            write(pipe3[1], &fibo1, sizeof(int));
            close(pipe4[0]);
            close(pipe3[1]);
        } else {
            int x, y, fac, fibo;
            close(pipe2[0]);
            close(pipe4[0]);
            close(pipe1[1]);
            close(pipe3[1]);
            scanf("%d%d", &x, &y);
            write(pipe2[1], &x, sizeof(int));
            write(pipe4[1], &y, sizeof(int));
            read(pipe1[0], &fac, sizeof(int));
            read(pipe3[0], &fibo, sizeof(int));
            printf("parent cal f(x=%d,y=%d)=%d\n", x, y, fac + fibo);
            close(pipe2[1]);
            close(pipe4[1]);
            close(pipe1[0]);
            close(pipe3[0]);
        }
    }
    return 0;
}