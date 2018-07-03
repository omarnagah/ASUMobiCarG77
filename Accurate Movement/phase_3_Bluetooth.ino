#define Sread Serial.read()
#define sby 'S'
#define zeroASCII 48

unsigned char slider = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char data = Sread;
    if (data == 'O') {
      while (data != 'o') {
        if (data >= zeroASCII && data <= zeroASCII + 9) {
          slider = data - zeroASCII;
        }
        else {
          unsigned char distance = slider * 10 + 10;
          if ( data == 'F') {

          }
          if ( data == 'R') {

          }
          if ( data == 'L') {

          }
          if ( data == 'B') {

          }
        }
        data = Sread;
      }
    }
    else if (data == 'P') {
      //do demo
    }
  }
}
