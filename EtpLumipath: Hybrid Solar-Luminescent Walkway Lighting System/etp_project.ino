const int trigPin1 = 10;  // Trigger pin for Ultrasonic Sensor 1
const int echoPin1 = 11;  // Echo pin for Ultrasonic Sensor 1
const int trigPin2 = 3;   // Trigger pin for Ultrasonic Sensor 2
const int echoPin2 = 4;   // Echo pin for Ultrasonic Sensor 2
int LedPin1 = 2;
int LedPin2 = 8;
int lightSensorPin = A0;
int analogValue = 0;
long duration1;  // Echo travel time for Sensor 1
long duration2;  // Echo travel time for Sensor 2
int distance1;
int distance2;
unsigned long led1StartTime = 0;
bool led1On = false;

unsigned long led2StartTime = 0;
bool led2On = false;
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //libraries

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27, 16 columns, 2 rows

void setup() {
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT); 
  Serial.begin(115200);   // Start Serial Monitor at 115200 baud
  pinMode (LedPin2, OUTPUT);
  pinMode(LedPin1, OUTPUT);

  lcd.init();        // Initialize LCD
lcd.backlight();   // Turn on backlight

lcd.setCursor(0,0);
lcd.print("System Ready");
delay(1000);
lcd.clear();
}

void loop() {
  analogValue = analogRead (lightSensorPin);
  Serial.println(analogValue);
 if (analogValue > 500){
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, LOW);

    led1On = false;
    led2On = false;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LDR:");
    lcd.print(analogValue);

    delay(500);

    return; ;
 }

  if (analogValue < 500) {
//ULTRASONIC SENSOR 1
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
 
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
 
    duration1 = pulseIn(echoPin1, HIGH);
    if (duration1 == 0)
    return 999;
 
    distance1 = duration1 * 0.034 / 2; //Convert time into distance (cm) 0.034cm/μs (SPEED OF SOUND IN AIR)
 
    Serial.print("Distance1: ");
    Serial.println(distance1);

//ULTRASONIC SENSOR 2
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
  
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
   
    duration2 = pulseIn(echoPin2, HIGH);
    if (duration2 == 0)
    return 999;

    distance2 = duration2 * 0.034 / 2;

    Serial.print("Distance2: ");
    Serial.println(distance2);

    // Show distances
lcd.clear();
lcd.setCursor(0,0);
lcd.print("D1:");
lcd.print(distance1);
lcd.print("cm ");

lcd.print("LDR:");
lcd.print(analogValue);

lcd.setCursor(0,1);
lcd.print("D2:");
lcd.print(distance2);
lcd.print("cm");





    if (distance1 < 14)
    {
        digitalWrite(LedPin1, HIGH);
        led1StartTime = millis();
        led1On = true;
    }

    if (distance2 < 14)
    {
        digitalWrite(LedPin2, HIGH);
        led2StartTime = millis();
        led2On = true;
    }

    // Turn off Red LED after 1 second
    if (led1On && millis() - led1StartTime >= 1000)
    {
        digitalWrite(LedPin1, LOW);
        led1On = false;
    }

    // Turn off White LED after 1 second
    if (led2On && millis() - led2StartTime >= 1000)
    {
        digitalWrite(LedPin2, LOW);
        led2On = false;
    }
  }
}