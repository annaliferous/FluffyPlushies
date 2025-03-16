#include <TFT_eSPI.h>  // Graphics and font library
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>


// Define the touch pin connected to conductive yarn
#define TOUCH_PIN 4
#define TOUCH_PIN 0

int motorPin = 2;
int ledPin = 13; 

HardwareSerial mp3Serial(1); 
DFRobotDFPlayerMini myDFPlayer;

int threshold = 40; // Sensitivity treshold for conductive yarn

TFT_eSPI tft = TFT_eSPI();  // instance of the lcd display

// Green MAC Address 
uint8_t receiverMac[] = {0xE8, 0x31, 0xCD, 0x16, 0x20, 0xC8};

//Store touch value
int sentTouchValue;
int receivedTouchValue; 

// Structure to send touchValue
typedef struct struct_message {
    int number;
} struct_message;

//Save sensor reading
struct_message outgoingData;

//Save received sensor reading
struct_message incomingData;

// Callback function executed when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

void OnDataRecv(const uint8_t * mac, const uint8_t *receivedData, int len) {
  memcpy(&incomingData, receivedData, sizeof(incomingData));
  Serial.print("Bytes received: ");
  Serial.println(len);  
  receivedTouchValue = incomingData.number;
}

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging
  delay(1000);           // Wait for 1 second for the serial monitor to start

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
  }
  esp_now_register_send_cb(OnDataSent); 

  // Add the peer (receiver)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
  } else {
    Serial.println("Peer added successfully");
}

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));



  pinMode(motorPin, OUTPUT); // Initialize the motor control pin as an output
  pinMode(ledPin, OUTPUT);    // Initialize LED pin as output

  mp3Serial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println();
  Serial.println("Initializing DFPlayer...");

  if (!myDFPlayer.begin(mp3Serial)) {  // Start communication with DFPlayer
    Serial.println("DFPlayer Mini not detected or connected!");
    while (true);  // Stay here forever if initialization failed
  }
  Serial.println("DFPlayer Mini online.");

  tft.init();
  tft.setRotation(0);  

  // Set the background to white
  tft.fillScreen(TFT_WHITE);
  
  // Define the center of the circles
  int centerX1 = tft.width() / 2;
  int centerY1 = tft.height() / 2;
  int centerX2 = tft.width() / 2.8;
  int centerY2 = tft.height() / 2.8;

  // Define the radius of the circles
  int radius1 = 80;
  int radius2 = 30;

  // draw eyes
  tft.fillCircle(centerX1, centerY1, radius1, TFT_BLACK);
  tft.fillCircle(centerX2, centerY2, radius2, TFT_WHITE);

}

void loop() {
    // Read the touch sensor value
    sentTouchValue = touchRead(TOUCH_PIN);
    outgoingData.number = sentTouchValue;

    // Send message to receiver
    esp_err_t result = esp_now_send(receiverMac, (uint8_t *) &outgoingData, sizeof(outgoingData));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }

    // Check if received touch value is below threshold
    if (receivedTouchValue < threshold) {
        Serial.println("Conductive yarn touched!");
        digitalWrite(ledPin, HIGH);  // Turn the LED on
        digitalWrite(motorPin, HIGH); // Start Motor
        myDFPlayer.volume(10);  // Set volume (0 to 30)
        myDFPlayer.play(2);     // Play the first MP3 file on the SD card
    } else {
        Serial.println("No touch detected.");
        digitalWrite(ledPin, LOW);   // Turn the LED off
        digitalWrite(motorPin, LOW); // Stop Motor
    }

    delay(2000); // Send data every 2 seconds
}
























