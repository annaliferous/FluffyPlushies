#include <TFT_eSPI.h>  // Include the TFT_eSPI library

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

void setup() {
  tft.begin();               // Initialize the TFT display
  tft.setRotation(0);        // Set the rotation of the screen if needed
  tft.fillScreen(TFT_WHITE); // Clear the screen with a white background
  
  drawHeart(64, 64, 80);    // Draw a heart at (100, 80) with a size of 50
}

void loop() {
  // Nothing to do here
}

// Function to draw a heart
void drawHeart(int x, int y, int size) {
  
  // Draw the two top arcs (left and right)
  tft.fillCircle(x - size / 2, y - size / 2, size / 2, TFT_RED);  // Left circle
  tft.fillCircle(x + size / 2, y - size / 2, size / 2, TFT_RED);  // Right circle
  
  // Draw the bottom triangle
  int triangleHeight = size;
  int triangleBase = size;
  tft.fillTriangle(x - triangleBase / 2, y, 
                   x + triangleBase / 2, y, 
                   x, y + triangleHeight, 
                   TFT_RED);
}
