#include "syscall.h"

int
main()
{
  int result;
  char *buff;
  
  result = Read(buff,12,3);

  Halt();
  /* not reached */
}