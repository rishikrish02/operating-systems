#include "syscall.h"

int
main()
{
  ThreadYield();
  //std::cout<<result<<endl;
  Halt();
  /* not reached */
}