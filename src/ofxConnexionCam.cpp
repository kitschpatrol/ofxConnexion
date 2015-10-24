//  ofxConnexionCam.cpp
//  Created by TAKAHASHI Tomohiro on 5/29/13.

#include "ofxConnexionCam.h"

ofxConnexionCam::ofxConnexionCam() : ofEasyCam() {
  bInputEnabled = false;
  bDoRotate = true;
  bDoTranslate = true;
  enableInput();
}

ofxConnexionCam::~ofxConnexionCam() {
  disableInput();
}

void ofxConnexionCam::enableInput() {
  if (!bInputEnabled) {
    bInputEnabled = true;
    ofxConnexion::start();
    ofAddListener(ofEvents().update, this, &ofxConnexionCam::connexionUpdate);
  }
}

void ofxConnexionCam::disableInput() {
  if (bInputEnabled) {
    bInputEnabled = false;
    ofRemoveListener(ofEvents().update, this, &ofxConnexionCam::connexionUpdate);
    ofxConnexion::stop();
  }
}

bool ofxConnexionCam::getInputEnabled() {
  return bInputEnabled;
}

void ofxConnexionCam::connexionUpdate(ofEventArgs &args) {
  // this would be a good place to get data from multiple devices
  // cout << data.getButton(0) << " " << data.getButton(1) << endl;
  ConnexionData &data = ofxConnexion::connexionData;
  if (data.getButton(1)) {
    reset();
  }
  if (data.getButton(0)) {
    if (!bLookBack) {
      ofQuaternion rot = ofQuaternion(180, ofCamera::getYAxis());
      rotate(rot);
      bLookBack = true;
    }
  } else {
    bLookBack = false;
  }
  // bDoRotate = !data.getButton(0);

  // use the button state to set the LED
  // ofxConnexion::setLed(data.getButton(0) || data.getButton(1));
  if (bDoRotate) {
    updateRotation();
  }
  if (bDoTranslate) {
    updateTranslation();
  }
}

void ofxConnexionCam::updateTranslation() {
  // clang-format off
	ConnexionData& data = ofxConnexion::connexionData;
#if 1
  float distance = getDistance() / 2000;
	move((getXAxis() * data.position[0] / 100 * (distance + 1))
     - (getYAxis() * data.position[2] / 100 * (distance + 1))
     + (getZAxis() * data.position[1] / 100 * (distance + 1)));
#else
    move((getXAxis() * data.position[0] / 50)
       + (getYAxis() * data.position[2] / 50)
       - (getZAxis() * data.position[1] / 50));
#endif
  // clang-format on
}

void ofxConnexionCam::updateRotation() {
  ofQuaternion curRot;
  ConnexionData &data = ofxConnexion::connexionData;

  // clang-format off
  curRot = ofQuaternion((float)data.rotation[0] / 500, ofCamera::getXAxis(),
                        (float)-data.rotation[2] / 500, ofCamera::getYAxis(),
                        (float)data.rotation[1] / 500, ofCamera::getZAxis());
  // clang-format on

  ofNode &target = getTarget();
  // setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
  rotate(curRot);
}
