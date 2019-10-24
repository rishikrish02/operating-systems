#include "syscall.h"

int
main()
{
  int result;
  char *buff;
  buff= "hello world! This is write syscall";
  result = Write(buff,30, 0);
  //std::cout<<result<<endl;
  Halt();
  /* not reached */
}