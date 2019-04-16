byte analogValues[8][8][8];
//bool patchConnections[512][512];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(500000);
}

byte nextPosition = 0;
byte currentCommand[8];

void loop() {
  while(Serial1.available()) {
    byte thisByte = Serial1.read();
    if(nextPosition==0) {
      // new command!
      currentCommand[0] = thisByte;
      if(thisByte>0) nextPosition ++; // don't increment position for command 0 because it doesn't have any other data expected
      else Serial.println("LOOP STARTED");
    } else {
      currentCommand[nextPosition] = thisByte;
      switch(currentCommand[0]) {
        case 1:
        nextPosition++;
        if(nextPosition>6) {
          nextPosition=0;
          Serial.print("PATCH CONNECTION: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(" to ");
          Serial.print(currentCommand[4]);
          Serial.print("-");
          Serial.print(currentCommand[5]);
          Serial.print("-");
          Serial.println(currentCommand[6]);
        }
        break;

        case 2:
        nextPosition++;
        if(nextPosition>4) {
          nextPosition=0;
          Serial.print("ANALOG READING: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(": ");
          Serial.println(currentCommand[4]);
        }
        break;

        case 3:
        nextPosition++;
        if(nextPosition>3) {
          nextPosition=0;
          Serial.print("MODULE ID: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print(": ");
          Serial.println(currentCommand[3]);
        }
        break;
      }
    }
  }
}
