#include "ServoController.h"

ServoController::ServoController() 
  : currentYaw(0.0), currentPitch(0.0), animationRunning(false),
    animationFrames(nullptr), frameCount(0), currentFrame(0),
    lastMovementTime(0), servosDisabled(false), minYaw(-60.0), maxYaw(60.0), minPitch(-60.0), maxPitch(60.0) {
}

ServoController::~ServoController() {
  if (animationFrames) {
    delete[] animationFrames;
  }
}

void ServoController::begin() {
  yawServo.attach(YAW_PIN);
  pitchServo.attach(PITCH_PIN);
  
  // Set initial position (center)
  setPosition(0.0, 0.0);
  
  Serial.println("ServoController initialized");
}

void ServoController::update() {
  unsigned long currentTime = millis();
  
  // Check for servo timeout (disable servos after 200ms of no movement)
  // This works even during animation - if position doesn't change, servos will be disabled
  if (!servosDisabled && (currentTime - lastMovementTime) >= (unsigned long)SERVO_TIMEOUT_MS) {
    disableServos();
  }
  
  if (!animationRunning || !animationFrames || frameCount == 0) {
    return;
  }
  
  unsigned long elapsed = currentTime - frameStartTime;
  
  AnimationFrame& current = animationFrames[currentFrame];
  
  if (elapsed >= current.duration) {
    // Move to next frame
    currentFrame++;
    frameStartTime = currentTime;
    
    if (currentFrame >= frameCount) {
      // Animation finished, restart from beginning
      currentFrame = 0;
    }
    
    // Set position for new frame
    setPosition(current.yaw, current.pitch);
  } else {
    // Get previous frame for comparison
    AnimationFrame& prev = animationFrames[(currentFrame - 1 + frameCount) % frameCount];
    float progress = (float)elapsed / current.duration;
    
    // Check if current position is the same as previous (for hold points)
    bool isHoldPoint = (abs(current.yaw - prev.yaw) < 0.1f && abs(current.pitch - prev.pitch) < 0.1f);
    
    if (isHoldPoint) {
      // For hold points, just set the position directly (no interpolation)
      setPosition(current.yaw, current.pitch);
    } else {
      // For movement points, use interpolation from previous to current
      float interpolatedYaw = prev.yaw + (current.yaw - prev.yaw) * progress;
      float interpolatedPitch = prev.pitch + (current.pitch - prev.pitch) * progress;
      
      setPosition(interpolatedYaw, interpolatedPitch);
    }
  }
}

void ServoController::setPosition(float yaw, float pitch) {
  // Clamp values to limits
  float newYaw = constrain(yaw, minYaw, maxYaw);
  float newPitch = constrain(pitch, minPitch, maxPitch);
  
  // Only update if position actually changed (more than 0.1 degrees)
  if (abs(newYaw - currentYaw) > 0.1f || abs(newPitch - currentPitch) > 0.1f) {
    currentYaw = newYaw;
    currentPitch = newPitch;
    
    // Update last movement time and mark servos as active
    lastMovementTime = millis();
    servosDisabled = false;
    
    updateServos();
  }
}

void ServoController::setLimits(float minYaw, float maxYaw, float minPitch, float maxPitch) {
  this->minYaw = minYaw;
  this->maxYaw = maxYaw;
  this->minPitch = minPitch;
  this->maxPitch = maxPitch;
  
  // Re-constrain current position
  setPosition(currentYaw, currentPitch);
}

void ServoController::disableServos() {
  // Detach servos to remove power
  yawServo.detach();
  pitchServo.detach();
  
  servosDisabled = true;
  Serial.println("Servos disabled (power removed)");
}

void ServoController::startAnimation(const String& data) {
  stopAnimation();
  parseAnimationData(data);
  
  if (frameCount > 0 && animationFrames) {
    animationRunning = true;
    currentFrame = 0;
    frameStartTime = millis();
    
    // Set initial position
    setPosition(animationFrames[0].yaw, animationFrames[0].pitch);
    
    Serial.println("Animation started with " + String(frameCount) + " frames");
  }
}

void ServoController::stopAnimation() {
  animationRunning = false;
  
  if (animationFrames) {
    delete[] animationFrames;
    animationFrames = nullptr;
  }
  
  frameCount = 0;
  currentFrame = 0;
  
  Serial.println("Animation stopped");
}

float ServoController::mapAngleToPWM(float angle, float minAngle, float maxAngle) {
  // Map angle to PWM value (500-2500 microseconds)
  // Assuming servo range is 180 degrees
  float normalized = (angle - minAngle) / (maxAngle - minAngle);
  return 500 + normalized * 2000; // 500-2500 microseconds
}

void ServoController::updateServos() {
  // Reattach servos if they were detached
  if (yawServo.attached() == 0U) {
    yawServo.attach(YAW_PIN);
  }
  if (pitchServo.attached() == 0U) {
    pitchServo.attach(PITCH_PIN);
  }
  
  // Mark servos as active if they were reattached
  if (servosDisabled && (yawServo.attached() != 0U || pitchServo.attached() != 0U)) {
    servosDisabled = false;
  }
  
  // Convert angles to PWM values
  float yawPWM = mapAngleToPWM(currentYaw, minYaw, maxYaw);
  // Invert pitch servo by swapping min and max angles
  float pitchPWM = mapAngleToPWM(currentPitch, maxPitch, minPitch);
  
  // Write to servos
  yawServo.writeMicroseconds((int)yawPWM);
  pitchServo.writeMicroseconds((int)pitchPWM);
  
  Serial.println("Position: Yaw=" + String(currentYaw, 1) + "°, Pitch=" + String(currentPitch, 1) + "°");
}

void ServoController::parseAnimationData(const String& data) {
  // Clear existing animation
  if (animationFrames) {
    delete[] animationFrames;
    animationFrames = nullptr;
  }
  
  // Count lines
  frameCount = 0;
  int start = 0;
  while (start < data.length()) {
    int end = data.indexOf('\n', start);
    if (end == -1) end = data.length();
    
    String line = data.substring(start, end);
    line.trim();
    
    if (line.length() > 0) {
      frameCount++;
    }
    
    start = end + 1;
  }
  
  if (frameCount == 0) {
    return;
  }
  
  // Allocate memory for frames
  animationFrames = new AnimationFrame[frameCount];
  
  // Parse lines
  int frameIndex = 0;
  start = 0;
  while (start < data.length() && frameIndex < frameCount) {
    int end = data.indexOf('\n', start);
    if (end == -1) end = data.length();
    
    String line = data.substring(start, end);
    line.trim();
    
    if (line.length() > 0) {
      // Parse CSV format: yaw,pitch,duration
      int comma1 = line.indexOf(',');
      int comma2 = line.indexOf(',', comma1 + 1);
      
      if (comma1 > 0 && comma2 > comma1) {
        animationFrames[frameIndex].yaw = line.substring(0, comma1).toFloat();
        animationFrames[frameIndex].pitch = line.substring(comma1 + 1, comma2).toFloat();
        animationFrames[frameIndex].duration = line.substring(comma2 + 1).toInt();
        
        frameIndex++;
      }
    }
    
    start = end + 1;
  }
  
  frameCount = frameIndex;
  
  Serial.println("Parsed " + String(frameCount) + " animation frames");
}
