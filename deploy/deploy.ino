#include <Servo.h>
#include <ServoInput.h>

// Setup radio port
ServoInputPin<2> channel;

// Setup servo ports and variables
Servo servo;
int servoPort1 = D7;
int servoPort2 = D6;
int currentDegree = 0;

// Define stepper motor connections and steps per revolution:
#define dirPin A3
#define stepPin A4
#define stepsPerRevolution 200

void setup() {
  // Setup Serial Monitor
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  Serial.print("start");

  // Setup Servo Motors
  servo1.attach(servoPort1);
  servo2.attach(servoPort2); 

  // Setup Stepper Motor Pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // Check for signal from radio in a loop
  boolean deploy = channel.getBoolean();
  if (deploy == true) {
    Serial.print("Deploying...\n");
    stepper();             // Run stepper demo program
    rotateDegrees(180, 1); // Rotate servo 1 by 180 degrees
    rotateDegrees(90, 2);  // rotate servo 2 by 90 degrees
    delay(3000);           // wait 3 seconds to avoid over-deploying
  }

// Rotate the servo to a target degree
void rotateToDegree(int targetDegree, int servo) {
  // Normalise the target degree
  while (targetDegree < 0) targetDegree = targetDegree + 360;
  targetDegree = targetDegree % 360;

  // Calculate the distancce to target and rotate
  int degrees = targetDegree - currentDegree;
  rotateDegrees(degrees, servo);
}

// Rotate the servo x degrees
void rotateDegrees(int degrees, int servo) {
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
  if (servo == 1){
    servo1.write(stationary);
    delay(degrees*2);
    servo1.write(dir);
  }
  if (servo == 2){
    servo2.write(stationary);
    delay(degrees*2);
    servo2.write(dir);
  }

  // Update the current degree
  currentDegree = currentDegree + degrees;
  if (currentDegree > 360) currentDegree - 360;
}



// Run a demo program on the servo motor
void stepper() {
  // Set the spinning direction clockwise:
  digitalWrite(dirPin, HIGH);

  // Set the spinning direction counterclockwise:
  digitalWrite(dirPin, LOW);

  // Spin the stepper motor 1 revolution quickly:
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);

  // Set the spinning direction clockwise:
  digitalWrite(dirPin, HIGH);

  // Spin the stepper motor 5 revolutions fast:
  for (int i = 0; i < 5 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(1000);

  // Set the spinning direction counterclockwise:
  digitalWrite(dirPin, LOW);

  //Spin the stepper motor 5 revolutions fast:
  for (int i = 0; i < 5 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(1000);
}