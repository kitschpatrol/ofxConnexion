#pragma once

#include "ofMain.h"
#include "ofxConnectionNodeController.h"

class ofxConnexionCam : public ofCamera {
public:
  ofxConnexionCam();
  ~ofxConnexionCam();
  ofxConnectionNodeController connectionController;

protected:
};
