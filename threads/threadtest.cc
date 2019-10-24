
#include "main.h"
#include "thread.h"
#include "IOInterrupt.h"
IOInterrupt *iointobj;
void
CpuFunc()
{
    int i=0;
    while(i++<100)
    {
        //std::cout<<"Thread name: "<<kernel->currentThread->getName()<<"priority lvel: "<<kernel->currentThread->tflag<<endl;
        kernel->interrupt->OneTick();
    }
}

void
ReadFunc()
{
    int i=0;
    while(i++<5)
    {
        //std::cout<<"Thread name: "<<kernel->currentThread->getName()<<"priority lvel: "<<kernel->currentThread->tflag<<endl;
        iointobj->IORead();
    }
    
}

void
WriteFunc()
{
    int i=0;
    while(i++<10)
    {
        //std::cout<<"Thread name: "<<kernel->currentThread->getName()<<"priority lvel: "<<kernel->currentThread->tflag<<endl;
        iointobj->IOWrite("write");
    }
    
}

void 
MixedFunc()
{
    int i=0;
    while(i++<5)
    {
        iointobj->IORead();
    }
    while(i++<10)
    {
        iointobj->IOWrite("write");
    }

}
void 
hybridFunc()
{
    int i=0;
    while(i++<10)
    {
        iointobj->IORead();
    }
    while(i++<15)
    {
        iointobj->IOWrite("write");
    }

}
void
read10Func()
{
    int i=0;
    while(i++<10)
    {
        iointobj->IORead();
    }
    
}

void
write5Func()
{
    int i=0;
    while(i++<20)
    {
       iointobj->IOWrite("write"); 
    }
    
}
void
cpuintensiveFunc()
{
    int i=0;
    while(i++<200)
    {
        kernel->interrupt->OneTick();
    }
    
}
void 
writereadFunc()
{
    int i=0;
    while(i++<10)
    {
        iointobj->IOWrite("hello");
    }
    while(i++<5)
    {
        iointobj->IORead();
    }

}
void 
readwriteFunc()
{
    int i=0;
    while(i++<10)
    {
        iointobj->IORead(); 
    }
    while(i++<5)
    {
        iointobj->IOWrite("hello");
    }

}

void
ThreadTest()
{
    Thread *t1 = new Thread("Thread 1 ");
    t1->Fork((VoidFunctionPtr) CpuFunc, (void *) NULL);
    
    Thread *t2 = new Thread("Thread 2 ");
    t2->Fork((VoidFunctionPtr) ReadFunc, (void *) NULL);

    Thread *t3 = new Thread("Thread 3 ");
    t3->Fork((VoidFunctionPtr) WriteFunc, (void *) NULL);

    Thread *t4 = new Thread("Thread 4 ");
    t4->Fork((VoidFunctionPtr) MixedFunc, (void *) NULL);

    Thread *t5 = new Thread("Thread 5 ");
    t5->Fork((VoidFunctionPtr) hybridFunc, (void *) NULL);

    Thread *t6 = new Thread("Thread 6 ");
    t6->Fork((VoidFunctionPtr) read10Func, (void *) NULL);

    Thread *t7 = new Thread("Thread 7 ");
    t7->Fork((VoidFunctionPtr) write5Func, (void *) NULL);

    Thread *t8 = new Thread("Thread 8 ");
    t8->Fork((VoidFunctionPtr) cpuintensiveFunc, (void *) NULL);

    Thread *t9 = new Thread("Thread 9 ");
    t9->Fork((VoidFunctionPtr) writereadFunc, (void *) NULL);

    Thread *t10 = new Thread("Thread 10 ");
    t10->Fork((VoidFunctionPtr) readwriteFunc, (void *) NULL);

    //SimpleThread(0);
}
