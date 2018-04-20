/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 7;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(8, OUTPUT);     
  pinMode(6, OUTPUT);     
  pinMode(5, OUTPUT);     
  pinMode(4, OUTPUT);     
  pinMode(3, OUTPUT);     
  pinMode(2, OUTPUT);     
  pinMode(1, OUTPUT);     
  pinMode(0, OUTPUT);     
  pinMode(11, OUTPUT);     

  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  //Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(0, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(2, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(3, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(5, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(6, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, LOW);   // turn the LED on (HIGH is the voltage level)
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
 delay(500);               // wait for a second
  
  /*Serial.print(analogRead(3));
  Serial.print("-");
  Serial.print(analogRead(2));
  Serial.print("-");
  Serial.print(analogRead(1));
  Serial.print("-");
  Serial.print(analogRead(0));
  Serial.println();*/
}
