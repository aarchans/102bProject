String incomingByte;
int startString = 100;
int endString = 101;
int output;
int notIndex = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    incomingByte = Serial.readString().toInt(); // read the incoming byte:
    if (incomingByte == "100") {
      incomingByte = Serial.readString().toInt();
      int output[incomingByte.toInt()];
      int index = 0;
      int notIndex = index;
    }else if (incomingByte == "101") {
      Serial.println(output);
    }else {
      output[index] = incomingByte;
      notIndex++;
      index = notIndex;
    }
  }
}
