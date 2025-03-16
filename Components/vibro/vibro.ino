#include <TFT_eSPI.h>  // Graphics and font library for ST7789 driver

// Define the touch pin connected to conductive yarn
#define TOUCH_PIN 4

int motorPin = 2; 

int ledPin = 13;      // GPIO for LED
// Sensitivity threshold for detecting a touch
int threshold = 80;

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display

void setup() {
  pinMode(ledPin, OUTPUT);    // Initialize LED pin as output
  pinMode(motorPin, OUTPUT); // Initialize the motor control pin as an output
  Serial.begin(115200);  // Start serial communication for debugging
  delay(1000);           // Wait for 1 second for the serial monitor to start

  tft.init();
  tft.setRotation(0);  // Adjust rotation if necessary

  
  
  
  // Set the background to white
  tft.fillScreen(TFT_WHITE);
  
  // Define the center of the display
  int centerX1 = tft.width() / 2;
  int centerY1 = tft.height() / 2;
  int centerX2 = tft.width() / 2.8;
  int centerY2 = tft.height() / 2.8;

  // Define the radius of the circle (adjust as needed)
  int radius1 = 80;
  int radius2 = 30;

  // Draw a black circle in the middle of the screen
  
  tft.fillCircle(centerX1, centerY1, radius1, TFT_BLACK);
  tft.fillCircle(centerX2, centerY2, radius2, TFT_WHITE);

   // Now fill the bottom quarter of the screen with blue
  int screenHeight = tft.height();
  int screenWidth = tft.width();
  //int bottomQuarterY = screenHeight * 3 / 4;  // Start at 75% of screen height
  int bottomQuarterHeight = screenHeight / 4; // Bottom 25% of the screen

  // Fill the bottom quarter with blue
  tft.fillRect(0, 0, screenWidth, bottomQuarterHeight, TFT_BLUE);

  // Read touch sensor value from the conductive yarn
  int touchValue = touchRead(TOUCH_PIN);
  
  // Print the touch value to the serial monitor for debugging
  Serial.print("Touch Value: ");
  Serial.println(touchValue);
  
  // Check if the touch value is below the threshold
  if (touchValue < threshold) {
    // If below threshold, it means the yarn was touched
    Serial.println("Conductive yarn touched!");
    digitalWrite(ledPin, HIGH);  // Turn the LED on
  } else {
    // If above threshold, no touch is detected
    Serial.println("No touch detected.");
    digitalWrite(ledPin, LOW);   // Turn the LED off
  }
  
  // Delay for stability and to make the readings more readable
  delay(500);

}

void loop() {
  
}