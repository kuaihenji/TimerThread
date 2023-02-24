#include "TimerThread.h"

Cu_Timer::Cu_Timer()
    :iInterval(0),iHeapIndex(-1)
{
    pThread = new TimerThread();
}

Cu_Timer::Cu_Timer(TimerThread *thread)
    :iInterval(0),iHeapIndex(-1)
{
    if( thread == nullptr){
        pThread = new TimerThread();
    }else {
        pThread = thread;
    }
}

Cu_Timer::~Cu_Timer()
{
    stopCuTimer();
}

void Cu_Timer::setInterval(cu_uint interval)
{
    this->iInterval = interval;
}

void Cu_Timer::startCuTimer()
{
    if( this->iInterval == 0 ){
        setInterval();
    }

    lExpires = iInterval + pThread->getCurrentSecond();

    pThread->addCuTimer(this);

    pThread->startThread();
}

void Cu_Timer::stopCuTimer()
{
    if ( iHeapIndex != -1 )
    {
        pThread->removeCuTimer(this);
        iHeapIndex = -1;
    }

    pThread->stopThread();

}

void Cu_Timer::doSomething()
{
    std::cout<<"time-up..."<<std::endl;
}

void Cu_Timer::handle(cu_llong now)
{
    lExpires = iInterval + now;
    pThread->addCuTimer(this);

    doSomething();
}

TimerThread::TimerThread()
{

}

TimerThread::~TimerThread()
{
    stopThread();
}

cu_llong TimerThread::getCurrentSecond()
{
#ifdef _MSC_VER
    _timeb timebuffer;
    _ftime(&timebuffer);
    cu_llong ret = timebuffer.time;
//    ret = ret * 1000 + timebuffer.millitm;
      ret = ret * 1000;
    return ret;
#else
    timeval tv;
    ::gettimeofday(&tv, 0);
    cu_llong ret = tv.tv_sec;
//    return ret * 1000 + tv.tv_usec / 1000;
      return ret * 1000;
#endif
}

void TimerThread::startThread()
{
    this->bDone = false;
    mThread = std::thread([&]{
        this->runThread();
    });

    mThread.detach();
}

void TimerThread::stopThread()
{
    this->bDone = true;
}

void TimerThread::runThread()
{
    while ( !bDone )
    {
        this->detectCuTimer();

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
}

void TimerThread::detectCuTimer()
{
     cu_llong now = getCurrentSecond();

     while (!veHeap.empty() && veHeap[0].time <= now)
     {
         Cu_Timer *timer = veHeap[0].pTimer;
         removeCuTimer(timer);
         timer->handle(now);
     }
}

void TimerThread::addCuTimer(Cu_Timer *timer)
{
    timer->iHeapIndex = veHeap.size();
    ST_HeapEntry entry = { timer->lExpires, timer};
    veHeap.push_back(entry);
    upHeap(veHeap.size() - 1);
}

void TimerThread::removeCuTimer(Cu_Timer *timer)
{
    size_t index = timer->iHeapIndex;
    if (!veHeap.empty() && index < veHeap.size())
    {
            if (index == veHeap.size() - 1)
            {
                  veHeap.pop_back();
            }
            else
            {
                   swapHeap(index, veHeap.size() - 1);
                   veHeap.pop_back();

                   size_t parent = (index - 1) / 2;
                   if (index > 0 && veHeap[index].time < veHeap[parent].time)
                       upHeap(index);
                   else
                       downHeap(index);
            }
     }
}

void TimerThread::upHeap(size_t index)
{
    size_t parent = (index - 1) / 2;
    while (index > 0 && veHeap[index].time < veHeap[parent].time)
    {
            swapHeap(index, parent);
            index = parent;
            parent = (index - 1) / 2;
     }
}

void TimerThread::downHeap(size_t index)
{
    size_t lchild = index * 2 + 1;
    while (lchild < veHeap.size())
    {
        size_t minChild = (lchild + 1 == veHeap.size() || veHeap[lchild].time < veHeap[lchild + 1].time) ? lchild : lchild + 1;
        if (veHeap[index].time < veHeap[minChild].time)
            break;
        swapHeap(index, minChild);
        index = minChild;
        lchild = index * 2 + 1;
    }
}

void TimerThread::swapHeap(size_t index1, size_t index2)
{
    ST_HeapEntry tmp = veHeap[index1];
    veHeap[index1] = veHeap[index2];
    veHeap[index2] = tmp;
    veHeap[index1].pTimer->iHeapIndex = index1;
    veHeap[index2].pTimer->iHeapIndex = index2;
}
