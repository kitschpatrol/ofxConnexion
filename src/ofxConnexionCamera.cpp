#include "ofxConnexionCamera.h"

void ofxConnexionCamera::setup(ofxConnexion& con){
	connexion = &con;
	curLeftRight = 0;
	curUpDown = 0;
}

void ofxConnexionCamera::update(){

	ConnexionData& data = connexion->getData();
	
	float leftRight = ofMap(data.translation[0],-2048,2048,-1,1,true);
	//leftRight = ofMap(data.currentState.getPosition().x,-2048,2048,-1,1,true);

//	float upDown = 0;
	float upDown = ofMap(data.rotation[0],-180,180,-1,1,true);
	//upDown += ofMap(data.currentState.getOrientationEuler().x,-180,180,-1,1,true);

	curLeftRight = ofLerp(curLeftRight, leftRight, .1);
	curUpDown    = ofLerp(curUpDown, upDown, .1);

	ofVec3f newPosition = lookTarget;
	newPosition.z += baseDistance;

	newPosition.rotate(curLeftRight*90,lookTarget,ofVec3f(0,1,0));
	newPosition.rotate(curUpDown*75,lookTarget,ofVec3f(1,0,0));

	setPosition(newPosition);
	lookAt(lookTarget);
	
}
