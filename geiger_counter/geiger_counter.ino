// Goes into a geiger counter prop, beeps and flashes
// when an object is between ~6-18 inches from sensor.
// Uses a control voltage from an LV‑MaxSonar‑EZ1 

#define ledPin 8
#define sensorPin A0
#define buzzerPin 11  // piezzo buzzer
#define tempo 20 // affects the beep/flash 
#define pitch 1600  // audible freq of beep

int range = 6;
int x = 0;
unsigned long time_of_last_beep = millis();
int sleep_time = 50;  // sensor delay to save power

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

int readSensor() {
   range = constrain( analogRead(sensorPin), 2, 300);
   range = map( range , 2, 300, 2, 30);
   return range;
}

void loop() {
  if (millis() - time_of_last_beep > 10000){
    sleep_time = 1500;}   // power saving mode
  else { 
    sleep_time = 50;}  //active state
  delay(sleep_time); // Save power
  range = readSensor();
  
  while( range == 3  ){
    x = random(3);
    delay(tempo);
    if ( x == 1 ){
      time_of_last_beep = millis();
      tone(buzzerPin, pitch);
      digitalWrite(ledPin, HIGH);
      delay(tempo);
      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);
      }
    range = readSensor();
    }
}
