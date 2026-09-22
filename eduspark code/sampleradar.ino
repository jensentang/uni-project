#include <Wire.h>
#include <Servo.h>

const int trigPin = 5;  //from sensor
const int echoPin = 3;  //from sensor
int R_LED = 2;    //red LED
int G_LED = 4;    //green LED
int buzz = 7;
long duration;
int distance;
int pos = 0;

Servo myservo;

void setup() {
  //sensor pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myservo.attach(6); //input signal to pin 6

  //LED and buzzer pin setup
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600); //Serial monitor on
}

void loop() {
  //from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos += 30){
    myservo.write(pos); //servo moves
    delay(400);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    //distance calculation
    duration = pulseIn(echoPin, HIGH);
    distance = duration *0.034/2;

    //Output selection
    if (distance <= 2){  //Example 3cm. Buzzer on.
      digitalWrite(buzz, HIGH);
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, LOW);
    }else if(distance <= 8){  //Example 6cm. Red LED on.
      digitalWrite(buzz, LOW);
      digitalWrite(R_LED, HIGH);
      digitalWrite(G_LED, LOW);
    } else {  //Example 15cm. Green LED on.
      digitalWrite(buzz, LOW);
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, HIGH);
    }
    delay(3000);

       //print on serial monitor
    Serial.print("Distance:");
    Serial.println(distance);

  }
  //from 180 to 0 degree
  for (int pos = 180; pos >= 0; pos -= 30){
    myservo.write(pos); //servo moves
    delay(400);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    //distance calculation
    duration = pulseIn(echoPin, HIGH);
    distance = duration *0.034/2;

 
    //output selection
    if (distance < 2){  //Example 3cm. Buzzer on.
      digitalWrite(buzz, HIGH);
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, LOW);
    }else if(distance < 8){  //Example 16cm. Red LED on.
      digitalWrite(buzz, LOW);
      digitalWrite(R_LED, HIGH);
      digitalWrite(G_LED, LOW);
    } else {  //Example 55cm. Green LED on.
      digitalWrite(buzz, LOW);
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, HIGH);
    }

    delay(3000);
    //print on serial monitor
    Serial.print("Distance:");
    Serial.println(distance);

  }
}
  