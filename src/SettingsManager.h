#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

struct EyeSettings {
  String preset;
  float speed;
  bool loop;
  float yaw;
  float pitch;
  float minYaw;
  float maxYaw;
  float minPitch;
  float maxPitch;
};

class SettingsManager {
private:
  static constexpr int EEPROM_SIZE = 512;
  static constexpr int SETTINGS_ADDRESS = 0;
  static constexpr int MAGIC_NUMBER = 0x12345678;
  
  EyeSettings settings;
  
  void writeStringToEEPROM(int address, const String& str);
  String readStringFromEEPROM(int address, int maxLength);
  void writeFloatToEEPROM(int address, float value);
  float readFloatFromEEPROM(int address);
  void writeBoolToEEPROM(int address, bool value);
  bool readBoolFromEEPROM(int address);
  
public:
  SettingsManager();
  
  void loadSettings();
  void saveSettings(const EyeSettings& newSettings);
  void saveSettings(float yaw, float pitch, const String& preset, float speed, bool loop);
  
  const EyeSettings& getSettings() const { return settings; }
};

#endif
