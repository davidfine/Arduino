/*
  *This code turns an arduino mega into an I/O serial board. This version is quick and dirty, and formatted terribly.
  The first thing
  anyone should do is change the variables and loop values to reflect the number of pins actually used.
  This version is hard coded to use pins 2-39 for input and 40-53 for output (reserving 13 for the arduino test LED.)
  Analog reads could be added pretty easily. 
  Don't forget to pull-down to ground each digital input pin through a 10 to 100K ohm resistor, or readings will be shaky. 
  The following cod sends single byte commands/readings to and from another serial device. 
  The highest two bits of each byte are the command type flags, and the following 6 are payload 
  (an analog value with 64 point resolution or, in the case of digital operations, a pin number 0-63 )
  This protocol supports 64 digital inputs, 64 digital outputs, 2 Analog readings, and 2 PWM outputs (with 64 point resolution).
  10xxxxxx = digital pin set LOW  (do a bitwise 'or' 128)
  11xxxxxx = digital pin set HIGH (bitwise 192)
  00xxxxxx = Analog value device 0  (bitwise 0)
  01xxxxxx = Analog value device 1 (bitwise 64)
  So as you see, a limitation of this single byte encoding is you only get 2 analog devices. 
  *The readPins loop sends readings from digital and analog input pins to another device over serial. It only sends values for pins whos state has changed.
  *The setPins loop takes incoming serial bytes and sets pins accordingly
  You'll see a lot of bit operations doing "value & 63" , since 192 is 00111111 and will clear out the highest two bits, leaving just the payload.
  */
  
int digInputs[40]; // array stores values of digital input pins 2-39. Bigger than it needs to be so i don't have to worry about offsets.
byte count=0;  // used in for loops a bunch
byte readPin;  // digital input pin being read
byte incomingSerialByte; //Takes data from serial.read for immediate processing
byte currentPinValue; //reads current input pin for comparison with 
byte messageByte;
byte ADC0=0; //Stores value of ADC to send to serial, after being mapped to 6 bit.
byte ADC1=0;
byte decodedPin; //stores lower 6 bits, or "payload" of incoming message byte
byte decodedFlag; //Stores higest 2 bits, or flag bits, of incoming message byte


void setup()                    // run once, when the sketch starts
{
  for(count=40;count<=53;count++){
  pinMode(count, OUTPUT);      // sets pins 40 thru 53 as output
  }
  for(count=2;count<=39;count++){ 
  pinMode(count, INPUT);  // sets pins 2 thru 39 as input
  }
  pinMode(13, OUTPUT); // Set the testing LED pin, comment out if you want to use pin 13 for other stuff.
  for(count=0;count<=36;count++){  // initialize array with 0's
    digInputs[count]=0;
  }
}

void loop()                     // run over and over again
{
  Serial.begin(57600);
 // Serial.flush(); //why not?

//This loop reads digital inputs and sends report bytes to serial out
  for(readPin=2;readPin<=39;readPin++)  
    {  
      if(readPin!=13){      //pin 13 is the test LED on the board.

     currentPinValue = digitalRead(readPin);
      if(currentPinValue != digInputs[readPin])//if value has changed from previous reading
      { 
        digInputs[readPin] = currentPinValue;  //update array to track new current value
        switch(currentPinValue){
          case 1:
          messageByte=(192 | readPin);  
          Serial.print(messageByte, BYTE); 
          // The byte will be 11.xxxxxx where last 6 contain number of pin. 11 means "digital high"
          case 0:
          messageByte=(128 | readPin);  // The byte will be 10.xxxxxx , 10 means "digital low"
         Serial.print(messageByte, BYTE); 
          default: 
          break;
        }
      }
       // Serial.print(messageByte, BYTE);

      }
    }
    
    // Move on to reading input from another serial device
   
    for(count=1;count<=8;count++) //This loop reads up to 8 bytes from serial buffer and sets corresponding digital output pins on arduino
    {
    if(Serial.available()==0)  
    {
      break;  //Skip if no serial data has been received
    }
    incomingSerialByte=Serial.read();
    // delay(5); // may not be needed
    decodedFlag=(incomingSerialByte&192);
    decodedPin=(incomingSerialByte&63); //Strips first 2 flag bits, leavig just payload "pin number"

    switch(decodedFlag) //the &192 strips all but first 2 bits of the byte.
    {
      case 128:  //the flag bits were 01, signals to set pin low
      digitalWrite(decodedPin, LOW);
      break;
      
      case 192: //flag bits were 11, so set pin high
      digitalWrite(decodedPin, HIGH);
      break;

    }
    }
}
    
 
//case 64:
/* If you wanted to read in analog values from another device, this is where you'd do it.
You'd have to do an &63 to get just the lower 6 bits.
For PWM output, you'd then map the 0-63 value
to the 0-1023 range. And set a pin to write to, of course.
*/
//case 0:
/*This would be available for a second analog value.
It could be a second PWM. Or, if you want more than 2 PWM outputs,
you could get sneaky and use this value to set a variable which the previous case uses
to determine which PWM to address.
}

      


    






    

