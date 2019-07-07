
#include <Adafruit_NeoPixel.h>

const int micPin = A2;
const int powerPin = 10;
const int dacPin = A0;
const int ledPin = 5;
const int ledCount = 15;
const int batteryPin = A7;

void setup() {
  Serial.begin(20000000);
  pinMode(powerPin,OUTPUT);
  digitalWrite(powerPin,HIGH);
}

float cutoff = 1000.;
void loop() {
  int sensorVal;
  while(1){
    sensorVal = analogRead(micPin);
    analogWrite(dacPin,sensorVal);
    Serial.println(sensorVal);
  }
}

