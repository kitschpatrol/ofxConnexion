//  ofxConnexionCam.cpp
//  Created by TAKAHASHI Tomohiro on 5/29/13.

#include "ofxConnexionCam.h"

ofxConnexionCam::ofxConnexionCam() {
  connectionController.targetNode = this;
  connectionController.enableInput();
}

ofxConnexionCam::~ofxConnexionCam() {
  connectionController.disableInput();
}
