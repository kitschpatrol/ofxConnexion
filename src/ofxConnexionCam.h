#pragma once

#include "ofEasyCam.h"
#include "ofEvents.h"
#include "ofxConnexion.h"

class ofxConnexionCam : public ofEasyCam {
public:
  ofxConnexionCam();
  ~ofxConnexionCam();

  void enableInput();
  void disableInput();
  bool getInputEnabled();

private:
  void connexionUpdate(ofEventArgs &args);
  void updateRotation();
  void updateTranslation();

  bool bInputEnabled;
  bool bDoRotate;
  bool bDoTranslate;
  bool bLookBack;
};
