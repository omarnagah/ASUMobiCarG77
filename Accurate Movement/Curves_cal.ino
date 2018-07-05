#define LM1 6 // left motor M1a
#define LM2 5 // left motor M2a
#define RM1 10 // right motor M2a
#define RM2 9 // right motor M2b
const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference

int pwm,pwml,pwmh=200,rpm1,rpm2,counter=0,n=0,m=350,k=0,calc_n=0,prev_n=0; // RPM value
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
  pwm=pwmh;
  for(int i=0;i<2;i++){
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
    while(counter<13){
    currentstate = digitalRead(dataIN); // Read IR sensor state
    if( prevstate != currentstate) // If there is change in input
    {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         counter++;
         if(counter == 13){
         duration = ( millis() - prevmillis ); // Time difference between revolution in microsecond
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
    calc_n++;
    Serial.println(ratio);
    Serial.print(rpm1);
    Serial.print("---");
    Serial.println(rpm2);
  }
  if(i == 0){
    pwml=pwm;
    rv=3.1;
    pwm=pwmh/rv;}
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
 for(;k<4;k++){
  analogWrite(LM1, pwml);
  digitalWrite(LM2, LOW);
  analogWrite(RM1, pwmh);
  digitalWrite(RM2, LOW);
  while(n<m){
    currentstate = digitalRead(2);
    if( prevstate != currentstate)
      {if( currentstate == HIGH ){
           n++;}}
    prevstate = currentstate;}
  if(k == 0 || k == 3){
    analogWrite(LM1, 0);
    digitalWrite(LM2, LOW);
    analogWrite(RM1, 0);
    digitalWrite(RM2, LOW);
    delay(10000);}
    n=0;
  if(k == 0){
    m=m/4;
    pwml=pwm;}
  else if(k == 1){
    m=m*2+1;
    pwml=pwmh;
    pwmh=pwm;}
  else if(k == 2){
    m=m/2;
    pwmh=pwml;
    pwml=pwm;}
 }
}
  // SERIAL Display
  //if( ( millis()-prevmillis ) >= 1000 )
   // {
      // Serial.println(rpm);  
   // }
