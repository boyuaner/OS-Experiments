#ifndef __MONITOR_H_
#define __MONITOR_H_
#define SLEEP 1

class Sema {
public:
    Sema(int semId);
    ~Sema();
    int wait();
    int signal();
    
    int semId;
};

class Condition {
public:
    Condition();
    ~Condition();
    void wait(Sema *mutex, int dirc);
    void signal(int dirc);

    Sema *sema[2];
};

class Monitor{
public:
    Monitor(int maxNum);
    ~Monitor();
    void arrive(int dirc);

    int maxNum;
    int *crossing;
    int *currentDirc;
    int *cnt[2];
    Sema *mutex1, *mutex2;
    Condition *condition;
};

#endif //__MONITOR_H_