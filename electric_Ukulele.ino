
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

double cutoff = 2000.*1e-6;
void loop() {
  int sensorVal;
  double lowpass = 0;
  int oldtime = micros();
  while(1){
    sensorVal = analogRead(micPin);
    lowpass = lowpass - (micros() - oldtime)*cutoff*(lowpass - sensorVal);
    
    analogWrite(dacPin,sensorVal);
    Serial.println((int) round(lowpass));

    oldtime = micros(); 
  }
}

