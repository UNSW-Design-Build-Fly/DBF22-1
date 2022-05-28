#include <Servo.h>
#include <ServoInput.h>

// Setup radio port
ServoInputPin<2> channel;

// Setup servo ports and variables
Servo servo1;
Servo servo2;
Servo servo3;
#define servoPort1 7 // upper deck
#define servoPort2 6 // trapdoor
#define servoPort3 5 // lock servo
int currentDegree = 0;


// Counting packages
int totalPackages = 6;
int dropLevel = 3;
int currentPackages = 0;


// Define stepper motor connections and steps per revolution:
#define dirPin A3
#define stepPin A4
#define stepsPerRevolution 200

// Setup ultrasonic sensor (HC-SR04) variables
#define echoPin 3 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 4 //attach pin D4 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
  // Setup Serial Monitor
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  Serial.print("start\n");

  // Setup Servo Motors
  servo1.attach(servoPort1); 
  servo2.attach(servoPort2); 
  servo3.attach(servoPort3); 

  // Setup Stepper Motor Pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Setup Ultrasonic Sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
}

void loop() {
  // Check for signal from radio in a loop
  boolean deploy = channel.getBoolean();
  if (deploy == true) {
    Serial.print("Deploying...\n");

    if (currentPackages > dropLevel) {
      rotateDegrees(180, 1); // drop upper deck packages
      delay(1000);           // wait 1 second
    }
    
    rotateDegrees(45, 3);  // unlock lock servo
    
    // Run stepper if package not on trapdoor
    while (sensorPulse() > 10) {
      stepper();           // Run stepper demo program
      Serial.print("Stepper should be running\n");
      delay(1000);         // wait 1 second
    }
    
    rotateDegrees(-45, 3); // lock lock servo
    delay(1000);           // wait 1 second

    rotateDegrees(90, 2);  // open trapdoor
    delay(3000);           // wait 3 seconds
    rotateDegrees(-90, 2); // close trapdoor

    currentPackages += 1;
    Serial.print(currentPackages);
    Serial.print(" packages deployed\nFinished Deploying\n");
  }
  
}

// Rotate the servo x degrees
void rotateDegrees(int degrees, int servo) {
  // Set Constants
  int stationary = 90;
  int backward = 180;
  int forward = 0;

  // Set offsets and directions
  int offset = 2;
  int dir = forward;
  if (degrees < 0){
    degrees = degrees * -1;
    dir = backward;
  }

  // Rotate the servo
  if (servo == 1){
    servo1.write(stationary);
    delay(degrees*2);
    servo1.write(dir);
  }
  if (servo == 2){
    servo2.write(dir);
    delay(degrees*2);
    servo2.write(stationary);
  }
  if (servo == 3){
    servo3.write(stationary);
    delay(degrees*2);
    servo3.write(dir);
  }
}



// Run a demo program on the servo motor
void stepper() {
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
}

int sensorPulse() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Return and display the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
