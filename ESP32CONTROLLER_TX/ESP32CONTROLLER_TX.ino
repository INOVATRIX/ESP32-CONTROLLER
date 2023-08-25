#include <RotaryEncoder.h>
#include <esp_now.h>
#include <WiFi.h>

#define vrx 34
#define vry 33
#define sw 5
#define dt 12
#define clk 14
#define fwd 32
#define rev 27
#define lef 26
#define rit 35

RotaryEncoder encoder(clk, dt, RotaryEncoder::LatchMode::TWO03);
uint8_t broadcastAddress[] = {0xE8, 0xDB, 0x84, 0x0C, 0x58, 0x30};  // E8:DB:84:0C:58:30

typedef struct struct_message {
  int x;
  int y;
  int swi;
  int front;
  int back;
  int left;
  int right;
  int speed;
} struct_message;
struct_message data;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  pinMode(vrx, INPUT);
  pinMode(vry, INPUT);
  pinMode(sw, INPUT);
  pinMode(fwd, INPUT);
  pinMode(rev, INPUT);
  pinMode(lef, INPUT);
  pinMode(rit, INPUT);
}

void loop() {

  encoder.tick();
  data.speed = 10*(encoder.getPosition()); 
  

  if(digitalRead(sw) == 1){
    data.swi = 0;
  } else{
    data.swi = 1;
  }
  
  data.x = analogRead(vrx);
  data.y = analogRead(vry);
  data.front = digitalRead(fwd);
  data.back = digitalRead(rev);
  data.left = digitalRead(lef);
  data.right = digitalRead(rit);


  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));

}