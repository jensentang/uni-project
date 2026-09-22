int R_LED = 2;
int G_LED = 4;
int buzzer = 6;

void setup() {
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(R_LED, HIGH);  //turn the red LED on
  digitalWrite(G_LED, HIGH);  //turn the green LED on
  delay(3000);              // wait for 3 seconds

  digitalWrite(R_LED, LOW);   //turn off the red LED
  digitalWrite(G_LED, LOW);   //turn off the green LED

  digitalWrite(buzzer, HIGH);   //turn off the buzzer
  delay(1000);              //wait for a second
  digitalWrite(buzzer, LOW);   //turn off the buzzer
}
