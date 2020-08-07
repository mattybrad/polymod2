// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
bool ledStatus = false;

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

byte tickNum = 0;
byte tempMsg[4][4]; // could reduce this to 3 because we know the module number

void loop()
{
  
  Wire.beginTransmission(0); // broadcast to all devices
  Wire.write(0); // message type 0 (tick)
  Wire.write(tickNum);
  Wire.endTransmission();    // stop transmitting
  /*Wire.beginTransmission(2); // broadcast to all devices
  Wire.write(0); // message type 0 (tick)
  Wire.write(tickNum);
  Wire.endTransmission();    // stop transmitting*/
  tickNum++;

  if(tickNum == 32) {
    for(byte n=1; n<=2; n++) {
      Wire.requestFrom(n,18);
      byte x = 0;
      byte numNewConnected = 0;
      byte numNewDisconnected = 0;
      while(Wire.available()) {
        if(x==0) numNewConnected = Wire.read();
        else if(x==1) numNewDisconnected = Wire.read();
        else tempMsg[(x-2)/4][(x-2)%4] = Wire.read();
        x++;
      }
      for(byte i=0; i<numNewConnected + numNewDisconnected; i++) {
        if(i<numNewConnected) Serial.print("connected ");
        else Serial.print("disconnected ");
        Serial.print(tempMsg[i][0], DEC);
        Serial.print(":");
        Serial.print(tempMsg[i][1], DEC);
        Serial.print(" to ");
        Serial.print(tempMsg[i][2], DEC);
        Serial.print(":");
        Serial.print(tempMsg[i][3], DEC);
        Serial.println("");
      }
    }
    tickNum = 0;
    delay(1);
  }

  delay(1);
}
