#include "SettingsManager.h"

SettingsManager::SettingsManager() {
  // Initialize default settings
  settings.preset = "Center Sweep";
  settings.speed = 1.0;
  settings.loop = true;
  settings.yaw = 0.0;
  settings.pitch = 0.0;
  settings.minYaw = -60.0;
  settings.maxYaw = 60.0;
  settings.minPitch = -60.0;
  settings.maxPitch = 60.0;
  // WiFi settings defaults
  settings.wifiSSID = "";
  settings.wifiPassword = "";
  settings.useWiFi = false;
}

void SettingsManager::loadSettings() {
  // Check magic number
  int magic = 0;
  EEPROM.get(SETTINGS_ADDRESS, magic);
  
  if (magic != MAGIC_NUMBER) {
    Serial.println("No valid settings found, using defaults");
    return;
  }
  
  int address = SETTINGS_ADDRESS + sizeof(int);
  
  // Load settings
  settings.preset = readStringFromEEPROM(address, 50);
  address += 50;
  
  settings.speed = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.loop = readBoolFromEEPROM(address);
  address += sizeof(bool);
  
  settings.yaw = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.pitch = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.minYaw = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.maxYaw = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.minPitch = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  settings.maxPitch = readFloatFromEEPROM(address);
  address += sizeof(float);
  
  // Load WiFi settings
  settings.wifiSSID = readStringFromEEPROM(address, 50);
  address += 50;
  
  settings.wifiPassword = readStringFromEEPROM(address, 50);
  address += 50;
  
  settings.useWiFi = readBoolFromEEPROM(address);
  
  Serial.println("Settings loaded from EEPROM");
  Serial.println("Preset: " + settings.preset);
  Serial.println("Speed: " + String(settings.speed));
  Serial.println("Loop: " + String(settings.loop));
  Serial.println("Position: Yaw=" + String(settings.yaw) + ", Pitch=" + String(settings.pitch));
  Serial.println("WiFi: " + String(settings.useWiFi ? "Enabled" : "Disabled"));
  if (settings.useWiFi) {
    Serial.println("WiFi SSID: " + settings.wifiSSID);
  }
}

void SettingsManager::saveSettings(const EyeSettings& newSettings) {
  settings = newSettings;
  saveSettings(settings.yaw, settings.pitch, settings.preset, settings.speed, settings.loop);
}

void SettingsManager::saveSettings(float yaw, float pitch, const String& preset, float speed, bool loop) {
  int address = SETTINGS_ADDRESS;
  
  // Write magic number
  EEPROM.put(address, MAGIC_NUMBER);
  address += sizeof(int);
  
  // Write settings
  writeStringToEEPROM(address, preset);
  address += 50;
  
  writeFloatToEEPROM(address, speed);
  address += sizeof(float);
  
  writeBoolToEEPROM(address, loop);
  address += sizeof(bool);
  
  writeFloatToEEPROM(address, yaw);
  address += sizeof(float);
  
  writeFloatToEEPROM(address, pitch);
  address += sizeof(float);
  
  writeFloatToEEPROM(address, settings.minYaw);
  address += sizeof(float);
  
  writeFloatToEEPROM(address, settings.maxYaw);
  address += sizeof(float);
  
  writeFloatToEEPROM(address, settings.minPitch);
  address += sizeof(float);
  
  writeFloatToEEPROM(address, settings.maxPitch);
  address += sizeof(float);
  
  // Save WiFi settings
  writeStringToEEPROM(address, settings.wifiSSID);
  address += 50;
  
  writeStringToEEPROM(address, settings.wifiPassword);
  address += 50;
  
  writeBoolToEEPROM(address, settings.useWiFi);
  
  EEPROM.commit();
  
  Serial.println("Settings saved to EEPROM");
}

void SettingsManager::writeStringToEEPROM(int address, const String& str) {
  int len = str.length();
  if (len > 49) len = 49; // Max 49 chars + null terminator
  
  for (int i = 0; i < len; i++) {
    EEPROM.write(address + i, str[i]);
  }
  EEPROM.write(address + len, '\0'); // Null terminator
}

String SettingsManager::readStringFromEEPROM(int address, int maxLength) {
  String result = "";
  
  for (int i = 0; i < maxLength - 1; i++) {
    char c = EEPROM.read(address + i);
    if (c == '\0') break;
    result += c;
  }
  
  return result;
}

void SettingsManager::writeFloatToEEPROM(int address, float value) {
  EEPROM.put(address, value);
}

float SettingsManager::readFloatFromEEPROM(int address) {
  float value;
  EEPROM.get(address, value);
  return value;
}

void SettingsManager::writeBoolToEEPROM(int address, bool value) {
  EEPROM.write(address, value ? 1 : 0);
}

bool SettingsManager::readBoolFromEEPROM(int address) {
  return EEPROM.read(address) != 0;
}

void SettingsManager::saveLimits(float minYaw, float maxYaw, float minPitch, float maxPitch) {
  settings.minYaw = minYaw;
  settings.maxYaw = maxYaw;
  settings.minPitch = minPitch;
  settings.maxPitch = maxPitch;
  
  // Save all settings to EEPROM
  saveSettings(settings.yaw, settings.pitch, settings.preset, settings.speed, settings.loop);
  
  Serial.println("Limits saved:");
  Serial.println("Yaw: " + String(minYaw) + " to " + String(maxYaw));
  Serial.println("Pitch: " + String(minPitch) + " to " + String(maxPitch));
}

void SettingsManager::saveWiFiSettings(const String& ssid, const String& password, bool useWiFi) {
  settings.wifiSSID = ssid;
  settings.wifiPassword = password;
  settings.useWiFi = useWiFi;
  
  // Save all settings to EEPROM
  saveSettings(settings.yaw, settings.pitch, settings.preset, settings.speed, settings.loop);
  
  Serial.println("WiFi settings saved:");
  Serial.println("Use WiFi: " + String(useWiFi ? "Yes" : "No"));
  if (useWiFi) {
    Serial.println("SSID: " + ssid);
  }
}
