#include <AFMotor.h>

#define NUM_MOTORS 3
AF_DCMotor head(1, MOTOR12_1KHZ); 
AF_DCMotor mouth(2, MOTOR12_1KHZ); 
AF_DCMotor tail(3, MOTOR34_1KHZ); 


void runMotorOnOff(AF_DCMotor motor, int aniDelay = 500){
      motor.run(FORWARD);
      delay(aniDelay);
      motor.run(BACKWARD);
      delay(5);
      motor.run(RELEASE);
      delay(aniDelay);
}

void runTwoMotorsOnOff(AF_DCMotor motor1, AF_DCMotor motor2, int aniDelay = 500){
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(aniDelay);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      delay(5);
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      delay(aniDelay);
}


bool randomDecisionToTurnOn(int oddsOn) {
  int score = random(1,100);
  return score < oddsOn;
}

void releaseMotor(AF_DCMotor motor) {
  motor.run(BACKWARD);
  delay(15);
  motor.run(RELEASE);
}

void tapTail(int duration, int times=5){
  int iterDuration = duration / times;
  for (int i = 0; i < times; i++) {
     runMotorOnOff(tail, iterDuration);
  }
}

void dance(int duration, int times = 1){
  int elapsedTime = 0;
  int iterDuration = duration / times;
  for (int i = 0; i < times; i++) {
    if(randomDecisionToTurnOn(60)){
      runTwoMotorsOnOff(head, tail, iterDuration);
    } else {
     tapTail(iterDuration/2, times*2);
    }
  }
}

void talk(int duration, int wordCount) {
  int iterDuration = duration / wordCount;
  //head.run(FORWARD);
  for (int i = 0; i < wordCount; i++) {
     runMotorOnOff(mouth, iterDuration);
  }
}

AF_DCMotor motors[NUM_MOTORS] = {head, mouth, tail};
void setupFishMotors() {
  Serial.begin(9600);
  Serial.println("Motor test!");
  for (int i = 0; i < NUM_MOTORS; i++) {
    motors[i].setSpeed(255);
  }
}

void billySleep(){
  head.run(RELEASE);
  tail.run(RELEASE);
  mouth.run(RELEASE);
}


// Normal audio: Mouth only moves
void mouthOnly(int duration) {
  // Mouth speed already set in main loop based on amplitude
  // Just pulse the mouth with random word count
  int wordCount = random(2, 6);
  int iterDuration = duration / wordCount;
  
  for (int i = 0; i < wordCount; i++) {
    runMotorOnOff(mouth, iterDuration);
  }
}

// Peak audio: All motors active (mouth continues + head + tail join)
// void performFullBody(int duration, int mouthSpeed) {
//   // Head and tail activate at peak
//   head.run(FORWARD);
//   tail.run(FORWARD);
  
//   // Head movement based on level (would need moveHead functions defined)
//   // Currently commented out - see moveHeadSmall/Medium/Large below
  
//   // Mouth keeps pulsing with amplitude-responsive speed (already set)
//   int wordCount = random(2, 6);
//   int iterDuration = duration / wordCount;
  
//   for (int i = 0; i < wordCount; i++) {
//     runMotorOnOff(mouth, iterDuration);
//   }
  
//   // Release tail after mouth finishes
//   tail.run(BACKWARD);
//   delay(5);
//   tail.run(RELEASE);
// }


// NEW: Peak audio with dance behavior + mouth movements (SEQUENTIAL - dance then mouth)
void performFullBodyWithDance(int duration, int mouthSpeed) {
  // Execute dance function (60% chance head+tail together, 40% chance tail only)
  dance(duration, 1);  // Use 80% of duration for dance (e.g., 400ms instead of 250ms)
  
  // After dance, continue with mouth movements
  int wordCount = random(2, 6);
  int iterDuration = (duration * 0.5) / wordCount;  // Use remaining time for mouth
  
  for (int i = 0; i < wordCount; i++) {
    runMotorOnOff(mouth, iterDuration);
  }
}

