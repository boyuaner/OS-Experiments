#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *args[]) {
    int i;
    int pid[3];
    struct sched_param p[3];
    for (i = 0; i < 3; ++i) {
        pid[i] = fork();
        if (pid[i] == 0) {
            sleep(1);
            for (i = 0; i < 10; ++i) {
                printf("Child PID=%d priority=%d\n", getpid(), getpriority(PRIO_PROCESS, 0));
                sleep(1);
            }
            exit(0);
        }
        p[i].sched_priority = args[i + 1] == NULL ? 10 : atoi(args[i + 1]);
        setpriority(PRIO_PROCESS, pid[i], args[i + 1] == NULL ? 10 : atoi(args[i + 1]));
    }
    for (i = 0; i < 3; ++i) printf("My child %d policy is %d\n", pid[i], sched_getscheduler(pid[i]));
    return 0;
}