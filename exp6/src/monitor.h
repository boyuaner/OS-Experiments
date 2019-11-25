#include "ipc.h"
#define MAXNUM 1

enum State {empty, left, right};

class Sema {
public:
    Sema(int id);
    ~Sema();
    int wait();
    int signal();
private:
    int semId;
};

class Condition {
public:
    Condition();
    ~Condition();
    void wait(Sema *lock, int dirc);
    void signal(int dirc);
private:
    Sema *sema[2];
};

class Monitor {
public:
    Monitor(int rate);
    ~Monitor();
    void arrive(int dirc);
    void cross(int dirc);
    void quit(int dirc);
private:
    int rate;
    int *currentDirc;
    int *cnt[2];
    Sema *lock;
    Condition *condition;

    void *shmPtr;
};