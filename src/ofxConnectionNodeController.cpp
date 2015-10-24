//
//  ofxConnectionNodeController.cpp
//
//  Created by Eric Mika on 10/24/15.
//
//

#include "ofxConnectionNodeController.h"

ofxConnectionNodeController::ofxConnectionNodeController() {
  upVector.set(0, 1, 0);
  isInputEnabled = false;
  rotationSensitivity = 0.002;
  translationSensitivity = 0.005;
  isRotationEnabled = true;
  isTranslationEnabled = true;
  isRollEnabled = true;
  isPitchInverted = true;
  enableInput();
}

ofxConnectionNodeController::~ofxConnectionNodeController() {
  disableInput();
}

void ofxConnectionNodeController::enableInput() {
  if (!isInputEnabled) {
    isInputEnabled = true;
    ofxConnexion::start();
    ofAddListener(ofxConnexion::axisUpdateEvent, this, &ofxConnectionNodeController::onAxisUpdate);
    ofAddListener(ofxConnexion::buttonPressedEvent, this, &ofxConnectionNodeController::onButtonPressed);
    ofAddListener(ofxConnexion::buttonReleasedEvent, this, &ofxConnectionNodeController::onButtonReleased);
  }
}

void ofxConnectionNodeController::disableInput() {
  if (isInputEnabled) {
    isInputEnabled = false;
    ofRemoveListener(ofxConnexion::axisUpdateEvent, this, &ofxConnectionNodeController::onAxisUpdate);
    ofRemoveListener(ofxConnexion::buttonPressedEvent, this, &ofxConnectionNodeController::onButtonPressed);
    ofRemoveListener(ofxConnexion::buttonReleasedEvent, this, &ofxConnectionNodeController::onButtonReleased);
    ofxConnexion::stop();
  }
}

bool ofxConnectionNodeController::getInputEnabled() {
  return isInputEnabled;
}

void ofxConnectionNodeController::onButtonPressed(int &button) {
  // Left button resets camera
  if (button == 0) {
    targetNode->resetTransform();
  }
}

void ofxConnectionNodeController::onButtonReleased(int &button) {
  // Nothing yet...
}

void ofxConnectionNodeController::onAxisUpdate(ConnexionData &data) {
  if (isRotationEnabled) {
    updateRotation(data);
  }

  if (isTranslationEnabled) {
    updateTranslation(data);
  }
}

void ofxConnectionNodeController::updateTranslation(ConnexionData &data) {
  // clang-format off
  targetNode->move((targetNode->getXAxis() * (float)data.position[0] * translationSensitivity)
                  -(targetNode->getYAxis() * (float)data.position[2] * translationSensitivity)
                  +(targetNode->getZAxis() * (float)data.position[1] * translationSensitivity));
  // clang-format on
}

void ofxConnectionNodeController::updateRotation(ConnexionData &data) {
  // clang-format off
  targetNode->rotate(ofQuaternion((float)data.rotation[0] * rotationSensitivity * (isPitchInverted ? 1.0 : -1.0), targetNode->getXAxis(), // pitch
                                  (float)-data.rotation[2] * rotationSensitivity, isRollEnabled ? targetNode->getYAxis() : upVector,      // yaw
                                  isRollEnabled ? (float)data.rotation[1] * rotationSensitivity : 0.0, targetNode->getZAxis()));          // roll
  // clang-format on
}
