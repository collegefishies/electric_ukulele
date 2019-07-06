#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#include <Adafruit_NeoPixel.h>
#include "arduinoFFT.h"
#include <FHT.h>
const int micPin = A2;
const int powerPin = 10;
const int dacPin = A0;
const int ledPin = 5;
const int ledCount = 15;
const int batteryPin = A7;


const int samplerate = 5000;
const int fftSize = 256;
const int spectrumSize = fftSize / 2;
int spectrume[spectrumSize];

Adafruit_NeoPixel strip(ledCount, ledPin, NEO_RGBW + NEO_KHZ800);
arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[fftSize];
double vImag[fftSize];

void setup() {

  Serial.begin(20000000);

  //pinMode(dacPin,OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin,HIGH);
  //analogWrite(dacPin,512);

  strip.begin();
  strip.show();
  strip.setBrightness(50);

  colorWipe(strip.Color(0,0,255,0));
  sampling_period_us = round(1000000*(1.0/samplerate));
  
    
}

int sensorVal;
int outVal = 500;
int meanVal = 488;
int hysteresis = 10;
int step = hysteresis;
int i = 0;
int starttime; 
int filtered = 0;
void loop() {
    
    /*SAMPLING*/
    for(int i=0; i<fftSize; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(micPin);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, fftSize, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, fftSize, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, fftSize);
    double peak = FFT.MajorPeak(vReal, fftSize, samplerate);
 
    /*PRINT RESULTS*/
    //Serial.println(peak);     //Print out what frequency is the most dominant.
 
    for(int i=0; i<(fftSize/2); i++)
    {
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
         
        //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / fftSize, 1);
        //Serial.print(" ");
        Serial.println(log(vReal[i]), 1);    //View only this line in serial plotter to visualize the bins
    }
 
    
    //while(1);       //Run code once
}

void colorWipe(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          // s Update strip to match
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
