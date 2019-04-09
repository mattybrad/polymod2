#include <SPI.h>

// this sketch is for optimizing the speed of the future polymod code

// High when a value is ready to be read
volatile int readFlag;

// Value to store analog result
volatile int analogVal;

const int addressClock = 8;
const int addressLatch = 9;
const int addressData = 10;
const int addressE[] = {2,3,4};
const int addressF[] = {5,6,7};
const int readConnections = A2;

void setup() {
  Serial.begin(115200);
  //pinMode(addressClock,OUTPUT);
  //pinMode(addressLatch,OUTPUT);
  //pinMode(addressData,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(13,OUTPUT);
  for(int i=0;i<3;i++) {
    pinMode(addressE[i], OUTPUT);
    pinMode(addressF[i], OUTPUT);
  }
  pinMode(readConnections, INPUT_PULLUP);
  SPI.begin();

  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= B11011111;
  
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= B01000000;
  
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input
  ADMUX &= B11110000;
  
  // Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
  // Do not set above 15! You will overrun other parts of ADMUX. A full
  // list of possible inputs is available in Table 24-4 of the ATMega328
  // datasheet
  ADMUX |= 0;
  // ADMUX |= B00001000; // Binary equivalent
  
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  ADCSRA |= B10000000;
  
  // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
  ADCSRA |= B00100000;
  
  // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  ADCSRB &= B11111000;
  
  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= B00000111;
  
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= B00001000;
  
  // Enable global interrupts
  // AVR macro included in <avr/interrupts.h>, which the Arduino IDE
  // supplies by default.
  sei();
  
  // Kick off the first ADC
  readFlag = 0;
  // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
  ADCSRA |=B01000000;
}

unsigned long lastStart = 0;
unsigned long innerStart = 0;
unsigned long innerEnd = 0;
int numGroups = 2;
//int analogValues[8*8*8];
byte cyclesSinceRead[8*8*8];

void loop() {
  Serial.println("START");
  bool thingFailed = false;
  lastStart = millis();
  int shiftData = 0; // 2-byte value to send to shift register
  for(byte a=0;a<numGroups;a++) {
    // set multiplexer to route connection test voltage to group A

    for(byte b=0;b<8;b++) {
      // if more than one module group...
      // send A/B address byte to shift register (via SPI for speed)
      
      // set multiplexer to route connection test voltage to group A, module B
      
      for(byte c=0;c<8;c++) {
        // set multiplexeter to route connection test voltage to group A, module B, socket C

        if(c==0) innerStart = millis();
        
        for(byte d=0;d<numGroups;d++) {
          // if more than one module group...
          // set multiplexer to route ID number data from group D
          // set multiplexer to route connection readings from group D
          // set multiplexer to route analog readings from group D
          // set multiplexer to route shift register latch to group D (for shift out, not shift in)
          // set multiplexer to route auxiliary data from group D (either from shift register or multiplexer)
          
          shiftData = (d<<9)+(c<<6)+(b<<3)+a;
          //digitalWrite(addressLatch,LOW);
          //shiftOut(addressData,addressClock,MSBFIRST,(shiftData>>8));
          //shiftOut(addressData,addressClock,MSBFIRST,shiftData);
          //digitalWrite(addressLatch,HIGH);
          digitalWrite(10,LOW);
          SPI.transfer((shiftData>>8));
          SPI.transfer(shiftData);
          digitalWrite(10,HIGH);

          //Serial.print("OUTPUT TO SHIFT REGISTERS: ");
          //Serial.println(shiftData, BIN);

          //delay(1000);
          
          for(byte e=0;e<8;e++) {
            // set multiplexer to route ID number data from group D, module E
            // set multiplexer to route connection readings from group D, module E
            // set multiplexer to route analog readings from group D, module E
            // set multiplexer to route shift register latch to group D, module E (for shift out, not shift in)
            // set multiplexer to route auxiliary data from group D, module E (either from shift register or multiplexer)
    
            // if applicable, send binary data to group D, module E shift register (for LEDs etc)
    
            // if applicable, read binary data from group D, module E via shift register (switches, buttons, etc)
            //digitalWrite(addressE[0],bitRead(e,0));
            //digitalWrite(addressE[1],bitRead(e,1));
            //digitalWrite(addressE[2],bitRead(e,2));
            
            for(byte f=0;f<8;f++) {
              // set multiplexer to route ID number data from group D, module E, switch F
              // set multiplexer to route connection reading from group D, module E, socket F
              // set multiplexer(s) to route analog reading from group D, module E, channel F, including auxiliary multiplexer if used

              // read ID number data (group D, module E, bit F)
              // something like bitWrite(moduleIDReadings[d*8+e], f, digitalRead(MODULE_ID_PIN));
    
              // read whether module A, socket B, is connected to group D, module E, socket F
    
              // read analog data from group D, module E, channel F
    
              // if applicable, read analog data from auxiliary multiplexer on group D, module E, channel F
    
              // read serial data from MIDI port

              int socket1 = (a<<6)+(b<<3)+c;
              int socket2 = (d<<6)+(e<<3)+f;

              if(socket1 < socket2) {

                //digitalWrite(addressF[0],bitRead(f,0));
                //digitalWrite(addressF[1],bitRead(f,1));
                //digitalWrite(addressF[2],bitRead(f,2));
                PORTD = (e<<2) + (f<<5); // faster "port manipulation" version version of commented-out lines above
                
                delayMicroseconds(6); // if there is a suitable, slowish task to be done, could do it here instead of delay?
                // maybe put analogRead stuff here?
                
                if(!bitRead(PINC,2)) {
                  Serial.print(socket1);
                  Serial.print("->");
                  Serial.println(socket2);
                }
              }

              // do analog stuff here for now
              if(readFlag == 1) {
                //analogValues[socket2] = analogVal;
                cyclesSinceRead[socket2] = 0;
                readFlag = 0;
                int testVal = analogVal;
              } else {
                cyclesSinceRead[socket2]++;
                if(cyclesSinceRead[socket2] > 10) {
                  while(readFlag != 1) {
                    // wait
                  }
                  cyclesSinceRead[socket2] = 0;
                  readFlag = 0;
                  int testVal = analogVal;
                }
              }
            }
          }
        }
        if(c==0) innerEnd = millis();
      }
    }
  }
  Serial.print("ALL MODULES TIME: ");
  Serial.println(innerEnd - innerStart);
  Serial.print("TOTAL TIME: ");
  Serial.println(millis() - lastStart);
  Serial.println(thingFailed?"FAILED":"SUCCEEDED");
}

// Interrupt service routine for the ADC completion
ISR(ADC_vect){

  // Done reading
  readFlag = 1;
  
  // Must read low first
  analogVal = ADCL | (ADCH << 8);
  
  // Not needed because free-running mode is enabled.
  // Set ADSC in ADCSRA (0x7A) to start another ADC conversion
  // ADCSRA |= B01000000;
}
