
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

double cutoff = 20000.*1e-6;
double hiCutoff = 10*1e-6;
void loop() {
  int sensorVal = analogRead(micPin);
  int oldLowpass = 0;
  double lowpass = 0;
  double bandpass = 0;
  double dblPass = 0;
  double oldBandpass = 0;
  int oldtime = micros();
  while(1){
    sensorVal = analogRead(micPin);
    oldLowpass = lowpass;
    oldBandpass = bandpass;

    lowpass = lowpass - (micros() - oldtime)*cutoff*(lowpass - sensorVal);  //lowpass
    bandpass = bandpass - (micros() - oldtime)*hiCutoff*(bandpass) + lowpass - oldLowpass; //first hi pass
    dblPass = dblPass - (micros() - oldtime)*hiCutoff*(dblPass) + bandpass - oldBandpass; //second hi pass

    analogWrite(dacPin,(int) 2*round(dblPass) + 500);
    Serial.println((int) round(dblPass));

    oldtime = micros(); 
  }
}

