#include "syscall.h"

int
main()
{
ThreadFork();
ThreadYield();
  //std::cout<<result<<endl;
  Halt();
  /* not reached */
}