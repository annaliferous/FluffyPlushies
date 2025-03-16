#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

HardwareSerial mp3Serial(1);  // Use Serial1 for the ESP32 (TX: GPIO17, RX: GPIO16)
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);       // Serial monitor
  mp3Serial.begin(9600, SERIAL_8N1, 16, 17);  // Initialize Serial1 at 9600 baud (TX=GPIO17, RX=GPIO16)

  Serial.println();
  Serial.println("Initializing DFPlayer...");

  if (!myDFPlayer.begin(mp3Serial)) {  // Start communication with DFPlayer
    Serial.println("DFPlayer Mini not detected or connected!");
    while (true);  // Stay here forever if initialization failed
  }
  Serial.println("DFPlayer Mini online.");

  myDFPlayer.volume(10);  // Set volume (0 to 30)
  myDFPlayer.play(1);     // Play the first MP3 file on the SD card
}

void loop() {
  // You can add more control logic in the loop if needed.
}