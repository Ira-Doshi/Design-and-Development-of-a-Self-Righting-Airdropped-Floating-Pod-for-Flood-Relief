#include <WiFi.h>
#include <esp_now.h>

// Define GPIO pins for buttons
const int buttonForward = 12;
const int buttonBackward = 13;
const int buttonRight = 15;
const int buttonLeft = 2;

// Receiver MAC address
uint8_t receiverMAC[] = {0x44, 0x1D, 0x64, 0xF2, 0xF6, 0x30};

// Last sent command to avoid repeating same command
String lastCommand = "";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Required for ESP-NOW

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize buttons with internal pull-ups
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonBackward, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);

  Serial.println("Sender ready");
}

void loop() {
  String command = "";

  if (digitalRead(buttonForward) == HIGH) {
    command = "Forward";
  } else if (digitalRead(buttonBackward) == HIGH) {
    command = "Backward";
  } else if (digitalRead(buttonRight) == HIGH) {
    command = "Right";
  } else if (digitalRead(buttonLeft) == HIGH) {
    command = "Left";
  } else {
    command = "Stop";
  }

  // Only send if the command has changed
  if (command != lastCommand) {
    esp_now_send(receiverMAC, (uint8_t *)command.c_str(), command.length());
    Serial.print("Sent: ");
    Serial.println(command);
    lastCommand = command;
  }

  delay(50);  // Small delay for stability
}


/*#include <WiFi.h>
#include <esp_now.h>

// Define GPIO pins for buttons
const int buttonForward = 12;
const int buttonBackward = 13;
const int buttonRight = 15;
const int buttonLeft = 2;

// Replace this with the actual MAC address of the receiver ESP32
uint8_t receiverMAC[] = {0x44, 0x1D, 0x64, 0xF2, 0xF6, 0x30};

// Function to send messages
void sendCommand(const char* command) {
  esp_now_send(receiverMAC, (uint8_t *)command, strlen(command));
  Serial.print("Sent: ");
  Serial.println(command);
  delay(300);  // Debounce delay
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Set WiFi to Station mode (required for ESP-NOW)

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize buttons with internal pull-up
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonBackward, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);

  Serial.println("ESP32 Button Sender Ready");
}

void loop() {
  if (digitalRead(buttonForward) == HIGH) {
    sendCommand("Forward");
  } else if (digitalRead(buttonBackward) == HIGH) {
    sendCommand("Backward");
  } else if (digitalRead(buttonRight) == HIGH) {
    sendCommand("Right");
  } else if (digitalRead(buttonLeft) == HIGH) {
    sendCommand("Left");
  }
}
*/
