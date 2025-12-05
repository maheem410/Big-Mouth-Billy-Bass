#include "AnimateBillySketch.h"

int sensorPin = A0;
int ledPin = 13;

int sampleSensor(int pin, int samples = 20, int duration = 250) {
  float sensorValue = 0;
  int iterDelay = duration / samples;
  for (int i = 0; i < samples; i++) {
    sensorValue += analogRead(pin);
    delay(iterDelay);
  }
  digitalWrite(ledPin, LOW);
  return (int)(sensorValue / samples);
}

int sampleAverages(int pin, int samples = 3, int duration = 100) {
  int iterDelay = duration / samples;
  int avgValue = 0;
  for (int i = 0; i < samples; i++) {
    avgValue += sampleSensor(pin, samples, iterDelay);
    delay(iterDelay);
  }
  return (int)(avgValue / samples);
}

void setup() {
  setupFishMotors();
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

int stillPlaying = 0;
int aniDuration = 500;
int previousValue = 0;
int quietThreshold = 450;     // Threshold for detecting sound
int peakThreshold1 = 520;    
int previousMouthSpeed = 100;

void loop() {
  int sensorValue = sampleSensor(sensorPin);
  float smoothedSensorValue = (sensorValue + previousValue) / 2;
  
  if(smoothedSensorValue > quietThreshold) {
    digitalWrite(ledPin, HIGH);
    
    int rawMouthSpeed = map(smoothedSensorValue, quietThreshold, 650, 120, 255);    // Converts amplitude to mouth speed
    int mouthSpeed = (rawMouthSpeed * 3 + previousMouthSpeed) / 4;     // Smooths the mouth speed
    mouthSpeed = constrain(mouthSpeed, 120, 255);
    mouth.setSpeed(mouthSpeed);        // Sets the mouth speed
    previousMouthSpeed = mouthSpeed;  // Stores the current mouth speed for smoothing
    
    Serial.print("Amplitude: ");
    Serial.print((int)smoothedSensorValue);
    Serial.print(" -> Mouth Speed: ");
    Serial.print(mouthSpeed);
    
    stillPlaying = min(stillPlaying + 1, 4);
    
    // Peak detection: Any peak triggers full body movement
    if(smoothedSensorValue > peakThreshold1) {
      Serial.println(" [PEAK - Dance + Mouth!]");
      performFullBodyWithDance(aniDuration, mouthSpeed);  // Sequential: Dance first, then mouth
    } else {
      Serial.println(" [Normal - Mouth Only]");
      mouthOnly(aniDuration);
    }
    
  } else if (stillPlaying == 0){    // If no sound is detected, the fish goes to sleep
      billySleep();
      digitalWrite(ledPin, LOW);
      stillPlaying = 0;
      mouth.setSpeed(255);
      previousMouthSpeed = 100;
  } else {  // If sound is detected, the fish continues to play
    stillPlaying--;
  }
  
  previousValue = sensorValue;
}

