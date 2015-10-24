//  ofxConnexionCam.cpp
//  Created by TAKAHASHI Tomohiro on 5/29/13.

#include "ofxConnexionCam.h"

ofxConnexionCam::ofxConnexionCam() : ofEasyCam() {
    bInputEnabled = false;
    bDoRotate = true;
    bDoTranslate = true;
	enableInput();
}

ofxConnexionCam::~ofxConnexionCam() {
	disableInput();
}


void ofxConnexionCam::enableInput(){
	if(!bInputEnabled){
		bInputEnabled = true;
        ofxConnexion::start();
		ofAddListener(ofEvents().update , this, &ofxConnexionCam::connexionUpdate);
	}
}

void ofxConnexionCam::disableInput(){
	if(bInputEnabled){
		bInputEnabled = false;
		ofRemoveListener(ofEvents().update, this, &ofxConnexionCam::connexionUpdate);
        ofxConnexion::stop();
	}
}

bool ofxConnexionCam::getInputEnabled(){
	return bInputEnabled;
}

void ofxConnexionCam::connexionUpdate(ofEventArgs & args) {
	// this would be a good place to get data from multiple devices
	//cout << data.getButton(0) << " " << data.getButton(1) << endl;
	ConnexionData& data = ofxConnexion::connexionData;
	if (data.getButton(1)) {
        reset();
    }
	if (data.getButton(0)) {
        if (!bLookBack) {
            ofQuaternion rot = ofQuaternion(180, ofCamera::getYAxis());
            rotate(rot);
            bLookBack = true;
        }
    } else {
        bLookBack = false;
    }
	//bDoRotate = !data.getButton(0);
    
	// use the button state to set the LED
	//ofxConnexion::setLed(data.getButton(0) || data.getButton(1));
    if (bDoRotate) {
        updateRotation();
    }
    if (bDoTranslate) {
        updateTranslation();
    }
}

void ofxConnexionCam::updateTranslation(){
	ConnexionData& data = ofxConnexion::connexionData;
#if 1
    float distance = getDistance() / 2000;
	move((getXAxis() * data.translation[0] / 100 * (distance + 1))
         + (getYAxis() * data.translation[2] / 100 * (distance + 1))
         - (getZAxis() * data.translation[1] / 100 * (distance + 1)));
#else
    move((getXAxis() * data.translation[0] / 50)
         + (getYAxis() * data.translation[2] / 50)
         - (getZAxis() * data.translation[1] / 50));
#endif
}

void ofxConnexionCam::updateRotation(){
	ofQuaternion curRot;
	ConnexionData& data = ofxConnexion::connexionData;

	curRot = ofQuaternion((float)data.rotation[0] / 500, ofCamera::getXAxis(),
                          (float)-data.rotation[1] / 500, ofCamera::getYAxis(),
                          (float)-data.rotation[2] / 500, ofCamera::getZAxis());
    
    ofNode& target = getTarget();
	//setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
	rotate(curRot);
}

