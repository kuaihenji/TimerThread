#include "TimerThread.h"


class CustomTimer : public Cu_Timer
{
public:
    CustomTimer(TimerThread *thread):Cu_Timer(thread){

    }

    ~CustomTimer(){}

    void doSomething(){
        std::cout<<"2 time-up..."<<std::endl;
    }

};


#define CUTIMER_NEW(pointer,constructor) \
    do{ pointer = new constructor;if(pointer == 0){ errno = -1;}}while(0)

int main(int argc,char *argv[])
{

    TimerThread *pThread = new TimerThread();
//    Cu_Timer *timer = new Cu_Timer(pThread);
//    timer->setInterval(1*1000);
//    timer->startCuTimer();

//    CustomTimer *timer = new CustomTimer(pThread);
//    timer->setInterval(1000);
//    timer->startCuTimer();

    Cu_Timer *pTimer = nullptr;
    CUTIMER_NEW(pTimer,Cu_Timer);
    if( pTimer != nullptr ){
        pTimer->startCuTimer();
    }


    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));
    }

   return 0;
}

