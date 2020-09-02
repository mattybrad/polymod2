#include <Bounce2.h>
#include <Wire.h>

byte idPin = 10;
byte ledPin = 13;
int moduleNum = 0;
int moduleType = 227; // will be different for a VCO, LFO, etc
unsigned long lastRead = 0;
bool foundModuleNum = false;

Bounce b = Bounce();

void setup() {
  Serial.begin(9600);
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

void receiveEvent(int howMany) {
  while(Wire.available()) {
    message[byteNum] = Wire.read();
    byteNum ++;
    if(byteNum == 2) {
      if(message[0] == 0) {
        tickNum = message[1];
        //doTick();
      }
      byteNum = 0;
    }
  }
}

void requestEvent() {
  Wire.write(moduleType);
}
