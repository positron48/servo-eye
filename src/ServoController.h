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
  static const int YAW_PIN = D1;
  static const int PITCH_PIN = D2;
  
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
  
  // Limits
  float minYaw;
  float maxYaw;
  float minPitch;
  float maxPitch;
  
  // Helper functions
  float mapAngleToPWM(float angle, float minAngle, float maxAngle);
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
};

#endif
