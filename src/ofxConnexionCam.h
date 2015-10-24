#pragma once

#include "ofMain.h"
#include "ofxConnexionNodeController.h"

class ofxConnexionCam : public ofCamera {
public:
  ofxConnexionCam();
  ~ofxConnexionCam();
  ofxConnexionNodeController connexionController;
};
