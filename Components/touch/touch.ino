// Define the touch pin connected to conductive yarn
#define TOUCH_PIN 4

int ledPin = 13;      // GPIO for LED

// Sensitivity threshold for detecting a touch
int threshold = 40;

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging
  delay(1000);           // Wait for 1 second for the serial monitor to start

  pinMode(ledPin, OUTPUT);    // Initialize LED pin as output
}

void loop() {
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
