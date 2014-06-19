#pragma once

#include "ofxConnexion.h"
#include "ofxConnexionCamera.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	ofxConnexion connection;
	ofxConnexionCamera camera;

	ofVboMesh spaceDust;
//	void connexionMoved(ConnexionData& data);
};

