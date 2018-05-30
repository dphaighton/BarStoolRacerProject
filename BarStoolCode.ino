#include <LiquidCrystal.h>  
#include <Servo.h>

//Controlling motor
#define VICTOR 3
#define THROTTLE A1
#define EBRAKE A2

//light outputs
#define HEADLIGHT 4
#define LEFTBACK 5
#define RIGHTBACK 6

//light inputs
#define HBUTTON A3
#define TLEFT A4
#define TRIGHT A5

//speed reader
#define ENCODER A0
#define LCD1 7
#define LCD2 8
#define LCD3 9
#define LCD4 10
#define LCD5 11
#define LCD6 12

#define CIRC 0.5//circumfrence in metres

//the data required for an async delay
struct ADelay
{
  int last;
  int wait;  
};

Servo motor;
LiquidCrystal lcd(LCD1,LCD2,LCD3,LCD4,LCD5,LCD6);

void setup() 
{
pinMode(EBRAKE,INPUT);

pinMode(HEADLIGHT,OUTPUT);
pinMode(LEFTBACK,OUTPUT);
pinMode(RIGHTBACK,OUTPUT);

pinMode(HBUTTON,INPUT);
pinMode(TRIGHT,INPUT);
pinMode(TLEFT,INPUT);

lcd.begin(16,0x02);
motor.attach(VICTOR);

Serial.begin(9600);
}

ADelay flashDelay={0,600};
ADelay buttonDelay={0,300};
ADelay encoderDelay={0,150};
ADelay dT={0,0};

//if the delay is over
bool AOkay(ADelay& ad)
{
  return (ad.last+ad.wait<=millis());
}

//reset the time on it
void resetDelay(ADelay& ad)
{
  ad.last = millis();
}



float motorSpeed;
float readSpeed;
//128EMPTY|64EMPTY|32FLASHCOUNTER|16HEADLIGHTS|8RSOLID|4LSOLID|2RFLASH|1LFLASH
byte lightData=0;

void loop()
{
  //if the arduino can flash again
  if(AOkay(flashDelay))
  {
    invertBit(lightData,0x20);
    resetDelay(flashDelay);
  }
  
  
  //deals with all the button inputs
  if(AOkay(buttonDelay))//button delay bc I dont want it to trigger twice and cancel
  {
    
    if(digitalRead(HBUTTON))
    {
      invertBit(lightData,16);
      digitalWrite(HEADLIGHT,(lightData&16)==16);
      resetDelay(buttonDelay);  
      
    }
    if(digitalRead(TLEFT))
    {
      invertBit(lightData,0x01);
      resetDelay(buttonDelay);
    }
    if(digitalRead(TRIGHT))
    {
      
      invertBit(lightData,0x02);
      resetDelay(buttonDelay);
    }
  }
  
  //deal with turning lights
  digitalWrite(LEFTBACK, ((lightData&4)!=0||(lightData&33)==33));
  digitalWrite(RIGHTBACK,((lightData&8)!=0||(lightData&34)==34));
  
  motorSpeed = digitalRead(EBRAKE)?0.0f:(float)(analogRead(THROTTLE)/1023.0f);
  motor.write((float)(90*(motorSpeed+1)));
  
  if(digitalRead(ENCODER)&&AOkay(encoderDelay))
  {
  dT.wait = millis()-dT.last;
  readSpeed = (float)((CIRC*3600)/(dT.wait));
  dT.last = millis();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(motorSpeed*100.0f)+"%");
  lcd.setCursor(0,1);
  lcd.print(String(readSpeed)+" km/h");

  
  }
  Serial.println(String(digitalRead(A0)));
}

void invertBit(byte& b, byte n)
{
 if((b&n)==n)
 {
  b-=n;
  } 
  else
  {
    b|=n;
  }
}

void printByte(byte b)
{
  
  for(int i=128;i>=1;i/=2)
  {
    Serial.print((b|i)==b?1:0);
  }
  Serial.print("\n");
}

