#include "TimedElement.h"


/*Currently, this is being used to test the TimedElement class
It will be used to turn the LED @ port 13 on and off,
It will be used to print out a bunch of messages sometimes
It should print out hello, and flash the LED at different rates and times
*/


class Messenger :public TimedElement//this is used to output a bunch of messages ever so often
{
private:
	String message;
	int count=0;
public:

	Messenger(String text) :
		TimedElement(100, 1000, TimedElement::FOREVER)
	{
		message = text;
	}

	void inCall()
	{
		count++;//count is used to show that it works
		Serial.println(message+count);
	}
};

class LED: public TimedElement
{
private:
	int port;
	bool on=false;
public:
	LED(int p):
		TimedElement(0,1000,TimedElement::FOREVER), port(p)
	{
		pinMode(port, OUTPUT);
	}

	void inCall() 
	{
		digitalWrite(port, on ? HIGH : LOW);
		on = !on;
	}
};



Messenger hello("hello");
LED L13(13);

void setup() 
{
  // put your setup code here, to run once:
	Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
	hello.call();
	L13.call();
}



