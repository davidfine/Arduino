/* 
2piR for Arduino Mega. 
David Fine
Version 10120102 - untested

Pin guide:
analog in on pins 0-15 to sensor CV.
digital out on 22 - 37
optional safety button to ground, pin 21
optional re-arm from safe button to ground, pin 20
optional arming buzzer on pin 38
*/

int sensorThreshold = 300;  // adjust sensitivity
boolean lastState[15];  // 0 for Meh, 1 for ON FIRE!
unsigned long time;
unsigned long flameCounter[15];  // the time millis() when each effect last triggered
int minimumFlame = 300; //milliseconds
int buzzerPin = 39; // audible warning
int armed = 1; // Start in armed mode

void setup(){
  attachInterrupt(2, safeMode, FALLING);  // disarm when pin grounded
  attachInterrupt(3, armMode, FALLING);  // re-arm , break out of safeMode loop
  pinMode(20, INPUT_PULLUP); // interrupt 3 
  pinMode(21,INPUT_PULLUP);  // interrupt 2
  pinMode(38, OUTPUT);  // arming buzzer
  for (int i=0; i <= 15; i++){
    pinMode(i, OUTPUT);
    flameCounter[i] = 0;
    lastState[i] = 0;
  }
}

int main(){
  while(armed) {
  startFire(); // if lastState changed to high
  stopFire();  // if lastState is 0 and flameCounter expired
  }
}
  
 
void startFire() {
  for(int i=0; i<=15; i++) {
    int currentState = (analogRead(i) > sensorThreshold);
     if ( currentState && (lastState[i] != currentState)) {  //switched state to high
       digitalWrite((i + 22), HIGH);
       lastState[i] = 1;
       flameCounter[i] = (millis() + minimumFlame);
     }
      else {   
         lastState[i] = 0;
       }
    }
 }

void stopFire() {
  time = millis();
  for(int i=0; i<=15; i++) {
    if !(lastState[i]) {
    if ( time > flameCounter[i] ) 
      digitalWrite((i + 22), LOW);  
    } 
  }
}

void safeMode() {   // shut down all pins and sleep loop
  armed = 0;
  for (int i=0; i <= 15; i++) {
    digitalWrite((i+22) , LOW);
    lastState[i] = 0; 
  }

}  

void armMode() {
  digitalWrite(buzzerPin, HIGH);
  delay(3000);
  digitalWrite(buzzerPin, LOW);
  armed = 1;
}


