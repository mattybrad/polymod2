#include <Wire.h>
#include <EEPROM.h>

const byte outPins[] = {2,3,4,5};
const byte inPins[] = {6,7,8,9};
byte moduleNum;
byte prevConnections[4][2];
byte newConnections[4][2];
byte prevConfirmedConnections[4][2];
byte confirmedConnections[4][2];
bool sendConnection[4];
bool sendDisconnection[4];
byte unchangedCount[4];
byte everConnected[4];

void setup() {
  moduleNum = EEPROM.read(0);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin(moduleNum);
  TWAR = (moduleNum << 1) | 1; // enable broadcasts to be received http://www.gammon.com.au/i2c
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  for(byte i=0;i<4;i++) {
    pinMode(outPins[i], OUTPUT);
    pinMode(inPins[i], INPUT_PULLUP);
    prevConnections[i][0] = 255;
    prevConnections[i][1] = 255;
  }
}

void loop() {
  
}

byte byteNum = 0;
byte message[2];
byte tickNum = 0;
bool ledStatus = false;

void receiveEvent(int howMany) {
  while(Wire.available()) {
    message[byteNum] = Wire.read();
    byteNum ++;
    if(byteNum == 2) {
      if(message[0] == 0) {
        tickNum = message[1];
        doTick();
      }
      byteNum = 0;
    }
  }
}

void requestEvent(int howMany) {
  byte numNewConnected = 0;
  byte numNewDisconnected = 0;
  for(byte i=0; i<4; i++) {
    if(sendConnection[i]) {
      numNewConnected ++;
    }
    if(sendDisconnection[i]) {
      numNewDisconnected ++;
    }
  }
  Wire.write(numNewConnected);
  Wire.write(numNewDisconnected);
  for(byte i=0; i<4; i++) {
    if(sendConnection[i]) {
      Wire.write(confirmedConnections[i][0]);
      Wire.write(confirmedConnections[i][1]);
      Wire.write(moduleNum);
      Wire.write(i);
      sendConnection[i] = false;
    }
  }
  for(byte i=0; i<4; i++) {
    if(sendDisconnection[i]) {
      Wire.write(prevConfirmedConnections[i][0]);
      Wire.write(prevConfirmedConnections[i][1]);
      Wire.write(moduleNum);
      Wire.write(i);
      sendDisconnection[i] = false;
    }
  }
}

void doTick() {
  if(tickNum==0) {
    for(byte i=0; i<4; i++) {
      newConnections[i][0] = 255;
      newConnections[i][1] = 255;
    }
  }
  byte phaseIndex = tickNum/2;
  if(tickNum%2==0) {
    // write phase
    for(byte i=0; i<4; i++) {
      if(phaseIndex<8) {
        digitalWrite(outPins[i], bitRead(moduleNum, phaseIndex));
      } else {
        digitalWrite(outPins[i], bitRead(i, phaseIndex - 8));
      }
    }
  } else {
    // read phase
    for(byte i=0; i<4; i++) {
      if(phaseIndex<8) {
        bitWrite(newConnections[i][0], phaseIndex, digitalRead(inPins[i]));
      } else {
        bitWrite(newConnections[i][1], phaseIndex - 8, digitalRead(inPins[i]));
      }
    }
  }
  if(tickNum==31) {
    for(byte i=0; i<4; i++) {
      if(newConnections[i][0] != prevConnections[i][0] || newConnections[i][1] != prevConnections[i][1]) {
        prevConnections[i][0] = newConnections[i][0];
        prevConnections[i][1] = newConnections[i][1];
        unchangedCount[i] = 0;
      } else {
        if(unchangedCount[i] < 3) {
          unchangedCount[i] ++;
        }
        if(unchangedCount[i] == 2) {
          prevConfirmedConnections[i][0] = confirmedConnections[i][0];
          prevConfirmedConnections[i][1] = confirmedConnections[i][1];
          confirmedConnections[i][0] = newConnections[i][0];
          confirmedConnections[i][1] = newConnections[i][1];
          if(confirmedConnections[i][0]==255) {
            if(everConnected[i]) {
              Serial.print("disconnect ");
              Serial.print("m");
              Serial.print(prevConfirmedConnections[i][0], DEC);
              Serial.print("p");
              Serial.print(prevConfirmedConnections[i][1], DEC);
              Serial.print("->m");
              Serial.print(moduleNum, DEC);
              Serial.print("p");
              Serial.print(i, DEC);
              Serial.println("");
              sendDisconnection[i] = true;
            }
          } else {
            Serial.print("connect ");
            Serial.print("m");
            Serial.print(confirmedConnections[i][0], DEC);
            Serial.print("p");
            Serial.print(confirmedConnections[i][1], DEC);
            Serial.print("->m");
            Serial.print(moduleNum, DEC);
            Serial.print("p");
            Serial.print(i, DEC);
            Serial.println("");
            sendConnection[i] = true;
            everConnected[i] = true;
          }
        }
      }
    }
  }
}
