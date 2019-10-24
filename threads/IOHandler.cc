#include<iostream>
#include "main.h"

void IOHandler:: read(Request* obj)
{
    kernel->currentThread->tflag=1;
    std::cout<<"IOhandler read function invoked by"<<" "<<kernel->currentThread->getName()<<endl;
    kernel->scheduler->ReadyToRun(obj->getthread());

}

void IOHandler:: write(Request* obj)
{
    kernel->currentThread->tflag=1;
    std::cout<<"IOHandler write function is invoked by"<<" "<<kernel->currentThread->getName()<<endl;
    kernel->scheduler->ReadyToRun(obj->getthread());
}