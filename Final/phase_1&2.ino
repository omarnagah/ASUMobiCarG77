

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
const float KP = 4;
const float KD = 12;
const float KI = 0.01;
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
int n = 0;

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

  if ( cm1 < 30)
  { n = 1;
  }
  else {
    n = 0;
  }
}
void avoid()
{ backward ();
  delay(200);
  while (n == 1)
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
  { //obtest ();
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
    if (data == 'F') {
      if (n == 1)
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
    { if (n == 1)
        avoid();
      else
        forwardRight();
      delay(10);
    }
    else if (data == 'G') //forward left
    { if (n == 1)
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

    if (data == 'V')
    {
      /// phase3
    }
    Serial.println(data);
  }
}


