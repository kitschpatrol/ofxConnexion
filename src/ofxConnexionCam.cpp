//  ofxConnexionCam.cpp
//  Created by TAKAHASHI Tomohiro on 5/29/13.

#include "ofxConnexionCam.h"

ofxConnexionCam::ofxConnexionCam() {
  connexionController.targetNode = this;
  connexionController.enableInput();
}

ofxConnexionCam::~ofxConnexionCam() {
  connexionController.disableInput();
}
