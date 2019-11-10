#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main() {
    int x, pid;
    int pipe1[2], pipe2[2];
    if(pipe(pipe1) < 0) {
        perror("pipe1 NOT created");
        exit(-1);
    }
    if(pipe(pipe2) < 0) {
        perror("pipe2 NOT created");
        exit(-1);
    }
    pid = fork();
    if(pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        do {
            read(pipe1[0], &x, sizeof(int));
            printf("child %d read: %d\n", getpid(), x++);
            write(pipe2[1], &x, sizeof(int));
        } while(x < 10);
        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);
        x = 1;
        do {
            write(pipe1[1], &x, sizeof(int));
            read(pipe2[0], &x, sizeof(int));
            printf("parent %d read: %d\n", getpid(), x++);
        } while(x < 10);
        close(pipe1[1]);
        close(pipe2[0]);
    }
    return 0;
}