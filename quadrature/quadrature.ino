//PIN's definition
#define encoder0PinA  2
#define encoder0PinB  3
#define thudSensorPin 21
#define dialPosPin 10
volatile unsigned int encoder0Pos = 0;
volatile boolean PastA = 0;
volatile boolean PastB = 0;
volatile boolean blockThud = 0;
volatile unsigned int thudMillis = 0;  // Time most recent thud interrupt

void setup() {
  Serial.begin(57600);
  //turn on pullup resistor
  //digitalWrite(encoder0PinA, HIGH); //ONLY FOR SOME ENCODER(MAGNETIC)!!!! 
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT); 
  pinMode(thudSensorPin, INPUT);
  pinMode(dialPosPin, OUTPUT);
  PastA = (boolean)digitalRead(encoder0PinA); //initial value of channel A;
  PastB = (boolean)digitalRead(encoder0PinB); //and channel B  
  //To speed up even more, you may define manually the ISRs
  // encoder A channel on interrupt 0 (arduino's pin 2)
  attachInterrupt(0, doEncoderA, RISING);
  // encoder B channel pin on interrupt 1 (arduino's pin 3)
  attachInterrupt(1, doEncoderB, CHANGE); 
  // Do whack is triggered by a vibration sensor and can trigger once per 500ms
  attachInterrupt(2, doThud, RISING);
}


void loop(){  
  if(blockThud){
    if((millis() - thudMillis) > 500)
    blockThud = 0;
  }
  setDialPos();
  if(millis() & 1000000){  //only send over serial every 64ms
    encoder0Pos &= 1111111111;  // enforce upper limit 1024
    Serial.write(encoder0Pos);
  }
}

void SerialEvent(){
  // Handle control messages from computer. May not be used.
}
  
  
void setDialPos(){
  // send PWM to servo on dialPosPin based on encoder0Pos
}


// Interrupt Definitions!
void doThud(){
  if(!blockThud){
    Serial.write("p");
    blockThud = 1;
    thudMillis = millis();
  }
}

void doEncoderA(){
     PastB ? encoder0Pos--:  encoder0Pos++;
}

void doEncoderB(){
     PastB = !PastB; 
}
