#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>

#ifdef _MSC_VER
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif

typedef int cu_int;
typedef unsigned int cu_uint;
typedef unsigned long long cu_llong;

class TimerThread;
class Cu_Timer
{
public:
    Cu_Timer();
    Cu_Timer(TimerThread  *thread);
    ~Cu_Timer();

    void setInterval(cu_uint interval = 1000 );

    void startCuTimer();

    void stopCuTimer();

    virtual void doSomething();

protected:

    void handle(cu_llong now);


private:
    friend class TimerThread;
    TimerThread *pThread;
    cu_uint iInterval;
    cu_llong  lExpires;
    cu_int  iHeapIndex;
};



class TimerThread
{
public:
    TimerThread();
    ~TimerThread();

    cu_llong getCurrentSecond();

    void startThread();

    void stopThread();

protected:

    void runThread();

    void detectCuTimer();

    friend class Cu_Timer;
    void addCuTimer(Cu_Timer *timer);

    void removeCuTimer(Cu_Timer *timer);

    void upHeap(size_t index);

    void downHeap(size_t index);

    void swapHeap(size_t index1, size_t index2);

private:

    bool bDone;
    std::thread mThread;

    struct ST_HeapEntry
    {
        cu_llong time;
        Cu_Timer* pTimer;
    };
    std::vector<ST_HeapEntry> veHeap;
};

#endif // TIMERTHREAD_H
