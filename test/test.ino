// THIS IS THE MAIN CODE FOR THE ESP THAT IS CONNECTED TO THE COMPUTER AND THAT IS SENDING OUT THE WIFI DATA

#include <esp_now.h>
#include <WiFi.h>

const int forwardPin = 34;  // the number of the pushbutton pin
const int backPin = 39;

int forwardState;            // the current reading from the input pin
int backState;
int lastForwardState = LOW;  // the previous reading from the input pin
int lastBackState = LOW;

unsigned long lastForwardTime = 0;  // the last time the output pin was toggled
unsigned long lastBackTime = 0;
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

//MAC Addresses of the 5 ESPs that are being used

uint8_t BoardOneAddress[] = {0x84, 0xCC, 0xA8, 0x12, 0x31, 0x8C}; //MAC address of main board
uint8_t BoardTwoAddress[] = {0x7C, 0x9E, 0xBD, 0xD8, 0xE3, 0xEC};
uint8_t BoardThreeAddress[] = {0xA8, 0x03, 0x2A, 0xEB, 0x02, 0xB4};
uint8_t BoardFourAddress[] = {0x40, 0x91, 0x51, 0x1E, 0x2D, 0x4C};
uint8_t BoardFiveAddress[] = {0x4C, 0x75, 0x25, 0xF4, 0x30, 0xE4};
esp_now_peer_info_t peerInfo;

typedef struct test_struct {
  int setMotorOne;
  int setMotorTwo;
} test_struct;

test_struct dataSent1;
test_struct dataSent2;
test_struct dataSent3;
test_struct dataSent4;
test_struct dataSent5;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  char macStr[18];
  //Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0],
  mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  //Serial.print(" send status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(forwardPin, INPUT);
  pinMode(backPin, INPUT);


  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);


  // Register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, BoardOneAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 1");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardTwoAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 2");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardThreeAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 3");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardFourAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 4");
    return;
  }

  memcpy(peerInfo.peer_addr, BoardFiveAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 5");
    return;
  }


}

void loop() {
  int forwardReading = digitalRead(forwardPin);
  int backReading = digitalRead(backPin);

  if (forwardReading != lastForwardState) {
    lastForwardTime = millis();
  }

  if (backReading != lastBackState) {
    lastBackTime = millis();
  }

  if ((millis() - lastForwardTime) > debounceDelay) {

    if (forwardReading != forwardState) {
      forwardState = forwardReading;

      if (forwardState == HIGH) {
        Serial.println(1);
      }
    }
  }

  if ((millis() - lastBackTime) > debounceDelay) {

    if (backReading != 0) {
      backState = backReading;

      if (backState == HIGH) {
        Serial.println(-1);
      }
    }
  }

  lastForwardState = forwardReading;
  lastBackState = backReading;

  // End of Button Code
  if (Serial.available() > 0) {
    dataSent1.setMotorOne = (Serial.readStringUntil('\n').toInt())*640;
    dataSent1.setMotorTwo = (Serial.readStringUntil('\n').toInt())*640;
    dataSent2.setMotorOne = (Serial.readStringUntil('\n').toInt())*640;
    dataSent2.setMotorTwo = (Serial.readStringUntil('\n').toInt())*640;
    dataSent3.setMotorOne = (Serial.readStringUntil('\n').toInt())*640;
    dataSent3.setMotorTwo = (Serial.readStringUntil('\n').toInt())*640;
    dataSent4.setMotorOne = (Serial.readStringUntil('\n').toInt())*640;
    dataSent4.setMotorTwo = (Serial.readStringUntil('\n').toInt())*640;
    dataSent5.setMotorOne = (Serial.readStringUntil('\n').toInt())*640;
    dataSent5.setMotorTwo = (Serial.readStringUntil('\n').toInt())*640;
  }

  esp_err_t result1 = esp_now_send(BoardOneAddress, (uint8_t *) &dataSent1, sizeof(test_struct));
  delay(10);
  esp_err_t result2 = esp_now_send(BoardTwoAddress, (uint8_t *) &dataSent2, sizeof(test_struct));
  delay(10);
  esp_err_t result3 = esp_now_send(BoardThreeAddress, (uint8_t *) &dataSent3, sizeof(test_struct));
  delay(10);
  esp_err_t result4 = esp_now_send(BoardFourAddress, (uint8_t *) &dataSent4, sizeof(test_struct));
  delay(10);
  esp_err_t result5 = esp_now_send(BoardFiveAddress, (uint8_t *) &dataSent5, sizeof(test_struct));
  delay(10);
}