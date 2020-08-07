#include <Bounce2.h>
#include <SoftwareSerial.h>

const byte moduleID = 1;

SoftwareSerial softSerial1(4, 5); // RX, TX
SoftwareSerial softSerial2(6, 7); // RX, TX
SoftwareSerial softSerial3(8, 9); // RX, TX
SoftwareSerial softSerial4(10, 11); // RX, TX

Bounce debouncer = Bounce(); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  softSerial1.begin(19200);
  softSerial2.begin(19200);
  softSerial3.begin(19200);
  softSerial4.begin(19200);

  pinMode(2,INPUT_PULLUP);
  debouncer.attach(2);
  debouncer.interval(5); // interval in ms
}

bool incrementChannel = false;
byte inputChannel = 0;
unsigned long nextTick = 0;

void loop() {
  // put your main code here, to run repeatedly:
  while (softSerial1.available()) {
    Serial.write(softSerial1.read());
  }
  while (softSerial2.available()) {
    Serial.write(softSerial2.read());
  }
  while (softSerial3.available()) {
    Serial.write(softSerial3.read());
  }
  while (softSerial4.available()) {
    Serial.write(softSerial4.read());
  }
  debouncer.update();
  if(debouncer.fell()) {
    //nextTick = millis() + 100;
    if(incrementChannel) {
      inputChannel ++;
      if(inputChannel >= 4) inputChannel = 0;
      switch(inputChannel) {
        case 0:
        softSerial1.listen();
        break;
        case 1:
        softSerial2.listen();
        break;
        case 2:
        softSerial3.listen();
        break;
        case 3:
        softSerial4.listen();
        break;
      }
    } else {
      /*softSerial1.print("m");
      softSerial1.print(moduleID);
      softSerial1.println("p1");
      softSerial2.print("m");
      softSerial2.print(moduleID);
      softSerial2.println("p2");
      softSerial3.print("m");
      softSerial3.print(moduleID);
      softSerial3.println("p3");
      softSerial4.print("m");
      softSerial4.print(moduleID);
      softSerial4.println("p4");*/
      softSerial1.print(1, DEC);
      softSerial2.print(2, DEC);
      softSerial3.print(3, DEC);
      softSerial4.print(4, DEC);
    }
    incrementChannel = !incrementChannel;
  }
}
