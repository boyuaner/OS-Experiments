#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "monitor.h"

Monitor *monitor;

void before_exit(int param) {
    if (param == SIGINT) {
        delete monitor;
    }
}

int main(int argc, char *argv[]) {
    int rate = argv[1] == NULL ? 3 : atoi(argv[1]);
    int pid;
    monitor = new Monitor(rate);
    signal(SIGINT, before_exit);
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (pid == 0) {
            while (1) {
                monitor->pickUp(i);
                monitor->putDown(i);
            }
        }
    }
    while (true) {
        pid = wait(NULL);
        if (pid <= 0) break;
    }
    return EXIT_SUCCESS;
}