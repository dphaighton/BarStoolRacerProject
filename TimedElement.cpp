#include "TimedElement.h"
#include "Arduino.h"



TimedElement::TimedElement(int upT,int downT,int eC)
{
    expireCounter = eC;
    uptime = upT;
    downtime = downT;
}


void TimedElement::call()//fix this for one time actions
{
    //add the time stuff to dT
    dT+=millis()-pastTime;
    
	if (dT >= downtime)
	{
		inCall();
	}
	
	if(dT>=(uptime+downtime))
    {
        dT=0;
        if(expireCounter!=FOREVER)
        {
          expireCounter--;
        }
    }
    
    pastTime=millis();

}  

bool TimedElement::isExpired()
{
    return expireCounter==0;
}

void TimedElement::inCall() 
{
	
}