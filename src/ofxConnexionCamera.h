#pragma once

#include "ofMain.h"
#include "ofxConnexion.h"

class ofxConnexionCamera : public ofCamera
{
  public:
	void setup(ofxConnexion& con);
	void update();
	
	ofVec3f lookTarget;
	float baseDistance;


  protected:
	  ofxConnexion* connexion;
	  float curLeftRight;
	  float curUpDown;

};