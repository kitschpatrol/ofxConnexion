#pragma once

#include "ofMain.h"

#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/
#include "siapp.h"     /* Required for siapp.lib symbols */

//TODO:
// -- buttons
class ConnexionData {
  public:
	ConnexionData(){
		active = 0;
		memset(translation,0,sizeof(short)*3);
		memset(rotation,0,sizeof(short)*3);
		deviceId = 0;
	}

	bool active;
	short translation[3];
	short rotation[3];
	ofNode currentState;
	short deviceId;
//	short buttonState;
//	bool getButton(int button);
};

class ofxConnexion : public ofThread {
  public:
	ofEvent<ConnexionData> connexionEvent;
	
	~ofxConnexion();

	bool setup();
	void update();
	bool isActive();

	ConnexionData& getData();
	

	//TODO:
	//void  SbButtonPressEvent(int buttonnumber);
	//void  SbButtonReleaseEvent(int buttonnumber);
	//void  HandleDeviceChangeEvent(SiSpwEvent *pEvent);

  protected:
	HDC         hdc;          /* Handle to Device Context used to draw on screen */
	HWND        hWndMain;     /* Handle to Main Window */
	SiHdl       devHdl;       /* Handle to 3D Mouse Device */
	ConnexionData data;

	void threadedFunction();
	void pollMessage();

	void SbMotionEvent(SiSpwEvent *pEvent);
	void SbZeroEvent();

};
