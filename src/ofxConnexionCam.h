#pragma once

#include "ofMain.h"
#include "ofxConnexion.h"

class ofxConnexionCam : public ofCamera {
public:
  ofxConnexionCam();
  ~ofxConnexionCam();

  void enableInput();
  void disableInput();
  bool getInputEnabled();

  bool isTranslationEnabled;
  bool isRotationEnabled;
  bool isRollEnabled; // Keeps "up" up

  float rotationSensitivity;
  float translationSensitivity;

protected:
  // Events from the device
  void onAxisUpdate(ConnexionData &data);
  void onButtonPressed(int &button);
  void onButtonReleased(int &button);

  void updateRotation(ConnexionData &data);
  void updateTranslation(ConnexionData &data);

  bool isInputEnabled;
  ofVec3f upVector;
};
