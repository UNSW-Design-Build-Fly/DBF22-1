#include <ServoInput.h>
 
ServoInputPin<2> ch;
const int LED_Pin = LED_BUILTIN;  // built-in LED (pin 13 on Uno)
 
void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  pinMode(LED_Pin, OUTPUT);
  Serial.print("start");
}

void loop() {
  boolean state = ch.getBoolean();
  digitalWrite(LED_Pin, state);
  if (state == true) {
    Serial.print("signal\n");
    delay(3000);
  }
}
