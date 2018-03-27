#define BTread Serial.read()
#define CalSBY 'W'
#define RaceSBY 'U'
#define CalKey 'w'
#define RaceKey 'u'
#define SBY 'S'

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) {
    if (BTread == CalSBY) {
      while (BTread == SBY) {
        //calibration stand-by
      }
      if (BTread == CalKey) {
        //calibrate
      }
    }
    else if (BTread == RaceSBY) {
      while (BTread == SBY) {
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
