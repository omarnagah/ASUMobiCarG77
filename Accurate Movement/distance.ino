#define LM1 6 // left motor M1a
#define LM2 5 // left motor M2a
#define RM1 10 // right motor M2a
#define RM2 9 // right motor M2b

const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

int n=0;
float distance;
void setup() {
distance = 60;
  prevmillis = 0;
  prevstate = LOW;  
  Serial.begin(9600);
  
  analogWrite(LM1, 70);
  digitalWrite(LM2, LOW);
  analogWrite(RM1, 70);
  digitalWrite(RM2, LOW);
}     
void loop() {
while(n<(57*(distance/100)))
  {
             Serial.println(distance);
  currentstate = digitalRead(dataIN); // Read IR sensor state
 if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
       n++;
       }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan
  }
    analogWrite(LM1, 0);
    digitalWrite(LM2, LOW);
    analogWrite(RM1, 0);
    digitalWrite(RM2, LOW);
}
