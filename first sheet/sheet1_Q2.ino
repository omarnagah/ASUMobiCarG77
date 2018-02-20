int waitTime = 1000;
void setup() {
  // put your setup code here, to run once:
  pinMode(4 , OUTPUT);
  pinMode(5 , OUTPUT);
  pinMode(6 , OUTPUT);
  pinMode(7 , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 4 ; i<=7 ; i++)
  {
    digitalWrite(i , HIGH);
    delay(waitTime);
    digitalWrite(i , LOW);
  }
    for(int i = 6 ; i>=4 ; i--)
  {
    digitalWrite(i , HIGH);
    delay(waitTime);
    digitalWrite(i , LOW);
  }
}
