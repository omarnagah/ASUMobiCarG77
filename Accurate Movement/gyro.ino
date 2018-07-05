#include <Wire.h>
#define LM1 10 // left motor M1a
#define LM2 9 // left motor M2a
#define RM1 6 // right motor M2a
#define RM2 5 // right motor M2b
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
float rot_dist, quart_dist;

long gyroX, gyroY, gyroZ;
int rotX, rotY, rotZ;
float gyroXangle,prevAngleX;
double timer,elapsedTimeInSeconds;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  recordAccelRegisters();
  recordGyroRegisters();
  prevAngleX=gyroXangle;
  quart_dist = (3.14/2)*9;
  timer = millis();
}


void loop() {
   elapsedTimeInSeconds=((millis()-timer)/1000);
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  if(rot_dist<quart_dist)
  {
    analogWrite(LM1, 80);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    analogWrite(RM2, 80);
  }

  else if(rot_dist>=quart_dist)
  {
    analogWrite(LM1, 70);
    digitalWrite(LM2, LOW);
    analogWrite(RM1, 70);
    digitalWrite(RM2, LOW);
  }
  timer = millis();
  delay(100);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
  //if(rotX<1&&rotX>-1)
  //{rotX=0;}
  gyroXangle = rotX * elapsedTimeInSeconds;
  rot_dist += 9*(gyroXangle/180)*3.14;
  
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" angle=");
  Serial.println(gyroXangle);
//  Serial.print(" Accel (g)");
//  Serial.print(" X=");
//  Serial.print(gForceX);
//  Serial.print(" Y=");
//  Serial.print(gForceY);
//  Serial.print(" Z=");
//  Serial.println(gForceZ);
}

