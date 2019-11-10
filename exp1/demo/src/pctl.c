#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
void sigcat(int param) {
    printf("%d Process continue\n", getpid());
}
int main(int argc, char *argv[]) {
    char *args[] = {"/bin/ls", "-a", NULL};
    signal(SIGINT, sigcat);
    int pid = fork(), status;
    if (pid == 0) {
        printf("I am Child process %d\nMy father is %d\n", getpid(), getppid());
        pause();
        printf("%d child will Running: \n", getpid());
        if (argv[1] != NULL) {
            for (int i = 1; argv[i] != NULL; ++i) printf("%s ", argv[i]);
            puts("");
            status = execve(argv[1], &argv[1], NULL);
        } else {
            for (int i = 0; args[i] != NULL; ++i) printf("%s ", args[i]);
            puts("");
            status = execve(args[0], args, NULL);
        }
    } else {
        printf("\nI am Parent process %d\n", getpid());
        if (argv[1] != NULL) {
            printf("%d Waiting for child done.\n\n", getpid());
            waitpid(pid, &status, 0);
            printf("\nMy child exit! status = %d\n\n", status);
        } else {
            if (kill(pid, SIGINT) >= 0) printf("%d Wakeup %d child.\n", getpid(), pid);
            printf("%d don't Wait for child done.\n\n", getpid());
        }
    }
    return EXIT_SUCCESS;
}