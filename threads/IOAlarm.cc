
#include "main.h"


IOAlarm::IOAlarm(bool doRandom)
{
    timer = new Timer(doRandom, this);
}

void 
IOAlarm::CallBack() 
{
    while(!kernel->rqlist->IsEmpty())
    {
        Request* reqobj = kernel->rqlist->Front();
        if(reqobj->getdelay_time()<=kernel->stats->totalTicks)
        {
           if(reqobj->getread_flag()==0)
              {
                  std::cout<<"Read_flag in read request:"<<reqobj->getread_flag()<<endl;
                  std::cout<<endl;
                  std::cout<<"Read current delay time:"<<reqobj->getdelay_time()<<endl;
                  std::cout<<endl;
                  std::cout<<"Read totalticks:"<<kernel->stats->totalTicks<<endl;
                  std::cout<<endl;
                  kernel->ioHandler->read(reqobj);
                  kernel->rqlist->RemoveFront();
              }
            else if(reqobj->getread_flag()==1)
              {
                  std::cout<<"Read_flag in write request:"<<reqobj->getread_flag()<<endl;
                  std::cout<<endl;
                  std::cout<<"Write current delay time:"<<reqobj->getdelay_time()<<endl;
                  std::cout<<endl;
                  std::cout<<"Write totalticks:"<<kernel->stats->totalTicks<<endl;
                  std::cout<<endl;
                  kernel->ioHandler->write(reqobj);
                  kernel->rqlist->RemoveFront();
              } 
        }
    else
    {
        break;
    }
    }
    
    
}