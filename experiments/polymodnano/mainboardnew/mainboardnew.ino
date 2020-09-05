#include <Wire.h>

int latchPin = 8;
int clockPin = 10;
int dataPin = 9;

const int maxModules = 8;
int moduleTypes[maxModules];
byte tickNum = 0;

void setup()
{
  while(!Serial);
  Serial.begin(9600);

  // init pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(13, OUTPUT);

  
  Serial.println("polymod main board started");
  Wire.begin(); // join i2c bus (address optional for master)
  
  // init ID shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
  
  delay(2000); // allow modules to power up
  
  // for each module number, send different number of pulses
  for(int i=0;i<maxModules;i++) {
    byte thisByte = 0;
    bitWrite(thisByte,i,HIGH);
    for(int j=0; j<i+1; j++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, thisByte);
      digitalWrite(latchPin, HIGH);
      delay(3);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
      digitalWrite(latchPin, HIGH);
      delay(3);
    }
  }
  
  delay(100); // wait for modules to be sure of their IDs
  
  for(int i=0; i<maxModules; i++) {
    moduleTypes[i] = 0; // default to type 0 (no module connected)
    Wire.beginTransmission(i+1); // broadcast to all modules
    Wire.write(1); // message type 1 (module type request)
    Wire.endTransmission();    // stop transmitting
    Wire.requestFrom(i+1,1);
    while(Wire.available()) {
      moduleTypes[i] = Wire.read();
      Serial.print("slot ");
      Serial.print(i);
      Serial.print(" type ");
      Serial.println(moduleTypes[i]);
    }
  }
  Serial.println("end of setup");
}

byte thisMessage[8]; // max message length currently 5 but giving some wiggle room
byte messageLengths[] = {2,1,3,3,3,3,5,5};
void loop()
{
  Wire.beginTransmission(0); // broadcast to all modules
  Wire.write(0); // message type 0 (tick)
  Wire.write(tickNum);
  Wire.endTransmission();    // stop transmitting

  tickNum++;

  if(tickNum == 32) {
    // on last tick, request data from all modules
    for(uint8_t n=0; n<=maxModules; n++) {
      Wire.requestFrom(n+1,1); // request data
      uint8_t bytesExpected = 0;
      while(Wire.available()) {
        bytesExpected = Wire.read();        
      }
      byte byteNum = 0;
      uint8_t channelNum = n+1;
      Wire.requestFrom(channelNum,bytesExpected);
      while(Wire.available()) {
        thisMessage[byteNum] = Wire.read();
        byteNum ++;
        if(byteNum >= messageLengths[thisMessage[0]]) {
          // message complete, handle command
          switch(thisMessage[0]) {
            case 6:
            Serial.println("connection");
            break;

            case 7:
            Serial.println("disconnection");
            break;

            case 2:
            Serial.print("analog value: ");
            Serial.println(thisMessage[2]);
            Wire.beginTransmission(channelNum); // broadcast to all modules
            Wire.write(5); // message type 0 (tick)
            byte digiChan = thisMessage[2]/128;
            byte digiVal = (thisMessage[2]/8)%2;
            Wire.write(digiChan);
            Wire.write(digiVal);
            Wire.endTransmission();    // stop transmitting
            Serial.println(digiChan);
            Serial.println(digiVal);
          }
          byteNum = 0;
        }
      }
    }
    tickNum = 0;
    delayMicroseconds(10);
  }

  delayMicroseconds(10);
}
