
#include <Adafruit_NeoPixel.h>
#include <arduinoFFT.h>

//hardware interface
const int micPin = A2;
const int powerPin = 10;
const int dacPin = A0;
const int ledPin = 5;
const int ledCount = 15;
const int batteryPin = A7;

//fft parameters
const int samplerate = 5000;
const int fftSize = 256;
const int spectrumSize = fftSize / 2;
const unsigned int sampling_period_us = round(1e6/samplerate);

double vReal[fftSize];
double vImag[fftSize];

arduinoFFT FFT = arduinoFFT();



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
  unsigned long oldSampleTime = micros();
  unsigned long sampleTime = micros();

  int sampleI = 0;
  while(1){

    /*** Filter the input ***/
    sensorVal = analogRead(micPin);
    oldLowpass = lowpass;
    oldBandpass = bandpass;

    lowpass = lowpass - (micros() - oldtime)*cutoff*(lowpass - sensorVal);  //lowpass
    bandpass = bandpass - (micros() - oldtime)*hiCutoff*(bandpass) + lowpass - oldLowpass; //first hi pass
    dblPass = dblPass - (micros() - oldtime)*hiCutoff*(dblPass) + bandpass - oldBandpass; //second hi pass

    analogWrite(dacPin,(int) round(2*dblPass) + 500);
    Serial.println(dblPass);

    oldtime = micros(); 

    /*** Non blocking sampling of input ***/
    sampleTime = oldtime;

    if(sampleTime - oldSampleTime > sampling_period_us){
      vReal[sampleI] = dblPass;
      vImag[sampleI] = 0;

      sampleI = (sampleI + 1);
      oldSampleTime = sampleTime;
    }

    /*** FFT ***/
    if(sampleI != fftSize){
      continue;
    }

    continue;

    sampleI = 0;
    for(int i = 0; i < fftSize/2; i++){
      //Serial.println((vReal[i]));
    }

    FFT.Windowing(vReal, fftSize, FFT_WIN_TYP_BLACKMAN_HARRIS, FFT_FORWARD);
    FFT.Compute(vReal,vImag, fftSize, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal,vImag, fftSize);

    //print results
    
    
  }
}

