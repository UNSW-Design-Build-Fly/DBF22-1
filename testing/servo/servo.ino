#include <Servo.h>

Servo servo;
int servoPort = 7;
int currentDegree = 0;

void setup() {
  servo.attach(servoPort);
  rotateDegrees(180);
  delay(5);
  rotateDegrees(180);

}

void loop() {
}

// Rotate the servo to a target degree
void rotateToDegree(int targetDegree) {
  // Normalise the target degree
  while (targetDegree < 0) targetDegree = targetDegree + 360;
  targetDegree = targetDegree % 360;

  // Calculate the distancce to target and rotate
  int degrees = targetDegree - currentDegree;
  rotateDegrees(degrees);
}

// Rotate the servo x degrees
void rotateDegrees(int degrees) {
  // Set Constants
  int forward = 90;
  int backward = 180;
  int stationary = 0;

  // Set offsets and directions
  int offset = 2;
  int dir = forward;
  if (degrees < 0){
    //degrees = degrees * -1;
    dir = backward;
  }

  // Rotate the servo
  servo.write(stationary);
  delay(degrees*2);
  servo.write(dir);

  // Update the current degree
  currentDegree = currentDegree + degrees;
  if (currentDegree > 360) currentDegree - 360;
}
