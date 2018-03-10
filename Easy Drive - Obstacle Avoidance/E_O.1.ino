int in1 = 10;
int in2 = 9;
int in3 = 5;
int in4 = 6;
int trigPin1 = 11;    //Trig - green Jumper Left sensor
int echoPin1 = 12;    //Echo - yellow Jumper
int x = 0;

long duration1, cm1;


void BackRight()
{
  analogWrite(in1, 192);
  analogWrite(in2, 192);
  analogWrite(in3, 192);
  digitalWrite(in4, LOW);
}
void BackLeft()
{
  digitalWrite(in1, LOW);
  analogWrite(in2, 192);
  analogWrite(in3, 192);
  analogWrite(in4, 192);
}
void nitrous()
{
  analogWrite(in1, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, HIGH);

}
void forward()
{ analogWrite(in3, LOW);
  analogWrite(in4, 192);
  analogWrite(in1, 192);
  digitalWrite(in2, LOW);
}
void backward ()
{ digitalWrite(in4, LOW);
  analogWrite(in3, 192);
  analogWrite(in2, 192);
  digitalWrite(in1, LOW);
}

void forwardRight()
{ digitalWrite(in3, LOW);
  analogWrite(in1, 190);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
}

void forwardLeft() {
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  analogWrite(in4, 190);
  delay(10);
}
void Right () {
  analogWrite(in3, 192);
  digitalWrite(in4, LOW);
  digitalWrite(in2, LOW);
  analogWrite(in1, 192);
}
void Left () {
  digitalWrite(in1, LOW);
  analogWrite(in2, 192);
  digitalWrite(in3, LOW);
  analogWrite(in4, 192);
}
void Stop () {
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void Brake () {
  digitalWrite(in2, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
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


  Serial.print(cm1);
  Serial.print("cm ");
  Serial.println();

  if ( cm1 < 30)
  { x = 1;
  }
}
void avoid()
{ backward ();
  delay(200);
  
  Left();
  delay(500);}

void setup()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  { x=0;
  obtest ();
    char data = Serial.read(); // reading the data received from the bluetooth module
    if (data == 'F') {
      if (x == 1)
        avoid();
      else
        forward();
      delay(100);
    }
    else if (data == 'B')
    {
      backward ();
      delay(100);
    }
    else if (data == 'Y') //forward right
    { if (x == 1)
        avoid();
      else
        forwardRight();
      delay(100);
    }
    else if (data == 'M') //forward left
    { if (x == 1)
        avoid();
      else
        forwardLeft();
      delay(100);
    }
    else if ( data == 'R') //mfrod ylef f makano lel ymen
    {
      Right();
      delay(100);
    }
    else if (data == 'L') //mfrod ylef f makano lel shemal
    {
      Left ();
      delay(100);
    }
    else if (data == 'H')
    {
      BackRight();
      delay(100);
    }
    else if (data == 'G')
    {
      BackLeft();
      delay(100);
    }
    else if (data == 'S')
    {
      Stop ();
      delay(100);
    }
    else if (data == 'N')
    {
      nitrous();
    }

    else
    {
      Brake ();
      delay(100);
    }
    Serial.println(data);
  }
}
