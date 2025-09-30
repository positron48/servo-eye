#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

struct AnimationFrame {
  float yaw;
  float pitch;
  int duration; // milliseconds
};

class ServoController {
private:
  Servo yawServo;
  Servo pitchServo;
  
  // Pin definitions
  static const int YAW_PIN = D2;
  static const int PITCH_PIN = D1;
  
  // Current position
  float currentYaw;
  float currentPitch;
  
  // Animation state
  bool animationRunning;
  AnimationFrame* animationFrames;
  int frameCount;
  int currentFrame;
  unsigned long frameStartTime;
  
  // Servo timeout management
  unsigned long lastMovementTime;
  static const unsigned long SERVO_TIMEOUT_MS = 200; // Reduced from 300ms to 200ms
  bool servosDisabled;
  
  // Limits (для валидации на фронтенде)
  float minYaw;
  float maxYaw;
  float minPitch;
  float maxPitch;
  
  // Physical servo range (полный диапазон для маппинга на PWM)
  static constexpr float SERVO_ANGLE_MIN = -60.0f;
  static constexpr float SERVO_ANGLE_MAX = 60.0f;
  
  // Helper functions
  float mapAngleToPWM(float angle);
  void updateServos();
  void parseAnimationData(const String& data);
  
public:
  ServoController();
  ~ServoController();
  
  void begin();
  void update();
  
  void setPosition(float yaw, float pitch);
  void setLimits(float minYaw, float maxYaw, float minPitch, float maxPitch);
  void disableServos();
  
  void startAnimation(const String& data);
  void stopAnimation();
  
  float getCurrentYaw() const { return currentYaw; }
  float getCurrentPitch() const { return currentPitch; }
  bool isAnimationRunning() const { return animationRunning; }
  
  // Геттеры для ограничений (используются на фронтенде)
  float getMinYaw() const { return minYaw; }
  float getMaxYaw() const { return maxYaw; }
  float getMinPitch() const { return minPitch; }
  float getMaxPitch() const { return maxPitch; }
};

#endif
