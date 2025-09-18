#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "ServoController.h"
#include "SettingsManager.h"
#include "WebServer.h"

// WiFi AP settings
const char* ssid = "EyeController";
const char* password = "12345678";

// Web server
ESP8266WebServer server(80);
WebServer webServer(&server);

// Servo controller
ServoController servoController;

// Settings manager
SettingsManager settingsManager;

// Current state
struct EyeState {
  float yaw = 0.0;
  float pitch = 0.0;
  bool animationRunning = false;
  String currentPreset = "Center Sweep";
  float speed = 1.0;
  bool loop = true;
} eyeState;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Eye Controller...");

  // Initialize EEPROM
  EEPROM.begin(512);
  
  // Initialize servo controller
  servoController.begin();
  
  // Load saved settings
  settingsManager.loadSettings();
  
  // Apply loaded settings to eye state
  const EyeSettings& settings = settingsManager.getSettings();
  eyeState.yaw = settings.yaw;
  eyeState.pitch = settings.pitch;
  eyeState.currentPreset = settings.preset;
  eyeState.speed = settings.speed;
  eyeState.loop = settings.loop;
  
  // Setup WiFi AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup mDNS for eye.local
  if (MDNS.begin("eye")) {
    Serial.println("mDNS responder started");
  }

  // Setup web server routes
  webServer.setupRoutes();
  
  // Start server
  server.begin();
  
  Serial.println("Eye Controller ready!");
}

void loop() {
  // Update servo controller
  servoController.update();
  
  // Handle web server
  server.handleClient();
  
  // Handle mDNS
  MDNS.update();
  
  delay(10);
}


