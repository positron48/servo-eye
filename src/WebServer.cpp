#include "WebServer.h"
#include "ui_html.h"
#include "ServoController.h"
#include "SettingsManager.h"

// Forward declarations
struct EyeState {
  float yaw;
  float pitch;
  bool animationRunning;
  String currentPreset;
  float speed;
  bool loop;
};

extern EyeState eyeState;
extern ServoController servoController;
extern SettingsManager settingsManager;

WebServer::WebServer(ESP8266WebServer* server) : server(server) {
}

void WebServer::setupRoutes() {
  // Serve main page
  server->on("/", [this]() {
    serveCompressedHTML(ui_html_data, ui_html_size);
  });

  // API endpoints
  server->on("/api/position", HTTP_POST, [this]() {
    if (server->hasArg("yaw") && server->hasArg("pitch")) {
      float yaw = server->arg("yaw").toFloat();
      float pitch = server->arg("pitch").toFloat();
      
      // Update global state
      eyeState.yaw = yaw;
      eyeState.pitch = pitch;
      
      // Update servo controller
      servoController.setPosition(yaw, pitch);
      
      server->send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      server->send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    }
  });

  server->on("/api/position", HTTP_GET, [this]() {
    JsonDocument doc;
    doc["yaw"] = eyeState.yaw;
    doc["pitch"] = eyeState.pitch;
    doc["animationRunning"] = eyeState.animationRunning;
    doc["currentPreset"] = eyeState.currentPreset;
    doc["speed"] = eyeState.speed;
    doc["loop"] = eyeState.loop;
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
  });

  server->on("/api/animation", HTTP_POST, [this]() {
    if (server->hasArg("action")) {
      String action = server->arg("action");
      
      if (action == "start") {
        if (server->hasArg("data")) {
          String data = server->arg("data");
          servoController.startAnimation(data);
          eyeState.animationRunning = true;
        }
      } else if (action == "stop") {
        servoController.stopAnimation();
        eyeState.animationRunning = false;
      }
      
      server->send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      server->send(400, "application/json", "{\"error\":\"Missing action parameter\"}");
    }
  });

  server->on("/api/settings", HTTP_POST, [this]() {
    if (server->hasArg("preset")) {
      eyeState.currentPreset = server->arg("preset");
    }
    if (server->hasArg("speed")) {
      eyeState.speed = server->arg("speed").toFloat();
    }
    if (server->hasArg("loop")) {
      eyeState.loop = server->arg("loop") == "true";
    }
    
    // Save settings
    settingsManager.saveSettings(eyeState.yaw, eyeState.pitch, eyeState.currentPreset, eyeState.speed, eyeState.loop);
    
    server->send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server->on("/api/settings", HTTP_GET, [this]() {
    JsonDocument doc;
    doc["preset"] = eyeState.currentPreset;
    doc["speed"] = eyeState.speed;
    doc["loop"] = eyeState.loop;
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
  });

  // OTA status endpoint
  server->on("/api/ota/status", HTTP_GET, [this]() {
    JsonDocument doc;
    doc["hostname"] = "eye";
    
    // Get IP address based on current mode
    if (WiFi.getMode() == WIFI_AP) {
      doc["ip"] = WiFi.softAPIP().toString();
      doc["mac"] = WiFi.softAPmacAddress();
      doc["mode"] = "AP";
    } else {
      doc["ip"] = WiFi.localIP().toString();
      doc["mac"] = WiFi.macAddress();
      doc["mode"] = "STA";
    }
    
    doc["version"] = "1.0.2"; // You can update this version number
    doc["uptime"] = millis() / 1000;
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["flashSize"] = ESP.getFlashChipSize();
    doc["flashUsed"] = ESP.getFlashChipSize() - ESP.getFreeSketchSpace();
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
  });

  // WiFi settings endpoints
  server->on("/api/wifi/settings", HTTP_GET, [this]() {
    const EyeSettings& settings = settingsManager.getSettings();
    JsonDocument doc;
    doc["useWiFi"] = settings.useWiFi;
    doc["ssid"] = settings.wifiSSID;
    doc["password"] = settings.wifiPassword;
    doc["currentMode"] = WiFi.getMode() == WIFI_AP ? "AP" : "STA";
    doc["connected"] = WiFi.status() == WL_CONNECTED;
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
  });

  server->on("/api/wifi/settings", HTTP_POST, [this]() {
    if (server->hasArg("ssid") && server->hasArg("password") && server->hasArg("useWiFi")) {
      String ssid = server->arg("ssid");
      String password = server->arg("password");
      bool useWiFi = server->arg("useWiFi") == "true";
      
      settingsManager.saveWiFiSettings(ssid, password, useWiFi);
      
      server->send(200, "application/json", "{\"status\":\"ok\",\"message\":\"WiFi settings saved. Device will restart to apply changes.\"}");
      
      // Restart after a short delay to allow response to be sent
      delay(1000);
      ESP.restart();
    } else {
      server->send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    }
  });
}

void WebServer::serveCompressedHTML(const char* htmlData, size_t htmlSize) {
  // Send HTML directly - no decoding needed
  server->send(200, "text/html", htmlData);
}