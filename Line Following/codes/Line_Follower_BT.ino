#define BTread Serial.read()
#define CalSBY 'W'  //'W' == front lights on
#define RaceSBY 'U' //'U' == back lights on
#define CalKey 'w'  //'w' == front lights off
#define RaceKey 'u' //'u' == back lights off
#define SBY 'S'     //'S' == no action

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
