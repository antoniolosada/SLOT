void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly: 
  Serial.print("p1=");
  Serial.print(analogRead(2));
  Serial.print("p2=");
  Serial.println(analogRead(3));
  delay(1000);
  digitalWrite(7,HIGH);
  delay(2000);
  digitalWrite(7,LOW);
  delay(2000);
}
