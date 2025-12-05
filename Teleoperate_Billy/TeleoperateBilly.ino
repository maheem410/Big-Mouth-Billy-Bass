#include <AFMotor.h>
#include <SoftwareSerial.h>

// ---------------- HM-10 SoftwareSerial ----------------
#define BT_RX 9
#define BT_TX 10
SoftwareSerial BTSerial(BT_RX, BT_TX);

// ---------------- Motors ----------------
AF_DCMotor tailMotor(1);   // M1
AF_DCMotor mouthMotor(2);  // M2
AF_DCMotor turnMotor(3);   // M3

#define MOTOR_SPEED 200

// ---------------- Command Handling ----------------
String inputString = "";
bool stringComplete = false;

void setup() {
  // Initialize HM-10 serial
  BTSerial.begin(9600);

  // USB Serial for debug
  Serial.begin(9600);
  Serial.println("Arduino ready to receive commands from HM-10");

  // Initialize motor speeds
  mouthMotor.setSpeed(MOTOR_SPEED);
  tailMotor.setSpeed(MOTOR_SPEED);
  turnMotor.setSpeed(MOTOR_SPEED);
}

void loop() {
  // ---------------- Read HM-10 ----------------
  while (BTSerial.available()) {
    char inChar = (char)BTSerial.read();
    if (inChar == '\n') {
      stringComplete = true;
      break;
    } else {
      inputString += inChar;
    }
  }

  // ---------------- Handle commands ----------------
  if (stringComplete) {
    inputString.trim();
    Serial.println("Received: " + inputString); // debug

    if (inputString == "MOUTH") {
      mouthMotor.run(FORWARD);
      delay(250);
      mouthMotor.run(RELEASE);
    }
    else if (inputString == "TAIL") {
      tailMotor.run(FORWARD);
      delay(250);
      tailMotor.run(RELEASE);
    }
    else if (inputString == "DANCE") {
      tailMotor.run(FORWARD);
      turnMotor.run(FORWARD);
      delay(250);
      turnMotor.run(RELEASE);
      tailMotor.run(RELEASE);
    }
    else if (inputString == "RIGHT") {
      turnMotor.run(FORWARD);
      delay(250);
      turnMotor.run(RELEASE);
    }
    else if (inputString == "HEADTURN") {
      mouthMotor.setSpeed(255);  // optional faster mouth
      turnMotor.run(FORWARD);
      delay(250);
      turnMotor.run(RELEASE);
    }
    else if (inputString == "STOP") {
      mouthMotor.run(RELEASE);
      tailMotor.run(RELEASE);
      turnMotor.run(RELEASE);
    }

    // Reset for next command
    inputString = "";
    stringComplete = false;
  }
}
