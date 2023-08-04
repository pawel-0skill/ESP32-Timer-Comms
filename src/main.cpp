/*
  Thank you Rui Santos https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// No Comms address yet - but we will set two later, to ping Gates

// Define message structure
typedef struct struct_message {
  char messageType[32]; // P for Pairing, E for Event
  char gateType[32]; // S for Start, F for Finish
} struct_message;
struct_message gateMessage;

// Not adding that magical bit, i guess we're just receiving so we dont need it
//esp_now_peer_info_t peerInfo;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //char macStr[18];
  //Serial.print("Packet received from: ");
  //snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  //Serial.println(macStr);
  //memcpy(&gateMessage, incomingData, sizeof(gateMessage));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("Content: ");
  Serial.print(gateMessage.messageType);
  Serial.print(" ");
  Serial.print(gateMessage.gateType);
  Serial.println();
}
 
void setup() {
  // TODO: LED init

  // Serial init
  Serial.begin(9600);
  
  // Wifi init, WiFi station, ESP-NOW init
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
    // TODO later - LED feedback
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
