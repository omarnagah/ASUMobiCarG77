#define LM1 10 // left motor M1a
#define LM2 9 // left motor M2a
#define RM1 6 // right motor M2a
#define RM2 5 // right motor M2b
double timer,time_rev;
void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  timer=millis();
  while(!(digitalRead(2)))
  {
    analogWrite(LM1, 80);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    analogWrite(RM2, 80);
}
time_rev=millis()-timer;
}
void loop() {
      analogWrite(LM1, 80);
    digitalWrite(LM2, LOW);
    digitalWrite(RM2, LOW);
    analogWrite(RM1, 80);
    delay(1000);
        analogWrite(LM1, 80);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    analogWrite(RM2, 80);
    delay(time_rev/4);
}
