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

// Define flag and timing variables
bool startEventFlag = 0;
bool finishEventFlag = 1;
int startMillis = 0;
int finishMillis = 1;
int runTimeMillis = 0;
float runTimeSeconds = 0.0;

// Not adding that magical bit, i guess we're just receiving so we dont need it
//esp_now_peer_info_t peerInfo;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //char macStr[18];
  //Serial.print("Packet received from: ");
  //snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  //Serial.println(macStr);
  memcpy(&gateMessage, incomingData, sizeof(gateMessage)); // You IDIOT! Why would you comment this out?!
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("Content: ");
  //Serial.print(gateMessage.messageType);
  //Serial.print(" ");
  //Serial.print(gateMessage.gateType);
  //Serial.print(" Flags: Start: ");
  //Serial.print(startEventFlag);
  //Serial.print(" Finish: ");
  //Serial.print(finishEventFlag);
  //Serial.println();
  // -- Above is like verbose debug code, no need for that when and if the code below works as intended, god strike me down... ok god please stand by, thank you stackoverflow for enlightening me
  if (strcmp(gateMessage.messageType, "E") == 0 && strcmp(gateMessage.gateType, "S") == 0 && startEventFlag == 0) {
    startMillis = millis();
    startEventFlag = 1;
    finishEventFlag = 0;
    Serial.print("Start! ... ");
    //Serial.println(startMillis);
  } else if (strcmp(gateMessage.messageType, "E") == 0 && strcmp(gateMessage.gateType, "F") == 0 && finishEventFlag == 0) {
    finishMillis = millis();
    startEventFlag = 0;
    finishEventFlag = 1; // autoreset
    runTimeMillis = finishMillis - startMillis;
    runTimeSeconds = runTimeMillis / 1000.0;
    Serial.print("Finish! Time: ");
    //Serial.println(finishMillis);
    Serial.println(runTimeSeconds, 3);
  } else {
  //  Serial.println("WTF xD");
  }
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
