#include <TFT_eSPI.h>  // Graphics and font library for ST7789 driver
#include <esp_now.h>
#include <WiFi.h>

// Define the touch pin connected to conductive yarn
#define TOUCH_PIN 4

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display

//Sender_Mac: e8:31:cd:59:87:78
//Rceiver_Mac: e8:31:cd:16:20:c8
uint8_t broadcastAddress[] = {0xE8, 0x31, 0xCD, 0x16, 0x20, 0xC8}; // MAC address of the receiver ESP32

// Sensitivity threshold for detecting a touch
int threshold = 40;

typedef struct struct_message {
  int touchStatus;  // 0 if no touch, 1 if touch detected
} struct_message;

struct_message myData;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {

  // Initialize serial monitor
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onSent);

   // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW initialized successfully");
  }

  //Register to peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
  Serial.println("Failed to add peer");
  return;
}

  tft.init();
  tft.setRotation(0);  // Adjust rotation if necessary
  
  // Set the background to white
  tft.fillScreen(TFT_WHITE);
  
  // Define the center of the display
  int centerX = tft.width() / 2;
  int centerY = tft.height() / 2;

  // Define the radius of the circle (adjust as needed)
  int radius1 = 50;
  int radius2 = 70;

  // Draw a black circle in the middle of the screen
  
  tft.fillCircle(centerX, centerY, radius2, TFT_BLUE);
  tft.fillCircle(centerX, centerY, radius1, TFT_BLACK);
}

void loop() {
  
  // Read touch sensor value from the conductive yarn
  int touchValue = touchRead(TOUCH_PIN);
  
  // Check if the touch value is below the threshold
  if (touchValue < threshold) {
    myData.touchStatus = 1;  // Touch detected
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    delay(200);
  } else {
    Serial.println("No touch detected.");
    myData.touchStatus = 0;
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
  
  // Delay for stability and to make the readings more readable
  delay(500);
}