int pin1=3;
void setup() {

pinMode(pin1,OUTPUT);
}

void loop() {
  digitalWrite(pin1,HIGH);
  delay(500);
  digitalWrite(pin1,LOW);
  delay(1500);

}
