//THIS IS THE CODE FOR THE BOARDS THAT ARE RECEIVING THE WIFI DATA
#include <esp_now.h>
#include <WiFi.h>
#include <AccelStepper.h>

#define FullStep 4
#define HalfStep 8

AccelStepper myStepper(FullStep, 26, 25, 4, 21);
AccelStepper myStepper2(FullStep, 13, 12, 27, 33);


typedef struct test_struct {
  int setMotorOne;
  int setMotorTwo;
} test_struct;

test_struct myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(myData.setMotorOne);
  Serial.print("y: ");
  Serial.println(myData.setMotorTwo);
  Serial.println();

}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(200.0);
  myStepper.setSpeed(800);
  myStepper2.setMaxSpeed(1000.0);
  myStepper2.setAcceleration(200.0);
  myStepper2.setSpeed(800);

}

void loop() {
  // put your main code here, to run repeatedly:
  myStepper.moveTo(myData.setMotorOne);
  myStepper2.moveTo(myData.setMotorTwo);
  myStepper.runToPosition();
  myStepper2.runToPosition();
}
