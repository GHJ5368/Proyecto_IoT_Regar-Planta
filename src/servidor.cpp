#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // If not work please scan the bus
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire);

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;
struct_message incomingData;

//MAC address
uint8_t destinationAddress [] = {0xE8,0xDB,0x84,0x90,0x48,0x39};

unsigned long lastTime = 0;  
unsigned long timerDelay = 10000;  // send readings timer

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.println("----");
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void OnDataRcv(uint8_t *mac_addr, struct_message incoming_data, uint8_t len){
  Serial.println("Data Received");
  Serial.println("Integuer recibido:");
  Serial.println(incomingData.a);

  if(incoming_data.a > 40){

    digitalWrite(D1, true);

  }else{
    digitalWrite(D1, false);
  }

}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRcv);
  esp_now_add_peer(destinationAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  // Codigo de la pantalla
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
  display.println("Hello World");

  display.display();
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    myData.a= 4;

    // Send message via ESP-NOW
    esp_now_send(destinationAddress, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();
    display.display();
  }
}