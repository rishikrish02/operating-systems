#include "copyright.h"
#include "utility.h"
#include "callback.h"
#include "timer.h"

// The following class defines a software alarm clock. 
class IOAlarm : public CallBackObj {
  public:
    IOAlarm(bool doRandomYield);	// Initialize the timer, and callback 
				// to "toCall" every time slice.
    ~IOAlarm() { delete timer; }
    
    void WaitUntil(int x);	// suspend execution until time > now + x
                                // this method is not yet implemented

  private:
    Timer *timer;		// the hardware timer device

    void CallBack();		// called when the hardware
				// timer generates an interrupt
};


