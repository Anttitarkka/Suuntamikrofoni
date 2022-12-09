#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalogStereo   adcs1;          //xy=180,180
AudioEffectDelay         delay2;         //xy=388,99
AudioEffectDelay         delay1;         //xy=389,246
AudioMixer4              mixer1;         //xy=555,165
AudioOutputAnalog        dac1;           //xy=705,158
AudioConnection          patchCord1(adcs1, 0, delay2, 0);
AudioConnection          patchCord2(adcs1, 1, delay1, 0);
AudioConnection          patchCord3(delay2, 0, mixer1, 0);
AudioConnection          patchCord4(delay1, 0, mixer1, 1);
AudioConnection          patchCord5(mixer1, dac1);
// GUItool: end automatically generated code

const int up = 2;     // Buttons control the microphone direction
const int down = 7;
int state1 = 0;       // States for checking if buttons are pressed
int lastState1 = 0;
int state2 = 0;
int lastState2 = 0;
float deg = 91.0;     // Default direction is straight
float degPrev = 91.0; 
float dpar1 = 0.0;    
float dpar2 = 0.0;

void setup() {
  // put your setup code here, to run once:´
  pinMode(up, INPUT_PULLUP);   // initialize the pushbutton pin as an input:
  pinMode(down, INPUT_PULLUP);   // initialize the pushbutton pin as an input:
  AudioMemory(60);  // Allocate audiomemory blocks for delay
  delay1.delay(0, dpar1); //Initialize delay and mixers
  delay2.delay(0, dpar2);
  mixer1.gain(0, 1.0);
  mixer1.gain(1, 1.0);
}

float calculateDelay(int mic, float x) { 
  float d;
  if (mic == 1){
    if (x < 91.0 && x >= 1.0) { // Checks if delay is positive
      d = 0.583062*cos(0.0174538*(-1 + x));
    } else {
      d = 0.0;
    }
  } else {
    if (x > 91.0 && x <= 181.0) { // Checks if delay is positive
      d = 0.583062*cos(0.0174538*(-181 + x));
    } else {
      d = 0.0;
    }
  }
  return d;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  state1 = digitalRead(up);
  if (state1 != lastState1) {
    if (state1 == LOW && deg < 181.0){
      if (deg > 176.5) {
        deg = 181.0;
      } else {
        deg = deg + 4.5;  
      } 
    } 
  }

  state2 = digitalRead(down);
  if (state2 != lastState2) {
    if (state2 == LOW && deg > 1.0){
      if (deg < 5.5) {
        deg = 1.0;
      } else {
        deg = deg - 4.5;  
      } 
    } 
  }
  
  if (deg <= (degPrev + 4.5) || deg >= (degPrev - 4.5)) {
    degPrev = deg;
    dpar1 = calculateDelay(1, deg);
    dpar2 = calculateDelay(2, deg);
  }
  
  lastState1 = state1;
  lastState2 = state2;
  delay1.delay(0, dpar1);
  delay2.delay(0, dpar2);
}
