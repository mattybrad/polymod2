#include <Bounce2.h>

// module board

#include <SoftwareSerial.h>


SoftwareSerial mySerial1(10, 11); // RX, TX
SoftwareSerial mySerial1(10, 11); // RX, TX
Bounce debouncer = Bounce(); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  mySerial.begin(4800);

  pinMode(2,INPUT_PULLUP);
  debouncer.attach(2);
  debouncer.interval(5); // interval in ms
}

void loop() {
  // put your main code here, to run repeatedly:
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  debouncer.update();
  if(debouncer.fell()) {
    mySerial.write("module 2 says hello");
  }
}
