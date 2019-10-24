#include "Request.h"

Thread* Request:: getthread()
{
    return thread;
}
void Request:: setthread(Thread* c)
{
    thread =c;
}
int Request:: getdelay_time()
{
    return delay_time;
}
void Request:: setdelay_time(int a)
{
    delay_time=a;
}
char* Request:: getstr()
{
    return str;
}
void Request:: setstr(char* b)
{
    str=b;
}

