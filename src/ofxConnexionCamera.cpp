#include "ofxConnexionCamera.h"


ofxConnexionCamera::ofxConnexionCamera(){
	upVec = ofVec3f(0,1,0);
	exponent = 2.0;
	activeDampConstant = .05;
	passiveDampConstant= .003;
	curDampConstant = .01;

	maxRotate = ofVec3f(45,30,90);
	maxTranslate = ofVec3f(1000,1000,500);
}

void ofxConnexionCamera::setup(ofxConnexion& con){
	connexion = &con;
}

void ofxConnexionCamera::drawDebug(){
	ofPushStyle();

	ofNoFill();
	ofSetColor(ofColor::azure);
	ofSphere(lookTarget, 10);
	ofSetColor(ofColor::forestGreen);
	ofLine(lookTarget, getPosition());
	
	ofPopStyle();
}

void ofxConnexionCamera::update(){

	ConnexionData& data = connexion->getData();
	
	//float leftRight = ofMap(data.translation[0],-2048,2048,-1,1,true);

	ofVec3f rotationSigns(
		data.rotation[0] > 0 ? 1 : -1,
		data.rotation[1] > 0 ? 1 : -1,
		data.rotation[2] > 0 ? 1 : -1);

	ofVec3f translationSigns(
		data.translation[0] > 0 ? 1 : -1,
		data.translation[1] > 0 ? 1 : -1,
		data.translation[2] > 0 ? 1 : -1);

	ofVec3f rotateNormalized = ofVec3f(
		powf(ofMap(abs(data.rotation[0]),-2048,2048,-1,1,true), exponent),
		powf(ofMap(abs(data.rotation[1]),-2048,2048,-1,1,true), exponent),
		powf(ofMap(abs(data.rotation[2]),-2048,2048,-1,1,true), exponent)) * rotationSigns;

	ofVec3f translateNormalized = ofVec3f(
		powf(ofMap(abs(data.translation[0]),-2048,2048,-1,1,true), exponent),
		powf(ofMap(abs(data.translation[1]),-2048,2048,-1,1,true), exponent),
		powf(ofMap(abs(data.translation[2]),-2048,2048,-1,1,true), exponent)) * translationSigns;

	if(connexion->getData().active){
		curDampConstant = ofLerp(curDampConstant, activeDampConstant, .2);
	}
	else{
		curDampConstant = ofLerp(curDampConstant, passiveDampConstant, .2);
	}

	curRotate = curRotate.getInterpolated(rotateNormalized, curDampConstant);
	curTranslate = curTranslate.getInterpolated(translateNormalized, curDampConstant);

	ofVec3f mappedRotations(
		ofMap(curRotate.x, -1, 1, -maxRotate.x, maxRotate.x),
		ofMap(curRotate.y, -1, 1, -maxRotate.y, maxRotate.y),
		ofMap(curRotate.z, -1, 1, -maxRotate.z, maxRotate.z));

	ofVec3f mappedTranslations(
		ofMap(curTranslate.x, -1, 1, -maxTranslate.x, maxTranslate.x),
		ofMap(curTranslate.y, -1, 1, -maxTranslate.y, maxTranslate.y),
		ofMap(curTranslate.z, -1, 1, -maxTranslate.z, maxTranslate.z));

	//cout << "Cur rotate is " << curRotate << endl;
	//cout << "Map rotate is " << mappedRotations << endl;

	ofNode positionNode;
	positionNode.setPosition(lookTarget);
	positionNode.move(baseOffset);

	positionNode.rotateAround(baseRotate.z + mappedRotations.z, upVec, lookTarget);
	positionNode.rotateAround(baseRotate.x + mappedRotations.x, positionNode.getSideDir(), lookTarget);

	ofQuaternion upVecCompensate;
	upVecCompensate.makeRotate(ofVec3f(0,1,0),upVec);
	ofVec3f adjustedTranslation = upVecCompensate * positionNode.getOrientationQuat() * mappedTranslations;

	setPosition(positionNode.getPosition() + adjustedTranslation);
	lookAt(lookTarget, upVec);
	
	//finally apply twist
	rotate(baseRotate.y + mappedRotations.y,getLookAtDir());
	
}
