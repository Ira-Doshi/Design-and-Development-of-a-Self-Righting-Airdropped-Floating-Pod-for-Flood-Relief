#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Motor pins
const int leftMotorPin = 13;
const int rightMotorPin = 12;

// Servo objects
Servo leftMotor;
Servo rightMotor;

// Servo values for continuous rotation (tweak as needed)
const int STOP = 90;        // Neutral position (no rotation)
const int FORWARD = 60;     // Forward rotation
const int BACKWARD = 120;   // Backward rotation

// Function to control motors based on received command
void handleCommand(const String& command) {
  if (command == "Forward") {
    leftMotor.write(FORWARD);
    rightMotor.write(FORWARD);
    Serial.println("Moving Forward");
  } else if (command == "Backward") {
    leftMotor.write(BACKWARD);
    rightMotor.write(BACKWARD);
    Serial.println("Moving Backward");
  } else if (command == "Left") {
    leftMotor.write(BACKWARD);
    rightMotor.write(FORWARD);
    Serial.println("Turning Left");
  } else if (command == "Right") {
    leftMotor.write(FORWARD);
    rightMotor.write(BACKWARD);
    Serial.println("Turning Right");
  } else {
    // Optional: stop motors on unknown command
    leftMotor.write(STOP);
    rightMotor.write(STOP);
    Serial.println("Stopping");
  }
}

// New-style ESP-NOW receive callback
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  String receivedCommand = "";
  for (int i = 0; i < len; i++) {
    receivedCommand += (char)data[i];
  }

  Serial.print("Received Command: ");
  Serial.println(receivedCommand);

  handleCommand(receivedCommand);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  // Attach motors to PWM-capable pins
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);

  // Initialize motors in stop position
  leftMotor.write(STOP);
  rightMotor.write(STOP);

  Serial.println("Receiver ready, motors attached.");
}

void loop() {
  // Nothing to do here
}
