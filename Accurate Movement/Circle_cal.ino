#define LM1 6 // left motor M1a
#define LM2 5 // left motor M2a
#define RM1 10 // right motor M2a
#define RM2 9 // right motor M2b
const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference

int pwm=200,rpm1,rpm2,counter=0,n=0,calc_n=0,prev_n=0; // RPM value
float ratio=2,rv=1.55;

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan
boolean done=0,start_cal=0,pos=0,neg=0;

void setup()
{
  Serial.begin(9600);
  pinMode(dataIN,INPUT);       
  prevmillis = 0;
  prevstate = LOW; 

  
  while(abs(ratio-rv)>=0.03)
  {
    analogWrite(LM1, pwm);
    digitalWrite(LM2, LOW);
    analogWrite(RM2, pwm);
    digitalWrite(RM1, LOW);
    Serial.println(pwm);
    if(done == 1){
      start_cal=1;
      done=0;}
      
      
                                                                        //RPM CALCULATION
    while(counter<13){
    currentstate = digitalRead(dataIN); // Read IR sensor state
    if( prevstate != currentstate) // If there is change in input
    {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         counter++;
         if(counter == 13){
         duration = ( millis() - prevmillis ); // Time difference between revolution in millisecond
         rpm2 = (60000/duration); // rpm = (1/ time millis)*1000*60;
         if(calc_n==5)
         {
          rpm1=rpm2;
          done=1;
          prev_n=5;
         }
         prevmillis = millis();} // store time for next revolution calculation
       }
    }
    prevstate = currentstate;} // store this scan (prev scan) data for next scan
    counter=0;

    
    if(done == 1){
      pwm=pwm/rv;}
    if((int)(rpm1/rv)-rpm2>0 && start_cal == 1 && (calc_n-prev_n) == 5){
      pos=1;
      if(neg == 1){
        pwm=pwm+2;}
      else{
        pwm=pwm+7;}
      neg=0;
      prev_n=calc_n;
    }
    else if((int)(rpm1/rv)-rpm2<0 && start_cal == 1 && (calc_n-prev_n) == 5){
      neg=1;
      if(pos == 1){
        pwm=pwm-2;}
      else{
        pwm=pwm-7;}
      pos=0;
      prev_n=calc_n;
    }
    ratio = (float)rpm1/rpm2;
    Serial.println(calc_n);
    calc_n++;
    Serial.println(ratio);
    Serial.print(rpm1);
    Serial.print("---");
    Serial.println(rpm2);
  }
  analogWrite(LM1, 0);
  digitalWrite(LM2, LOW);
  analogWrite(RM1, 0);
  digitalWrite(RM2, LOW);
  Serial.println(rpm1);
  Serial.println(rpm2);
  delay(5000);
  prevstate = LOW; 
}

void loop()
{
 currentstate = digitalRead(2);
 if( prevstate != currentstate)
   {if( currentstate == HIGH ){
       n++;}}
 prevstate = currentstate;
 
if(n<350)
  {analogWrite(LM1, pwm);
   digitalWrite(LM2, LOW);
   analogWrite(RM1, 200);
   digitalWrite(RM2, LOW);}
   
 else
  {analogWrite(LM1, 0);
   digitalWrite(LM2, LOW);
   analogWrite(RM1, 0);
   digitalWrite(RM2, LOW);}
   
  // SERIAL Display
  //if( ( millis()-prevmillis ) >= 1000 )
   // {
      // Serial.println(rpm);  
   // }

}

