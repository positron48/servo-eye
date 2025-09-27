#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <ArduinoOTA.h>
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

// OTA setup function
void setupOTA() {
  ArduinoOTA.setHostname("eye");
  ArduinoOTA.setPassword("12345678"); // Same as WiFi password
  
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  Serial.print("OTA Hostname: ");
  Serial.println(ArduinoOTA.getHostname());
  Serial.println("OTA Port: 8266");
}

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

  // Setup OTA
  setupOTA();

  // Setup web server routes
  webServer.setupRoutes();
  
  // Start server
  server.begin();
  
  Serial.println("Eye Controller ready!");
}

void loop() {
  // Handle OTA updates
  ArduinoOTA.handle();
  
  // Update servo controller
  servoController.update();
  
  // Handle web server
  server.handleClient();
  
  // Handle mDNS
  MDNS.update();
  
  delay(10);
}


