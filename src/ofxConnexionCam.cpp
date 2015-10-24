//  ofxConnexionCam.cpp
//  Created by TAKAHASHI Tomohiro on 5/29/13.

#include "ofxConnexionCam.h"

ofxConnexionCam::ofxConnexionCam() {
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

ofxConnexionCam::~ofxConnexionCam() {
  disableInput();
}

void ofxConnexionCam::enableInput() {
  if (!isInputEnabled) {
    isInputEnabled = true;
    ofxConnexion::start();
    ofAddListener(ofxConnexion::axisUpdateEvent, this, &ofxConnexionCam::onAxisUpdate);
    ofAddListener(ofxConnexion::buttonPressedEvent, this, &ofxConnexionCam::onButtonPressed);
    ofAddListener(ofxConnexion::buttonReleasedEvent, this, &ofxConnexionCam::onButtonReleased);
  }
}

void ofxConnexionCam::disableInput() {
  if (isInputEnabled) {
    isInputEnabled = false;
    ofRemoveListener(ofxConnexion::axisUpdateEvent, this, &ofxConnexionCam::onAxisUpdate);
    ofRemoveListener(ofxConnexion::buttonPressedEvent, this, &ofxConnexionCam::onButtonPressed);
    ofRemoveListener(ofxConnexion::buttonReleasedEvent, this, &ofxConnexionCam::onButtonReleased);
    ofxConnexion::stop();
  }
}

bool ofxConnexionCam::getInputEnabled() {
  return isInputEnabled;
}

void ofxConnexionCam::onButtonPressed(int &button) {
  // Left button resets camera
  if (button == 0) {
    resetTransform();
  }
}

void ofxConnexionCam::onButtonReleased(int &button) {
  // Nothing yet...
}

void ofxConnexionCam::onAxisUpdate(ConnexionData &data) {
  if (isRotationEnabled) {
    updateRotation(data);
  }

  if (isTranslationEnabled) {
    updateTranslation(data);
  }
}

void ofxConnexionCam::updateTranslation(ConnexionData &data) {
  // clang-format off
  move((getXAxis() * (float)data.position[0] * translationSensitivity)
      -(getYAxis() * (float)data.position[2] * translationSensitivity)
      +(getZAxis() * (float)data.position[1] * translationSensitivity));
  // clang-format on
}

void ofxConnexionCam::updateRotation(ConnexionData &data) {
  // clang-format off
  rotate(ofQuaternion((float)data.rotation[0] * rotationSensitivity * (isPitchInverted ? 1.0 : -1.0), ofCamera::getXAxis(), // pitch
                      (float)-data.rotation[2] * rotationSensitivity, isRollEnabled ? ofCamera::getYAxis() : upVector, // yaw
                      isRollEnabled ? (float)data.rotation[1] * rotationSensitivity : 0.0, ofCamera::getZAxis())); // roll
  // clang-format on
}
