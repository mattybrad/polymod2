#include <Wire.h>

int latchPin = 8;
int clockPin = 10;
int dataPin = 9;

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("polymod main board started");
  Wire.begin(); // join i2c bus (address optional for master)
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
  delay(5000); // allow modules to power up
  for(int i=0;i<8;i++) {
    byte thisByte = 0;
    bitWrite(thisByte,i,HIGH);
    for(int j=0; j<i+1; j++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, thisByte);
      digitalWrite(latchPin, HIGH);
      delay(10);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
      digitalWrite(latchPin, HIGH);
      delay(10);
    }
  }
  delay(3000);
  for(int i=0; i<8; i++) {
    Wire.requestFrom(i+1,1);
    while(Wire.available()) {
      byte thisModuleType = Wire.read();
      Serial.print("slot ");
      Serial.print(i);
      Serial.print(" type ");
      Serial.println(thisModuleType);
    }
  }
  Serial.println("end of setup");
}

void loop()
{
  
}
