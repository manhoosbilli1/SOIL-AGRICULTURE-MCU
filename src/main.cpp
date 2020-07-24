#include <Arduino.h>

//dht settings
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Gas settings
#define gasPin A1
#define lowerThreshold 400
#define upperThreshold 900

//moisture settings
#define moisturePin A6
#define lowerThreshold 400
#define upperThreshold 900

//Ph settings
#define phPin A0
#define lowerThreshold 400
#define upperThreshold 900
unsigned long int avgValue; //Store the average value of the sensor feedback
float b;
int buf[10], temp;

//variables
int pin_value;
int sensorVolts;

String encode(float p, float m, float d, float g)
{
  String data = "*";
  data += String(p, 2);
  data += ',';
  data += String(m, 2);
  data += '!';
  data += String(d, 2);
  data += '@';
  data += String(g, 2);
  data += '?';

  return data;
}

float getDhtValue()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  return h;
}

float getGasValue()
{
  pin_value = analogRead(gasPin);
  return float(pin_value);
}


float getMoistureValue()
{
  float pin_value = analogRead(moisturePin);
  return pin_value;
}



float getPhValue()
{
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(phPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) //take the average value of 6 center sample
  {
    avgValue += buf[i];
  }
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 3.5 * phValue;                          //convert the millivolt into pH value

  return phValue;
}


void setup()
{
  // put your setup code here, to run once:
  Serial3.begin(9600);
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(gasPin, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  dht.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(1000);
  float phValue = getPhValue();
  float moisture = getMoistureValue();
  float gasValue = getGasValue();
  float humidity = getDhtValue();
  // String ptr = "Gas Value: ";
  // ptr += String(gasValue,2);
  // ptr += "   Moisture value: ";
  // ptr += String(moisture,2);
  // ptr += "   Ph Data: ";
  // ptr += String(phValue,2);
  // ptr += "   Humidity: ";
  // ptr += String(humidity,2);

  // Serial.println(ptr);

  String sensorData = encode(phValue,moisture,humidity,gasValue);
  Serial3.println(sensorData);
  Serial.println(sensorData);
  
}
