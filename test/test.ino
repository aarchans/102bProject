// THIS IS THE MAIN CODE FOR THE ESP THAT IS CONNECTED TO THE COMPUTER AND THAT IS SENDING OUT THE WIFI DATA

#include <AccelStepper.h>
#include <esp_now.h>
#include <WiFi.h>

#define FullStep 4 // THIS IS FOR FULL STEP CONTROL
#define HalfStep 8 // THIS IS FOR HALF STEP CONTROL

// GOES Pin 1, Pin 3, Pin 2, Pin 4
AccelStepper myStepper(FullStep, 26, 25, 4, 21);
AccelStepper myStepper2(FullStep, 13, 12, 27, 33);

//MAC Addresses of the 5 ESPs that are being used

//uint8_t BoardOneAddress[] = {0x84, 0xCC, 0xA8, 0x12, 0x31, 0x8C}; MAC address of main board
uint8_t BoardTwoAddress[] = {0x7C, 0x9E, 0xBD, 0xD8, 0xE3, 0xEC};
uint8_t BoardThreeAddress[] = {0xA8, 0x03, 0x2A, 0xEB, 0x02, 0xB4};
uint8_t BoardFourAddress[] = {0x40, 0x91, 0x51, 0x1E, 0x2D, 0x4C};
uint8_t BoardFiveAddress[] = {0x4C, 0x75, 0x25, 0xF4, 0x30, 0xE4};
esp_now_peer_info_t peerInfo;

int currTime = millis();


typedef struct test_struct {
  int setMotorOne;
  int setMotorTwo;
} test_struct;

test_struct dataSent2;
test_struct dataSent3;
test_struct dataSent4;
test_struct dataSent5;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0],
  mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(50.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(2048);

  myStepper2.setMaxSpeed(1000.0);
  myStepper2.setAcceleration(50.0);
  myStepper2.setSpeed(200);
  myStepper2.moveTo(2048);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);


  // Register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, BoardTwoAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardThreeAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 3");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardFourAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 4");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardFiveAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 5");
    return;
  }


}

void loop() {
  // step one revolution  in one direction:
  if (myStepper.distanceToGo() == 0){
    myStepper.moveTo(-myStepper.currentPosition());
  }
  if (myStepper2.distanceToGo() == 0){
    myStepper2.moveTo(-myStepper2.currentPosition());
  }
  myStepper.run();
  myStepper2.run();

  dataSent1.setMotorOne = random(0,2038);
  dataSent1.setMotorTwo = random(0,2038);
  int current = millis();
  if ((current - currTime) > 5000) {
    esp_err_t result = esp_now_send(BoardTwoAddress, (uint8_t *) &dataSent2, sizeof(test_struct));
    esp_err_t result = esp_now_send(BoardThreeAddress, (uint8_t *) &dataSent3, sizeof(test_struct));
    esp_err_t result = esp_now_send(BoardFourAddress, (uint8_t *) &dataSent4, sizeof(test_struct));
    esp_err_t result = esp_now_send(BoardFiveAddress, (uint8_t *) &dataSent5, sizeof(test_struct));

    currTime = millis();

  }
  
}