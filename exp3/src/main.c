#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void solve(int param) {
    if(param == SIGINT) {
        setpriority(PRIO_PROCESS, getpid(), getpriority(PRIO_PROCESS, 0) + 1);
    } else if(param == SIGTSTP) {
        setpriority(PRIO_PROCESS, getpid(), getpriority(PRIO_PROCESS, 0) - 1);
    }
}

int main() {
    signal(SIGINT, solve);
    signal(SIGTSTP, solve);
    int pid = fork();
    if(pid == 0) {
        while(1) {
            printf("Child PID=%d priority=%d\n", getpid(), getpriority(PRIO_PROCESS, 0));
            pause();
        }
        exit(0);
    }
    while(1)    {
        printf("Parent PID=%d priority=%d\n", getpid(), getpriority(PRIO_PROCESS, 0));
        pause();
    }
    return 0;
}