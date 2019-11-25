#include "ipc.h"

enum State {eating, thinking, hungry};

/*
 * 信号量的封装
 */
class Sema {
public:
    Sema(int semId);
    ~Sema();
    int wait();
    int signal();
private:
    int semId;
};

/*
 * 哲学家条件信号量
 * wait     判断是否可以吃饭
 * signal   唤起等待的可以吃饭的哲学家
 */
class Condition {
public:
    Condition(char *state[5], Sema *sema);
    ~Condition();
    void wait(Sema *lock, int i);
    void signal(int i);
private:
    char **state;   // 哲学家们的状态
    Sema *sema;     // 哲学家信号量
};

class Monitor {
public:
    Monitor(int rate);
    ~Monitor();
    void pickUp(int i);         // 拿起筷子吃饭
    void putDown(int i);        // 吃完饭发下筷子
private:
    int rate;
    Sema *lock;                 // 管程的互斥锁
    Condition *condition[5];    // 5个哲学家的条件信号量
    char *state[5];             // 5个哲学家的状态
};