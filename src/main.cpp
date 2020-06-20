#include <Arduino.h>

int lastTime = 0;
int thisTime = 0;
bool BPMTiming=false;
bool BeatComplete=false;
int lastVal = 0;
int BPM=0;
unsigned int prevTime =0;
#define UpperThreshold 560
#define LowerThreshold 530
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  thisTime = millis();
  int value = analogRead(A6);

    if(value>UpperThreshold)
  {
    if(BeatComplete)
    {
      BPM=thisTime-lastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
      Serial.println(BPM);
    }
    if(BPMTiming==false)
    {
      lastTime=millis();
      BPMTiming=true;
    }
  }
  if((value<LowerThreshold)&(BPMTiming))
    BeatComplete=true;





}