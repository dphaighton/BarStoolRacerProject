/*
* The timed element is the command in the schedueler, "TimedList"
* The purpose of the TimedElement is to fake concurrent programming on the arduino.
*/
#ifndef TimedElement_h
#define TimedElement_h


class TimedElement
{
  public:
    const int FOREVER=-1;//this value, when subbed into the expireCounter, will make the command be read forever
    
    TimedElement(int upT,int downt,int eC);//the constructor requires these to run
    unsigned int uptime;//the amount of time the command to run
    unsigned int downtime;//the amount of time for the command to wait between being called
    int expireCounter;//the number of times the command can be called before being discarded
    
    void call();//run the command
    bool isExpired();//returns if the command is expired

  private:
    unsigned int dT=0;//the change in time between when the command was finished being read to the current time
    unsigned int pastTime=0;
  protected:
    virtual void inCall();//the function that defines what the command does
  };
#endif