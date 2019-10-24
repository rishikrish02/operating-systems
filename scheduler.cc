// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------


Scheduler::Scheduler()
{ 
    //readyList = new List<Thread *>; 
    toBeDestroyed = NULL;
    priority1 = new List<Thread *>();
    priority2 = new List<Thread *>();
    priority3 = new List<Thread *>();
    priority4 = new List<Thread *>();
    // std::cout<<kernel->timequant1<<endl;
    // std::cout<<kernel->timequant2<<endl;
    // std::cout<<kernel->timequant3<<endl;
    // std::cout<<kernel->timequant4<<endl;
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    //delete readyList; 
    delete priority1;
    delete priority2;
    delete priority3;
    delete priority4;
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());

    thread->setStatus(READY);
    //readyList->Append(thread);
    //std::cout<<"thread name    "<<thread->getName()<<endl;
    if(thread->tflag==1)
    {
        priority1->Append(thread);
        thread->tflag+=1;
    }
    else if(thread->tflag==2)
    {
        priority2->Append(thread);
        thread->tflag+=1;
    }
    else if(thread->tflag==3)
    {
        priority3->Append(thread);
        thread->tflag+=1;
    }
    else if(thread->tflag==4)
    {
        priority4->Append(thread);
    }
    // std::cout<<"Num in proity 1:"<<priority1->NumInList()<<endl;
    // std::cout<<"Num in proity 2:"<<priority2->NumInList()<<endl;
    // std::cout<<"Num in proity 3:"<<priority3->NumInList()<<endl;
    // std::cout<<"Num in proity 4:"<<priority4->NumInList()<<endl;
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    //std::cout<<" inside find next run"<<endl;
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    //Print();
    Starvation();
    
    if(!priority1->IsEmpty())
    {
        //std::cout<<"Num in proity 1:"<<priority1->NumInList()<<endl;
        //std::cout<<" if priority is not  empty"<<priority1->NumInList()<<endl;
        Print();
        return priority1->RemoveFront();
    }
    else
    {
        //std::cout<<" if priority is empty"<<endl;
        if(!priority2->IsEmpty())
        {
            Print();
             return priority2->RemoveFront();
        }
        else 
        {
            if(!priority3->IsEmpty())
            {
                Print();
                return priority3->RemoveFront();
            }
            else
            {
                if(!priority4->IsEmpty())
                {
                    Print();
                     return priority4->RemoveFront();
                 }
                 else
                 {
                     return NULL;
                 }
                 
            }
        }
        
    }
    
    
    
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
// 
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread, bool finishing)
{
    Thread *oldThread = kernel->currentThread;
    
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    // std::cout<<"thread name in run  ::"<<nextThread->getName()<<endl;
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    SWITCH(oldThread, nextThread);

    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}
 
//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    // cout << "Ready list contents:\n";
    // readyList->Apply(ThreadPrint);
    std::cout<<"Priority Level 1"<<" ";
    priority1->Apply(ThreadPrint);
    std::cout<<endl;
    std::cout<<"Priority Level 2"<<" ";
    priority2->Apply(ThreadPrint);
    std::cout<<endl;
    std::cout<<"Priority Level 3"<<" ";
    priority3->Apply(ThreadPrint);
    std::cout<<endl;
    std::cout<<"Priority Level 4"<<" ";
    priority4->Apply(ThreadPrint);
    std::cout<<endl<<"\n";
}

void
Scheduler::Starvation()
{
     ListIterator<Thread*> iterpri2(priority2);
            for(; !iterpri2.IsDone(); iterpri2.Next())
            {
              Thread* queueobj2 = iterpri2.Item();
              if(((kernel->stats->totalTicks)-(queueobj2->getstarvation()))>1000)
              {
                  std::cout<<"Starvation time of object:"<<queueobj2->getstarvation()<<endl;
                  queueobj2->tflag= queueobj2->tflag-1;
                  priority2->Remove(queueobj2);
                  kernel->scheduler->ReadyToRun(queueobj2);
              }
            }

     ListIterator<Thread*> iterpri3(priority3);
            for(; !iterpri3.IsDone(); iterpri3.Next())
            {
              Thread* queueobj3 = iterpri3.Item();
              if(((kernel->stats->totalTicks)-(queueobj3->getstarvation()))>1000)
              {
                  queueobj3->tflag= queueobj3->tflag-1;
                  priority3->Remove(queueobj3);
                  kernel->scheduler->ReadyToRun(queueobj3);
              }
            }

         ListIterator<Thread*> iterpri4(priority4);
            for(; !iterpri4.IsDone(); iterpri4.Next())
            {
              Thread* queueobj4 = iterpri4.Item();
              if(((kernel->stats->totalTicks)-(queueobj4->getstarvation()))>1000)
              {
                  queueobj4->tflag= queueobj4->tflag-1;
                  priority4->Remove(queueobj4);
                  kernel->scheduler->ReadyToRun(queueobj4);
              }
            }
}