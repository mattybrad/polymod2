#include <Bounce2.h>
#include <Wire.h>

byte idPin = 10;
int moduleNum = 0;
int moduleType = 15; // will be different for a VCO, LFO, etc
unsigned long lastRead = 0;
bool foundModuleNum = false;
const byte socketOutPins[] = {2,3,4,5};
const byte socketInPins[] = {6,7,8,9};
const byte digitalOutPins[] = {13,12};
byte prevConnections[4][2];
byte newConnections[4][2];
byte prevConfirmedConnections[4][2];
byte confirmedConnections[4][2];
byte unchangedCount[4];
bool sendConnection[4];
bool sendDisconnection[4];
byte everConnected[4];
int storedAnalogValues[2];

Bounce b = Bounce();
Bounce b2 = Bounce();

void setup() {
  while(!Serial);
  Serial.begin(9600);

  // init pins and connection variables
  for(byte i=0;i<4;i++) {
    pinMode(socketOutPins[i], OUTPUT);
    pinMode(socketInPins[i], INPUT_PULLUP);
    prevConnections[i][0] = 255;
    prevConnections[i][1] = 255;
  }
  for(byte i=0; i<2; i++) {
    pinMode(digitalOutPins[i], OUTPUT);
  }
  b.attach(idPin, INPUT);
  b.interval(1);
  
  delay(1000); // wait for main module to boot up
  lastRead = millis();
  Serial.println("listening...");
}

void loop() {
  if(!foundModuleNum) {
    b.update();
    if(b.rose()) {
      moduleNum ++;
      lastRead = millis();
    }
    if(!foundModuleNum && millis() > lastRead + 50 && moduleNum > 0) {
      foundModuleNum = true;
      Wire.begin(moduleNum);
      TWAR = (moduleNum << 1) | 1; // enable broadcasts to be received http://www.gammon.com.au/i2c
      Wire.onReceive(receiveEvent);
      Wire.onRequest(requestEvent);
      Serial.print("module type ");
      Serial.print(moduleType);
      Serial.print(" found in slot ");
      Serial.println(moduleNum);
    }
  }
  b2.update();
}

byte byteNum = 0;
byte message[2];
byte tickNum = 0;
byte messageLengths[] = {2,1,3,3,3,3,5,5};

void receiveEvent(int howMany) {
  while(Wire.available()) {
    message[byteNum] = Wire.read();
    byteNum ++;
    if(byteNum >= messageLengths[message[0]]) {
      // message complete, handle command
      switch(message[0]) {
        case 0:
        tickNum = message[1];
        doTick();
        break;
        
        case 5:
        byte channelNum = message[1];
        if(channelNum <= 1) {
          digitalWrite(digitalOutPins[channelNum], message[2]);
        }
        break; 
      }
      byteNum = 0;
    }
  }
}

bool numBytesSent = false;
void requestEvent() {
  if(message[0]==1) {
    // send module type
    Wire.write(moduleType);
    Serial.println("sending module type");
  } else if(message[0]==0) {
    // send changed data
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

    // todo next: calculate number of bytes to be sent
    
    int testAnalogValue = analogRead(0);
    bool sendChange = false;
    if(testAnalogValue!=storedAnalogValues[0]) {
      sendChange = true;
      storedAnalogValues[0] = testAnalogValue;
    }
    if(!numBytesSent) {
      if(sendChange) Wire.write(3); // sending 3 bytes for testing
      else Wire.write(0);
      numBytesSent = sendChange;
    } else {
      byte lowRes = testAnalogValue/4;
      Wire.write(2);
      Wire.write(0);
      Wire.write(lowRes);
      numBytesSent = false;
    }
    //Wire.write(numNewConnected);
    //Wire.write(numNewDisconnected);
    for(byte i=0; i<4; i++) {
      if(sendConnection[i]) {
        /*Wire.write(6); // connection message
        Wire.write(confirmedConnections[i][0]);
        Wire.write(confirmedConnections[i][1]);
        Wire.write(moduleNum);
        Wire.write(i);*/
        sendConnection[i] = false;
      }
    }
    for(byte i=0; i<4; i++) {
      if(sendDisconnection[i]) {
        /*Wire.write(7); // disconnection message
        Wire.write(prevConfirmedConnections[i][0]);
        Wire.write(prevConfirmedConnections[i][1]);
        Wire.write(moduleNum);
        Wire.write(i);*/
        sendDisconnection[i] = false;
      }
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
        digitalWrite(socketOutPins[i], bitRead(moduleNum, phaseIndex));
      } else {
        digitalWrite(socketOutPins[i], bitRead(i, phaseIndex - 8));
      }
    }
  } else {
    // read phase
    for(byte i=0; i<4; i++) {
      if(phaseIndex<8) {
        bitWrite(newConnections[i][0], phaseIndex, digitalRead(socketInPins[i]));
      } else {
        bitWrite(newConnections[i][1], phaseIndex - 8, digitalRead(socketInPins[i]));
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
