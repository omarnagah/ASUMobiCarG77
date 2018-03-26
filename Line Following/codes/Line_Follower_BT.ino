#define BTread Serial.read()
#define CalSBY 'W'
#define RaceSBY 'U'
#define CalKey 'w'
#define RaceKey 'u'

void setup() {
  // put your setup code here, to run once:
  Serial.begin(300);
}

void loop() {

  if (Serial.available()) {
    if (BTread == CalSBY) {
      while (BTread == CalSBY) {
        //calibration stand-by
      }
      if (BTread == CalKey) {
        //calibrate
      }
    }
    else if (BTread == RaceSBY) {
      while (BTread == RaceSBY) {
        //race stand-by
      }
      if (BTread == RaceKey) {
        while ((BTread != RaceSBY) && (BTread != CalSBY)) {   //press cal- or race-standby to stop
          //race
        }
      }
    }
  }
}
