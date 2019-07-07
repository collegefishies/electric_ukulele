
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
  int oldtime;
  float lowpass = 0;
  float dt = 0;
  float T = 0;
  int runs = 0;
  int sensorVal = analogRead(micPin);

  while(1){
    lowpass = -cutoff*dt*(lowpass - sensorVal);
    oldtime = micros();

    sensorVal = analogRead(micPin);
    dt = (micros() - oldtime)*1e-6;

    analogWrite(dacPin, round(lowpass));

    T = T + dt;
    if(++runs % 1000 == 0){
      Serial.println(T/runs);
    }
  }
}

