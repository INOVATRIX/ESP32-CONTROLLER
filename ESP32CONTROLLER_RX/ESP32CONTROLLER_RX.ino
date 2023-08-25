#include <esp_now.h>
#include <WiFi.h>

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

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print(data.x);
  Serial.print("\t");
  Serial.print(data.y);
  Serial.print("\t");
  Serial.print(data.swi);
  Serial.print("\t");
  Serial.print(data.front);
  Serial.print("\t");
  Serial.print( data.back);
  Serial.print("\t");
  Serial.print(data.left);
  Serial.print("\t");
  Serial.print(data.right);
  Serial.print("\t");
  Serial.print(data.speed);
  Serial.println("\t");
}
 
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}