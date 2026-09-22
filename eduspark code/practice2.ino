#include <Wire.h>
#include <Servo.h>

const int echoPin = 5;  //from sensor
const int trigPin = 6;  //from sensor

int buzzer = 2;
long duration;
int distance;

void setup() {
  //sensor pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600); //Serial monitor on
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //distance calculation
  duration = pulseIn(echoPin, HIGH);
  distance = duration *0.034/2;

  //Output selection
  if (distance <= 8){  //Example 8cm. Buzzer on.
    digitalWrite(buzzer, HIGH);
  }
  delay(3000);

  //print on serial monitor
  Serial.print("Distance:");
  Serial.println(distance);
}
  