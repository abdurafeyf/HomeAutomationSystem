#include <dht.h>
#include <LiquidCrystal.h>

// Initialize LCD with pin configuration
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
dht DHT;

// Pin and threshold definitions
#define DHT11_PIN 6
int smokeSensorPin = A0;
int temperatureThreshold = 35; // Minimum temperature to trigger the action
int humidityThreshold = 24; // Minimum humidity to trigger the action
int smokeThreshold = 300; // Smoke level to trigger the action

// Relays connected to specific pins
#define coolingRelay 7  // Cooling system relay
#define smokeRelay 4    // Smoke alarm relay
#define humidifierRelay 5  // Humidifier relay

// Flag to toggle control modes
boolean controlFlag = true;

// Input command storage for app communication
String appInput;

void setup() {
  Serial.begin(9600); // Serial communication for app interaction
  lcd.begin(16, 2);   // Initialize the LCD
  pinMode(smokeSensorPin, INPUT); // Set smoke sensor pin as input

  // Initialize relays as outputs and set to OFF initially
  pinMode(coolingRelay, OUTPUT);
  pinMode(smokeRelay, OUTPUT);
  pinMode(humidifierRelay, OUTPUT);
  digitalWrite(coolingRelay, LOW);
  digitalWrite(smokeRelay, LOW);
  digitalWrite(humidifierRelay, LOW);
}

void loop() {
  // Read temperature, humidity, and smoke sensor values
  int chk = DHT.read11(DHT11_PIN);
  int smokeLevel = analogRead(smokeSensorPin);

  // Automatic mode operations based on sensor values
  if (controlFlag) {
    // Control cooling system based on temperature
    if (DHT.temperature >= temperatureThreshold) {
      digitalWrite(coolingRelay, LOW); // Turn off cooling
    } else {
      digitalWrite(coolingRelay, HIGH); // Turn on cooling
    }

    // Control humidifier based on humidity
    if (DHT.humidity <= humidityThreshold) {
      digitalWrite(humidifierRelay, HIGH); // Turn on humidifier
    } else {
      digitalWrite(humidifierRelay, LOW); // Turn off humidifier
    }

    // Trigger smoke alarm based on smoke level
    if (smokeLevel >= smokeThreshold) {
      digitalWrite(smokeRelay, HIGH); // Activate smoke alarm
    } else {
      digitalWrite(smokeRelay, LOW); // Deactivate smoke alarm
    }
  }

  // Display sensor data on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(int(DHT.temperature));
  lcd.print("C");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  lcd.print(int(DHT.humidity));
  lcd.print("%");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("Smoke:");
  lcd.print(smokeLevel);
  delay(1000);
  lcd.clear();

  // Handle app-based commands
  while (Serial.available()) {
    delay(10);
    char c = Serial.read();
    if (c == '#') break; // End command input when '#' is detected
    appInput += c;
  }

  if (appInput.length() > 0) {
    controlFlag = false; // Disable automatic control during manual input
    Serial.println(appInput);

    if (appInput == "A") {
      digitalWrite(coolingRelay, LOW); // Turn off cooling system
    } else if (appInput == "a") {
      digitalWrite(coolingRelay, HIGH); // Turn on cooling system
    } else if (appInput == "C") {
      digitalWrite(smokeRelay, HIGH); // Activate smoke alarm
    } else if (appInput == "c") {
      digitalWrite(smokeRelay, LOW); // Deactivate smoke alarm
    } else if (appInput == "D") {
      digitalWrite(humidifierRelay, LOW); // Turn off humidifier
    } else if (appInput == "d") {
      digitalWrite(humidifierRelay, HIGH); // Turn on humidifier
    } else if (appInput == "E" || appInput == "e") {
      controlFlag = true; // Re-enable automatic control
    }

    appInput = ""; // Clear input for the next command
  }
}
