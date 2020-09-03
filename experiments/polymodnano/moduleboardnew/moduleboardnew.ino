#include <Bounce2.h>
#include <Wire.h>

byte idPin = 10;
byte ledPin = 13;
int moduleNum = 0;
int moduleType = 12; // will be different for a VCO, LFO, etc
unsigned long lastRead = 0;
bool foundModuleNum = false;
const byte outPins[] = {2,3,4,5};
const byte inPins[] = {6,7,8,9};
byte prevConnections[4][2];
byte newConnections[4][2];
byte prevConfirmedConnections[4][2];
byte confirmedConnections[4][2];
byte unchangedCount[4];
bool sendConnection[4];
bool sendDisconnection[4];
byte everConnected[4];

Bounce b = Bounce();

void setup() {
  while(!Serial);
  Serial.begin(9600);
  for(byte i=0;i<4;i++) {
    pinMode(outPins[i], OUTPUT);
    pinMode(inPins[i], INPUT_PULLUP);
    prevConnections[i][0] = 255;
    prevConnections[i][1] = 255;
  }
  b.attach(idPin, INPUT);
  b.interval(5);
  pinMode(ledPin, OUTPUT);
  delay(2000);
  lastRead = millis();
  Serial.println("listening...");
}

void loop() {
  b.update();
  if(b.rose()) {
    moduleNum ++;
    lastRead = millis();
  }
  if(!foundModuleNum && millis() > lastRead + 2000 && moduleNum > 0) {
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
    if(!numBytesSent) {
      Wire.write(3); // sending 3 bytes for testing
      numBytesSent = true;
    } else {
      Wire.write(2);
      Wire.write(0);
      int testAnalogValue = analogRead(0) / 4;
      Wire.write(testAnalogValue);
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
