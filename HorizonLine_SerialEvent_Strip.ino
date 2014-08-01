
#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

// Number of RGB LEDs in strand:
int nLEDs = 160;
const int numLEDS = 160;
// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);


const int length = 480;
char charArray[length];
boolean arrayComplete = false;
boolean bufferCleared = false;
boolean printedOnce = false;
int arrayIndex = 0;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  for(int i = 0; i<sizeof(charArray); i++){
    charArray[i] = 0;
  }
  strip.begin();
  for(int t = 0; t< strip.numPixels(); t++){
    strip.setPixelColor(t, 0);
    
  }
  strip.show();
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
   if(bufferCleared){  //check to make sure there are no bytes sitting in the input buffer
      arrayComplete = false;
      arrayIndex = 0;
     // Serial.println("arrayIndex is false");
      bufferCleared = false;
      printedOnce = false;
      Serial.write('A');
    }
  // print the string when a newline arrives:
  // Serial.println(arrayIndex);
  if (arrayComplete && printedOnce == false) {  //only execute if array has been filled and only execute once
    //for(int i = 0; i<sizeof(charArray); i++){
      //Serial.write(charArray[i]);
      printedOnce = true;
      illuminate();
    //}
  
    
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
//int totalBytes = Serial.available();
//Serial.println(totalBytes, DEC);
  while (Serial.available()) {
    if(arrayComplete==false ){
      // get the new byte:
      charArray[arrayIndex] = (char)Serial.read(); 
      // add it to the inputString:
      arrayIndex++;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (arrayIndex == sizeof(charArray)-1 ) {
        arrayComplete = true;
      // Serial.println("arrayComplete is true");
       
      }
    
    } else{ //if for some reason we get more bytes than expected dispose of unwanted bytes
      while(Serial.available()){
        char c = (char)Serial.read();
        //Serial.println(c);
      }
        bufferCleared = true;
        arrayIndex = 0;
    }
   
   // Serial.println(arrayIndex, DEC);
  }
  
  
 
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write('A');   // send a capital A
    delay(300);
  }
}

void illuminate(){
  int s = 0;
  for(int t = 0; t< strip.numPixels(); t++){
    char red, blue, green;
    red = charArray[s];
    green = charArray[s+1];
    blue = charArray[s+2];
    
    strip.setPixelColor(t, strip.Color(red, green, blue));
    s+=3;
  }
  strip.show();
    
}

