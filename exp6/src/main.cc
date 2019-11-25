#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include "monitor.h"

Monitor *monitor;
int *dirc;

void beforeExit(int param) {
    if (param == SIGINT) {
        delete monitor;
        delete[] dirc;
    }
}

int main(int argc, char *argv[]) {
    int cnt, rate;
    if (argv[1] == NULL) {
        cnt = 10;
        rate = 2;
    } else if (argv[2] == NULL) {
        cnt = atoi(argv[1]);
        rate = 2;
    } else {
        cnt = atoi(argv[1]);
        rate = atoi(argv[2]);
    }

    dirc = new int[cnt];
    srand((unsigned int)time(0));
    for (int i = 0; i < cnt; ++i) dirc[i] = rand() % 2;

    monitor = new Monitor(rate);
    printf("MAX Volumn: %d\n", MAXNUM);
    for (int i = 0; i < cnt; ++i) {
        int pid = fork();
        if (pid == 0) {
            // printf("%d\n", getpid());
            sleep(1);
            monitor->arrive(dirc[i]);
            monitor->cross(dirc[i]);
            monitor->quit(dirc[i]);
            delete[] dirc;
            exit(EXIT_SUCCESS);
        }
    }

    signal(SIGINT, beforeExit);
    while (true) {
        int pid = wait(NULL);
        if (pid <= 0) break;
    }

    delete monitor;
    delete[] dirc;
    return EXIT_SUCCESS;
}