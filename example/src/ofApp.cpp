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
	

	ofEnableDepthTest();
	camera.begin();

	ofNoFill();
	
	ofSetColor(255);
	ofDrawSphere(0,0,0,15);	

	ofVec3f satellite(30, 0, 0);
	ofSetColor(255,255,0);
	ofDrawSphere(satellite,5);

	satellite.rotate(90,ofVec3f(0,1,0));
	ofSetColor(255,0,255);
	ofDrawSphere(satellite,5);

	satellite.rotate(90,ofVec3f(0,1,0));
	ofSetColor(0,255,255);
	ofDrawSphere(satellite,5);

	satellite.rotate(90,ofVec3f(0,1,0));
	ofSetColor(0,255,0);
	ofDrawSphere(satellite,5);

	ofFill();
	ofSetColor(255);
	glPointSize(3);
	spaceDust.drawVertices();

	camera.end();

	ofNode n = connection.getData().currentState;

	ofDrawBitmapString("POS:\n" + string() +
		"TX: " + ofToString(connection.getData().translation[0]) + "\n" +
		"TY: " + ofToString(connection.getData().translation[1]) + "\n" + 
		"TZ: " + ofToString(connection.getData().translation[2]) + "\n" +
		"RX: " + ofToString(connection.getData().rotation[0]) + "\n" +
		"RY: " + ofToString(connection.getData().rotation[1]) + "\n" +
		"RZ: " + ofToString(connection.getData().rotation[2]) + "\n", 20, 20);
	
}

