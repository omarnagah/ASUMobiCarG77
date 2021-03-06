

#define BTread Serial.read()
#define CalSBY 'W'    //'W' == front lights on
#define RaceSBY 'U'   //'U' == back lights on
#define CalKey 'w'    //'w' == front lights on
#define RaceKey 'u'   //'u' == back lights on
#define SBY 'S'       //'S' == no action

#define dW digitalWrite
#define dR digitalRead

/* PIN DEFINITION */

#define mot1  10
#define mot2  9
#define mot3  6
#define mot4  5
#define LM1 6 // left motor M1a
#define LM2 5 // left motor M2a
#define RM1 10 // right motor M2a
#define RM2 9 // right motor M2b
#define Sread Serial.read()
#define sby 'S'
#define zeroASCII 48

const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

int pwm,pwml,pwmh=255,pwmc,rpm1,rpm2,counter=0,n=0,m=0,ref=100,calc_n=0,prev_n=0; // RPM value
float ratio=2,rv=1.7,dist=0;
boolean done=0,start_cal=0,pos=0,neg=0;

unsigned char slider = 0;

/* GLOBAL CONSTANTS */

#define WHITE  1
#define BLACK   0

#define COLOR   BLACK         //Line color

#define N_SENS        5       //Number of sensors
#define R_SENS        1000    //Sensor readings are mapped to this range
#define WEIGHT_UNIT   1000    //Unit for weighted average
#define RIGHT_DIR     1
#define LEFT_DIR      0
#define PRESS         0
#define CAL_SPEED     350     //Sensor calibration speed
#define CAL_TIME      3000     //Calibration time

//=========================================================================================/
#define P_LINE_MIN   65     //Minimum brightness percentage to consider part of the line

const float SPEED = 800;
const float KP = .4;
const float KD = 12;
const float KI = 0.04;
//=============================================================================================/

/* GLOBAL VARIABLES */

unsigned long ms = 0;
const int SENSOR[N_SENS] = { A0, A1, A2, A3, A4 };     //Arduino pins
int sens_max[N_SENS];          //Maximum value each sensor measures (calibrated)
int sens_min[N_SENS];          //Minimum value each sensor measures (calibrated)
float line_pos = 0;
float last_line_pos = 0;



int MOT1 = 6;
int MOT2 = 5;
int MOT3 = 9;
int MOT4 = 10;
int trigPin1 = 11;    //Trig - green Jumper Left sensor
int echoPin1 = 12;    //Echo - yellow Jumper
int noga = 0;

long duration1, cm1;


void BackRight()
{
  analogWrite(MOT1, 192);
  analogWrite(MOT2, 192);
  analogWrite(MOT3, 192);
  digitalWrite(MOT4, LOW);
}
void BackLeft()
{
  digitalWrite(MOT1, LOW);
  analogWrite(MOT2, 192);
  analogWrite(MOT3, 192);
  analogWrite(MOT4, 192);
}

void forward()
{ analogWrite(MOT3, LOW);
  analogWrite(MOT4, 192);
  analogWrite(MOT1, 192);
  digitalWrite(MOT2, LOW);
}
void backward ()
{ digitalWrite(MOT4, LOW);
  analogWrite(MOT3, 192);
  analogWrite(MOT2, 192);
  digitalWrite(MOT1, LOW);
}

void forwardRight()
{ digitalWrite(MOT3, LOW);
  digitalWrite(MOT1, HIGH);
  digitalWrite(MOT2, LOW);
  digitalWrite(MOT4, LOW);
}

void forwardLeft() {
  digitalWrite(MOT3, LOW);
  digitalWrite(MOT2, LOW);
  digitalWrite(MOT1, LOW);
  analogWrite(MOT4, 190);

}
void Right () {
  analogWrite(MOT3, 192);
  digitalWrite(MOT4, LOW);
  digitalWrite(MOT2, LOW);
  analogWrite(MOT1, 192);
}
void Left () {
  digitalWrite(MOT1, LOW);
  analogWrite(MOT2, 192);
  digitalWrite(MOT3, LOW);
  analogWrite(MOT4, 192);
}
void Stop () {
  digitalWrite(MOT2, LOW);
  digitalWrite(MOT1, LOW);
  digitalWrite(MOT3, LOW);
  digitalWrite(MOT4, LOW);
}
void Brake () {
  digitalWrite(MOT2, HIGH);
  digitalWrite(MOT1, HIGH);
  digitalWrite(MOT3, HIGH);
  digitalWrite(MOT4, HIGH);
}

void obtest ()
{
  digitalWrite(trigPin1, LOW);

  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);

  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin1, INPUT);

  duration1 = pulseIn(echoPin1, HIGH);


  // convert the time into a distance
  cm1 = (duration1 / 2) / 29.1;


  /* Serial.print(cm1);
    Serial.print("cm ");
    Serial.println();*/

  if ( cm1 < 40)
  { noga = 1;
  }
  else {
    noga = 0;
  }
}
void avoid()
{ backward ();
  delay(200);
  while (noga == 1)
  { obtest();  Left();
    delay(50);
  }

}

float get_line_pos(int color, int last_dir) {
  float line = 0;
  int line_detected = 0;
  float sens_scaled[N_SENS];
  float avg_num = 0;          //Average numerator
  float avg_den = 0;          //Average denominator
  for (int x = 0; x < N_SENS; x++) {
    //Scale from 0 to R_SENS
    sens_scaled[x] = analogRead(SENSOR[x]) - sens_min[x];
    sens_scaled[x] *= R_SENS;
    sens_scaled[x] /= (sens_max[x] - sens_min[x]);

    if (color == BLACK) {
      sens_scaled[x] = R_SENS - sens_scaled[x];     //Reverse scale to go from R_SENS to 0
    }
    if (sens_scaled[x] >= (float) R_SENS * ((float)P_LINE_MIN / 100.0)) { //At least one sensor has to detect a line
      line_detected = 1;
    }

    avg_num += sens_scaled[x] * x * WEIGHT_UNIT;
    avg_den += sens_scaled[x];
  }
  if (line_detected == 1) {
    line = avg_num / avg_den;                           //Weighted average
    line = line - (WEIGHT_UNIT * (N_SENS - 1) / 2);     //Change scale from 0 _ 4000 to -2000 _ 2000
  } else {
    line = WEIGHT_UNIT * (N_SENS - 1) * last_dir;       //Use last direction to calculate error as the maximum value
    line = line - (WEIGHT_UNIT * (N_SENS - 1) / 2);     //Change scale
  }

  return line;
}

float get_PID_correction(float line, float last_line, float kp, float kd, float ki) {
  float proportional = line;
  float derivative = line - last_line;
  float integral = line + last_line;
  float correction = (kp * proportional + kd * derivative + ki * integral);

  return correction;
}

void motorSpeed(int m1, int m2) {           //From -1000 to 1000
  int pwm1 = map(abs(m1), 0, 1000, 0, 255);
  int pwm2 = map(abs(m2), 0, 1000, 0, 255);
  pwm1 = (m1 > 0) ? 255 - pwm1 : pwm1;
  pwm2 = (m2 <= 0) ? pwm2 : 255 - pwm2;
  analogWrite(mot2, pwm1);
  analogWrite(mot4, pwm2);
  digitalWrite(mot1, (m1 > 0) ? HIGH : LOW);
  digitalWrite(mot3, (m2 <= 0) ? LOW : HIGH);
}

void InitializeGPIO() {
  pinMode(MOT1, OUTPUT);
  pinMode(MOT2, OUTPUT);
  pinMode(MOT3, OUTPUT);
  pinMode(MOT4, OUTPUT);

  for (int x = 0; x <= N_SENS; x++) {
    pinMode(SENSOR[x], INPUT);
  }
}

void race(void) {
  last_line_pos = line_pos;
  line_pos = get_line_pos(COLOR, (last_line_pos > 0));

  float PID_correction = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);
  float max_correction = SPEED;                   //Can be changed to a lower value in order to limit the correction, needs to be at most SPEED

  if (PID_correction > 0) {
    PID_correction = (PID_correction > max_correction) ? max_correction : PID_correction;
    motorSpeed((SPEED - PID_correction), SPEED);
    Serial.print("correction > 0\t");
    Serial.println(( PID_correction), DEC);
  } else {
    PID_correction = (PID_correction < -max_correction) ? -max_correction : PID_correction;
    motorSpeed(SPEED, SPEED + PID_correction);
    Serial.print("correction < 0\t");
    Serial.println(( PID_correction), DEC);
  }
}

void calibrate(int cal_time, int cal_speed, int cal_dir) {
  ms = millis();
  while ((ms + cal_time) > millis()) {
    if (cal_dir == RIGHT_DIR)  motorSpeed(cal_speed, -cal_speed);
    if (cal_dir == LEFT_DIR)  motorSpeed(-cal_speed, cal_speed);

    int sens_value[N_SENS];
    for (int x = 0; x < N_SENS; x++) {
      sens_value[x] = analogRead(SENSOR[x]);
      sens_min[x] = (sens_value[x] < sens_min[x]) ? sens_value[x] : sens_min[x];
      sens_max[x] = (sens_value[x] > sens_max[x]) ? sens_value[x] : sens_max[x];
    }
  }
  motorSpeed(0, 0);
}


void setup() {

  Serial.begin(9600);
  InitializeGPIO();

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);



  for (int x = 0; x < N_SENS; x++) {
    sens_max[x] = 0;
    sens_min[x] = 1023;
  }
}


void loop() {
  if (Serial.available() > 0)
  { obtest ();
    char data = Serial.read(); // reading the data received from the bluetooth module
    if (data == 'W') {
      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
      /* for (int x = 0; x < N_SENS; x++) {
         Serial.println(sens_min[x]);
         Serial.println(sens_max[x]);
         Serial.println();
        }*/  //debug
      delay(5000);

      while ( BTread == 'S') {
        race();
        delay(10);
      }
    }
    if (data == 'F') {obtest ();
      if (noga == 1)
        avoid();
      else
        forward();
      delay(10);
    }
    else if (data == 'B')
    {
      backward ();
      delay(10);
    }
    else if (data == 'I') //forward right
    { obtest ();
    if (noga == 1)
        avoid();
      else
        forwardRight();
      delay(10);
    }
    else if (data == 'G') //forward left
    {obtest (); if (noga == 1)
        avoid();
      else
        forwardLeft();
      delay(10);
    }
    else if ( data == 'R') //mfrod ylef f makano lel ymen
    {
      Right();
      delay(10);
    }
    else if (data == 'L') //mfrod ylef f makano lel shemal
    {
      Left ();
      delay(10);
    }
    else if (data == 'J')
    {
      BackRight();
      delay(10);
    }
    else if (data == 'H')
    {
      BackLeft();
      delay(10);
    }
    else if (data == 'S')
    {
      Stop ();
      delay(10);
    }

    else

    {
      Brake ();
      delay(10);
    }
    Serial.println(data);

     if (data == 'X') {
      while (data != 'x') {
        data = Sread;
        Serial.println(data);
        if (data >= zeroASCII && data <= zeroASCII + 9) {
          slider = data - zeroASCII;
             // Serial.println(slider);
        }
        else {
          dist = slider * 10 + 10;
              Serial.println(dist);
          if ( data == 'F') {
            analogWrite(LM1, 150);
            digitalWrite(LM2, LOW);
            analogWrite(RM1, 150);
            digitalWrite(RM2, LOW);
            m=60; ref=100;
          }
          else if ( data == 'R') {
            analogWrite(LM1, 150);
            digitalWrite(LM2, LOW);
            analogWrite(RM2, 150);
            digitalWrite(RM1, LOW);
            m=40; ref=360;
            dist*=3;
         }
          else if ( data == 'L') {
            analogWrite(LM2, 150);
            digitalWrite(LM1, LOW);
            analogWrite(RM1, 150);
            digitalWrite(RM2, LOW);
            m=40; ref=360;
            dist*=3;
          }
          else if ( data == 'B') {
            analogWrite(LM2, 150);
            digitalWrite(LM1, LOW);
            analogWrite(RM2, 150);
            digitalWrite(RM1, LOW);
            m=60; ref=100;
          }
          else if ( data == 'S') {
            analogWrite(LM1, 0);
            digitalWrite(LM2, LOW);
            analogWrite(RM1, 0);
            digitalWrite(RM2, LOW);
            m=0;
          }
          while(n<(int)(m*(dist/ref))){
            //Serial.println(distance);
            currentstate = digitalRead(dataIN);
            if( prevstate != currentstate){
              if( currentstate == HIGH )
                {n++;}}
            prevstate = currentstate;}
            n=0;
        }
        dist=0;
      }
    }
    else if (data == 'V'){
out:  while (data != 'v'){
        data = Sread;
        if(data == 'B'){
          //do calibration
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
            data = Sread;
            if(data == 'v'){goto out;}
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
              pwmc=pwm;
              pwm=pwm/rv;
              rv=2.7;
              prev_n=calc_n;
              /*analogWrite(LM1, 0);
              digitalWrite(LM2, LOW);
              analogWrite(RM1, 0);
              digitalWrite(RM2, LOW);
              delay(1000);*/
            }
           }
          analogWrite(LM1, 0);
          digitalWrite(LM2, LOW);
          analogWrite(RM1, 0);
          digitalWrite(RM2, LOW);
          Serial.println(rpm1);
          Serial.println(rpm2);
          prevstate = LOW; 
        }
        else if (data == 'F') {
          //do demo
          m=50;
          pwml=pwm;
          for(int k=0;k<5;k++){
            if(k != 3){
              analogWrite(LM1, pwml);
              digitalWrite(LM2, LOW);
              analogWrite(RM1, pwmh);
              digitalWrite(RM2, LOW);}
            while(n<m){
              data = Sread;
              if(data == 'v'){goto out;}
              currentstate = digitalRead(2);
              if( prevstate != currentstate)
                {if( currentstate == HIGH ){
                    n++;}}
              prevstate = currentstate;}
            if(k == 2 || k == 3 || k == 4){
              analogWrite(LM1, 0);
              digitalWrite(LM2, LOW);
              analogWrite(RM1, 0);
              digitalWrite(RM2, LOW);
              delay(5000);}
            n=0;
            if(k == 0){
              m=220;
              pwml=pwmh;
              pwmh=pwm;}
            else if(k == 1){
              m=65;
              pwmh=pwml;
              pwml=pwm;}
            else if(k == 2){
              m=30;
              prevstate = LOW;
              for(int j=0;j<=8;j++){ 
              if(j==8)
              {m=30;}
              if(m==60 || m==30){
                analogWrite(LM1, 150);
                digitalWrite(LM2, LOW);
                analogWrite(RM1, 150);
                digitalWrite(RM2, LOW);}
              else if(m==12){
                analogWrite(LM2, 200);
                digitalWrite(LM1, LOW);
                analogWrite(RM1, 200);
                digitalWrite(RM2, LOW);
               }
              while(n<m){
                data = Sread;
                if(data == 'v'){goto out;}
                currentstate = digitalRead(dataIN);
                if( prevstate != currentstate){
                  if( currentstate == HIGH ){
                    n++;}}
                prevstate = currentstate;}
             if(m==60 || m==30)
               {m=12;}
             else if(m==12)
               {m=60;}
             n=0;}
            }
            else if(k == 3){
              pwml=pwmc;
              m=345;}
          }
        }
      }
    }
    Serial.println(data);
  }
}
