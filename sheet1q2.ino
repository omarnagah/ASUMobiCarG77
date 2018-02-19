char led[4]={4,5,6,7};
int T=1000;
void setup() {
  // put your setup code here, to run once:
pinMode (led[0],OUTPUT);
pinMode (led[1],OUTPUT);
pinMode (led[2],OUTPUT);
pinMode (led[3],OUTPUT);
}
void loop() {
  for(char i=0;i<4;i++){
    digitalWrite (led[i],HIGH);
    delay (T);
    digitalWrite (led[i],LOW);    
    }
    for(char j=2;j>=1;j--){
      
      digitalWrite (led[j],HIGH);
    delay (T);
    digitalWrite (led[j],LOW);
    }

}
