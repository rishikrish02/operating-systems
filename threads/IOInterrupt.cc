#include "IOInterrupt.h"
Request* obj;
char* IOInterrupt::IORead()
{
    std::cout<<"IORead function inside IOInterrupt.cc"<<endl;
    Request* readobj = new Request(kernel->currentThread,kernel->stats->totalTicks,0);
    kernel->rqlist->Insert(readobj);
    std::cout<<"Inserting read event into request list"<<endl;
    //ioAlarm->Callback(rqlist);
    kernel->interrupt->SetLevel(IntOff);
    kernel->currentThread->Sleep(FALSE);
    kernel->interrupt->SetLevel(IntOn);
    return readobj->getstr();
}


void IOInterrupt::IOWrite(char* a)
{
    std::cout<<"IOWrite function inside IOInterrupt.cc"<<endl;
    Request* writeobj = new Request(kernel->currentThread,kernel->stats->totalTicks,1,a);
    kernel->rqlist->Insert(writeobj);
    std::cout<<"Inserting write event into request list"<<endl;
    //ioAlarm->Callback(rqlist);
    kernel->interrupt->SetLevel(IntOff);
    kernel->currentThread->Sleep(FALSE);
    kernel->interrupt->SetLevel(IntOn);
}