//  Created by Eric Mika on 10/24/15.

#pragma once

#include "ofxConnexion.h"
#include "ofMain.h"

class ofxConnectionNodeController {
public:
  ofxConnectionNodeController();
  ~ofxConnectionNodeController();

  ofNode *targetNode;

  void enableInput();
  void disableInput();
  bool getInputEnabled();

  bool isTranslationEnabled;
  bool isRotationEnabled;
  bool isRollEnabled;   // Keeps "up" up
  bool isPitchInverted; // Controversial

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
