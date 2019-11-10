#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
void solve(int param) {
    char *args[] = {"/bin/ls", "-al", NULL};
    printf("I am process1 %d\n", getpid());
    for (int i = 0; args[i] != NULL; ++i) printf("%s ", args[i]);
    puts("");
    execve(args[0], args, NULL);
}
int main() {
    int cnt = 0, status;
    while (1) {
        printf("Round %d\n", ++cnt);
        int pid1 = fork();
        if (pid1 == 0) {
            signal(SIGUSR1, solve);
            pause();
            break;
        }
        int pid2 = fork();
        if (pid2 == 0) {
            char *args[] = {"/bin/ps", NULL};
            printf("I am process2 %d\n", getpid());
            for (int i = 0; args[i] != NULL; ++i) printf("%s ", args[i]);
            puts("");
            execve(args[0], args, NULL);
            break;
        }
        waitpid(pid2, &status, 0);
        kill(pid1, SIGUSR1);
        waitpid(pid1, &status, 0);
        sleep(2);
    }
    return 0;
}