

#include "copyright.h"
#include "schalarm.h"
#include "main.h"


schalarm::schalarm(bool doRandom)
{
    timer = new Timer(doRandom, this);
}


void 
schalarm::CallBack() 
{
    // Interrupt *interrupt = kernel->interrupt;
    // MachineStatus status = interrupt->getStatus();
    
    if (status != IdleMode) {
	interrupt->YieldOnReturn();
    if (kernel->currentThread->get_flag()==1)
    {
       quantum = kernel->timequant1; 
    }
    else if(kernel->currentThread->get_flag()==2)
    {
        quantum=kernel->timequant2;
    }
    else if(kernel->currentThread->get_flag()==3)
    {
        quantum=kernel->timequant3;
    }
    else if(kernel->currentThread->get_flag()==4)
    {
        quantum=kernel->timequant4;
    }
    }
}
