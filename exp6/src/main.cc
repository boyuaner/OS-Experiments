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
    int cnt, maxNum;
    if (argv[1] == NULL) {
        cnt = 10;
        maxNum = 1;
    } else if (argv[2] == NULL) {
        cnt = atoi(argv[1]);
        maxNum = 1;
    } else {
        cnt = atoi(argv[1]);
        maxNum = atoi(argv[2]);
    }
    // cnt = 10;
    dirc = new int[cnt];
    srand((unsigned int)time(0));
    for (int i = 0; i < cnt; ++i) dirc[i] = rand() % 2;
    // dirc[0] = 1;
    // dirc[1] = 1;
    // dirc[2] = 1;
    // dirc[3] = 1;
    // dirc[4] = 0;

    // dirc[5] = 1;
    // dirc[6] = 0;
    // dirc[7] = 0;
    // dirc[8] = 0;
    // dirc[9] = 1;


    monitor = new Monitor(maxNum);
    printf("MAX NUM: %d\n", maxNum);
    for (int i = 0; i < cnt / 2; ++i) {
        int pid = fork();
        if (pid == 0) {
            sleep(1);
            monitor->arrive(dirc[i]);
            delete[] dirc;
            exit(EXIT_SUCCESS);
        }
    }

    sleep(3);

    for (int i = cnt / 2; i < cnt; ++i) {
        int pid = fork();
        if (pid == 0) {
            sleep(1);
            monitor->arrive(dirc[i]);
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