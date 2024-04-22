#include <esp_now.h>
#include <WiFi.h>

uint8_t BoardThreeAddress[] = {0xA8, 0x03, 0x2A, 0xEB, 0x02, 0xB4};

esp_now_peer_info_t peerInfo;


typedef struct test_struct {
  int x;
} test_struct;

test_struct test;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  char macStr[18];
  //Serial.print("Packet to:");
  //snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0],
  //mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  //Serial.print(" send status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, BoardThreeAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer 2");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    test.x = Serial.readStringUntil('\n').toInt(); // read the incoming byte:
    Serial.print("X: ");
    Serial.println(test.x);
    esp_err_t result = esp_now_send(BoardThreeAddress, (uint8_t *) &test, sizeof(test_struct));
  }
}

