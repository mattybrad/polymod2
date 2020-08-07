// main board

const byte addressPins[] = {2,3,4};

void setup() {
  // put your setup code here, to run once:
  for(byte i=0; i<3; i++) {
    pinMode(addressPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    // read the incoming byte:
    byte incomingByte = Serial.read();

    // say what you got:
    Serial.print("main board received: ");
    Serial.write(incomingByte);
    Serial.println("");
  }
}
