#include <TFT_eSPI.h>   // Graphics and font library for ST7789 driver
#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2   // Define the pin connected to the LED (GPIO 2 is typically used for built-in LED)

// Structure to receive data from sender
typedef struct struct_message {
  int touchStatus;  // 0 if no touch, 1 if touch detected
} struct_message;

struct_message receivedData;

// Updated callback function to match new esp_now_recv_cb_t signature
void onReceive(const esp_now_recv_info* info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // If touch is detected, turn the LED on, else turn it off
  if (receivedData.touchStatus == 1) {
    digitalWrite(LED_PIN, HIGH);  // Turn on LED
    Serial.println("Touch detected, LED ON.");
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED
    Serial.println("No touch, LED OFF.");
  }
}

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Make sure the LED is off initially

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function for receiving data
  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver ready, waiting for touch data...");
}

void loop() {
  // Nothing needed in loop, the onReceive callback handles the data reception and LED control
}
