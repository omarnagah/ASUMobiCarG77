// question 1
char led=13;
unsigned int dutyCycle=2000;
void setup() {
 pinMode (led,OUTPUT);

}

void loop() {
  digitalWrite (led,HIGH);
  delay (dutyCycle*.25);
  digitalWrite (led,LOW);
  delay (dutyCycle*.75);

}
