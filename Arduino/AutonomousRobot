#include <VirtualWire.h>

const int enablePin1 = 23;
const int enablePin2 = 25;
const int controlPin1 = 22;
const int controlPin2 = 24;
const int controlPin3 = 26;
const int controlPin4 = 27;
const int echoPin1 = 34;
const int trigPin1 = 35;
const int echoPin2 = 30;
const int trigPin2 = 31;
const int echoPin3 = 32;
const int trigPin3 = 33;
const int trigPin4 = 39;
const int echoPin4 = 38;
int duration1, distance1, duration2, distance2, duration3, distance3, duration4, distance4;
boolean reverse = false;
char nums[5];
boolean control = true;

void setup(){
  Serial.begin(9600);
  
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(controlPin3, OUTPUT);
  pinMode(controlPin4, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin4, OUTPUT);
  
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);
  
  vw_setup(2000);
  vw_set_rx_pin(43);
  vw_rx_start();
}

void loop(){
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[buflen];
  
  if(vw_get_message(buf, &buflen)){
    for(int i = 0; i < buflen; i++){
      nums[i] = buf[i];
    }
  }
  if(nums[4] == 'c' || nums[4] != 'a'){
    if(nums[0] == 'f' && nums[1] != 'r'){
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
    }
    else if(nums[1] == 'r' && nums[0] != 'f'){
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2, HIGH);
    }
    else{
      digitalWrite(enablePin1, LOW);
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2, LOW);
    }
    if(nums[2] == 'l' && nums[3] != 'r'){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, HIGH);
      digitalWrite(controlPin4, LOW);
    }
    else if(nums[3] == 'r' && nums[2] != 'l'){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, HIGH);
    }
    else{
      digitalWrite(enablePin2, LOW);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, LOW);
    }
  }
  else if(nums[4] == 'a'){
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;
  
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = (duration3 / 2) / 29.1;
  
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin4, LOW);
  duration4 = pulseIn(echoPin4, HIGH);
  distance4 = (duration4 / 2) / 29.1;
  
  if(reverse == false){
    digitalWrite(enablePin2, LOW);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, LOW);
    digitalWrite(enablePin1, HIGH);
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  }
  
  if(reverse == true){
    digitalWrite(enablePin2, LOW);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, LOW);
    digitalWrite(enablePin1, HIGH);
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }
    
  if(distance1 <= 15 && distance1 >= 1){
    if(distance2 > distance3 && distance2 > distance4){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, HIGH);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2, HIGH);
      delay(750);
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, HIGH);
      digitalWrite(controlPin4, LOW);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
      delay(750);
      digitalWrite(enablePin2, LOW);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, LOW);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
    }
    else if(distance2 < distance3 && distance3 > distance4){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, HIGH);
      digitalWrite(controlPin4, LOW);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2, HIGH);
      delay(750);
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, HIGH);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
      delay(750);
      digitalWrite(enablePin2, LOW);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, LOW);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
    }
    else if(distance4 >= distance3 && distance4 >= distance2){
      reverse = true;
    }
  }
  else if(distance4 <= 15 && distance4 >= 1){
    if(distance1 >= distance2 && distance1 >= distance3){
      reverse = false;
    }
    else if(distance2 > distance3 && distance2 > distance1){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, HIGH);
      digitalWrite(controlPin4, LOW);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
      delay(750);
      reverse = false;
    }
    else if(distance3 > distance2 && distance3 > distance1){
      digitalWrite(enablePin2, HIGH);
      digitalWrite(controlPin3, LOW);
      digitalWrite(controlPin4, HIGH);
      digitalWrite(enablePin1, HIGH);
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
      delay(750);
      reverse = false;
    }
  }
  else if(distance2 <= 10 && distance2 >= 1 && reverse == false){
    digitalWrite(enablePin2, HIGH);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, HIGH);
  }
  else if(distance3 <= 10 && distance3 >= 1 && reverse == false){
    digitalWrite(enablePin2, HIGH);
    digitalWrite(controlPin3, HIGH);
    digitalWrite(controlPin4, LOW);
  }
  else if(distance2 <= 10 && distance2 >= 1 && reverse == true){
    digitalWrite(enablePin2, HIGH);
    digitalWrite(controlPin3, HIGH);
    digitalWrite(controlPin4, LOW);
  }
  else if(distance3 <= 10 && distance3 >= 1 && reverse == true){
    digitalWrite(enablePin2, HIGH);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, HIGH);
  }
  }
  
  for(int j = 0; j < 5; j++){
    Serial.print(nums[j]);
    Serial.print(" ");
  }
  Serial.println("");
}
