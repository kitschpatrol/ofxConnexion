#include "ofApp.h"

void ofApp::setup(){

	ofBackground(0);
	ofSetFrameRate(60);

	connection.setup();
	camera.setup(connection);
	camera.baseDistance = 100;

	int randomScale = 100;
	for(int i = 0; i < 500; i++){
		spaceDust.addVertex(ofVec3f(
			ofRandom(-randomScale,randomScale),
			ofRandom(-randomScale,randomScale),
			ofRandom(-randomScale,randomScale)));
	}
}

void ofApp::update(){
	connection.update();
	camera.update();
}


void ofApp::draw(){
	
//	ofBackground(ofColor::salmon);
//	ofSetColor(255);
//	ofCircle(ofGetWidth()/2, ofGetHeight()/2, 200);
	
	//cout << "DRAW CALLED " << ofGetElapsedTimef() << endl;
	camera.begin();

	ofNoFill();
	ofColor(255,0,0);
	ofSphere(0,0,0,30);
	
	ofFill();
	glPointSize(3);
	spaceDust.drawVertices();

	camera.end();

	ofNode n = connection.getData().currentState;

	ofDrawBitmapString("POS:\n" + string() +
		"TX: " + ofToString(n.getPosition().x) + "\n" +
		"TY: " + ofToString(n.getPosition().y) + "\n" + 
		"TZ: " + ofToString(n.getPosition().z) + "\n" +
		"RX: " + ofToString(n.getOrientationEuler().x) + "\n" +
		"RY: " + ofToString(n.getOrientationEuler().y) + "\n" +
		"RZ: " + ofToString(n.getOrientationEuler().z) + "\n", 20, 20);
	
}

