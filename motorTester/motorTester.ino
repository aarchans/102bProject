#include <AccelStepper.h>

#define FullStep 4

AccelStepper myStepper(FullStep, 26, 25, 4, 21);

int accel = 600;
int maxSpeed = 1000;
int position = 0;
int speed = 1000;

void setup() {
  Serial.begin(115200);
  myStepper.setMaxSpeed(maxSpeed);
  myStepper.setAcceleration(accel);
  myStepper.setSpeed(speed);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
  String reading = Serial.readStringUntil('\n');  // read from the Serial Monitor
  switch(reading[0]){
    case 'a':
      accel = reading.substring(1).toInt();
      break;
    case 's':
      speed = reading.substring(1).toInt();
      break;
    case 'p':
      position = reading.substring(1).toInt();
      break;
    case 'm':
      maxSpeed = reading.substring(1).toInt();
      break;
    }
  } 


  myStepper.setMaxSpeed(maxSpeed);
  myStepper.setAcceleration(accel);
  myStepper.setSpeed(speed);
  myStepper.moveTo(position);
  myStepper.runToPosition();

  Serial.print("Max Speed: ");
  Serial.print(maxSpeed);
  Serial.print(" Accel: ");
  Serial.print(accel);
  Serial.print(" Speed: ");
  Serial.print(speed);
  Serial.print(" Current Position: ");
  Serial.print(myStepper.currentPosition());
  Serial.print(" Position: ");
  Serial.println(position);
}

