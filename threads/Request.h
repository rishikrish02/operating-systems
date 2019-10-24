#include "thread.h"

class Request{
    public:
    Request(Thread *current_Thread,int ticks, int flag, char* str="")
    {
        thread = current_Thread;
        read_flag=flag;
        if (flag==0)
        {
           delay_time = ticks + rand()% 101;  
        }
        else if(flag==1)
        {
            delay_time = ticks + rand()%51;
        }
        
    }
    Thread* getthread();
    void setthread(Thread* c);
    int getdelay_time();
    void setdelay_time(int a);
    char* getstr();
    void setstr(char* b);
    int getread_flag(){return read_flag;}
    void setread_flag(int d){read_flag=d;}
private:
Thread* thread;
int delay_time;
int read_flag;
char* str; 
};
