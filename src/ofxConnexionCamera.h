#pragma once

#include "ofMain.h"
#include "ofxConnexion.h"

class ofxConnexionCamera : public ofCamera
{
  public:
	ofxConnexionCamera();
	void setup(ofxConnexion& con);
	void update();
	
	void drawDebug();

	ofVec3f lookTarget;
	ofVec3f upVec;

	ofVec3f maxRotate;
	ofVec3f maxTranslate;

	float exponent;
	float activeDampConstant;
	float passiveDampConstant;

	ofVec3f baseOffset;
	ofVec3f baseRotate;
	
  protected:
	  ofxConnexion* connexion;
	  float curDampConstant; //lerps between active and inactive
	  ofVec3f curTranslate;
	  ofVec3f curRotate;

};