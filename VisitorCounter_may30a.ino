#include <Ultrasonic.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "thingProperties.h"

// Define ultrasonic sensor pins
#define TRIG_PIN_1 4
#define ECHO_PIN_1 2
#define TRIG_PIN_2 32
#define ECHO_PIN_2 33

// Define OLED screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define buzzer and relay pins
#define BUZZER_PIN 19
#define RELAY_PIN 18

// Define cooldown period in milliseconds
#define COOLDOWN_PERIOD 2000 // Adjust as needed

// Create ultrasonic sensor objects
Ultrasonic ultrasonic1(TRIG_PIN_1, ECHO_PIN_1);
Ultrasonic ultrasonic2(TRIG_PIN_2, ECHO_PIN_2);

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variables to track state
bool objectDetected1 = false;
bool objectDetected2 = false;
unsigned long lastDetectionTime1 = 0;
unsigned long lastDetectionTime2 = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Initialize buzzer and relay pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize Arduino Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Measure distances
  long distance1 = ultrasonic1.read();
  long distance2 = ultrasonic2.read();

  // Check for entry (assuming a threshold distance of 10 cm)
  if (distance1 < 10 && !objectDetected1 && millis() - lastDetectionTime1 > COOLDOWN_PERIOD) {
    objectDetected1 = true;
    if (distance2 >= 10) {
      inCount++;
      insideCount++;
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      lastDetectionTime1 = millis(); // Update last detection time
    }
  } else if (distance1 >= 10) {
    objectDetected1 = false;
  }

  // Check for exit
  if (distance2 < 10 && !objectDetected2 && millis() - lastDetectionTime2 > COOLDOWN_PERIOD) {
    objectDetected2 = true;
    if (distance1 >= 10 && insideCount > 0 && outCount <= inCount) {
      outCount++;
      insideCount--;
      digitalWrite(RELAY_PIN, HIGH);
      delay(100);
      digitalWrite(RELAY_PIN, LOW);
      lastDetectionTime2 = millis(); // Update last detection time
    }
  } else if (distance2 >= 10) {
    objectDetected2 = false;
  }

  // Reset cooldown periods if both sensors detect visitors simultaneously
  if (distance1 < 10 && distance2 < 10) {
    lastDetectionTime1 = millis();
    lastDetectionTime2 = millis();
  }

  // Display counts on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("In: ");
  display.println(inCount);
  display.setCursor(0, 20);
  display.print("Out: ");
  display.println(outCount);
  display.setCursor(0, 40);
  display.print("Inside: ");
  display.println(insideCount);
  display.display();
}

void onInCountChange() {
  // No need to update inCount here, it is automatically updated by the IoT Cloud
}

void onOutCountChange() {
  // No need to update outCount here, it is automatically updated by the IoT Cloud
}

void onInsideCountChange() {
  // No need to update insideCount here, it is automatically updated by the IoT Cloud
}
