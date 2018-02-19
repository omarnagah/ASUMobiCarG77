int HighTime = 500;
int LowTime = 1500;
int LEDPIN = 13;
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPIN , OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LEDPIN , HIGH);
delay(HighTime);
digitalWrite (LEDPIN , LOW );
delay(LowTime);
}
