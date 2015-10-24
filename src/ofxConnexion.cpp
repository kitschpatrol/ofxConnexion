
#include "ofxConnexion.h"

bool ofxConnexion::setup(){

  int res;                             /* result of SiOpen, to be returned  */
  SiOpenData oData;                    /* OS Independent data to open ball  */ 
  
  hWndMain  = ofGetWindowPtr()->getWin32Window();

  /*init the SpaceWare input library */
  if (SiInitialize() == SPW_DLL_LOAD_ERROR)  {
	  MessageBox(hWndMain,_T("Error: Could not load SiAppDll dll files"),
		         NULL, MB_ICONEXCLAMATION);
	}

  SiOpenWinInit(&oData, hWndMain);    /* init Win. platform specific data  */
  SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

  /* open data, which will check for device type and return the device handle
     to be used by this function */ 
	if ( (devHdl = SiOpen ("3DxTest32", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)  {
		#if 0
		SPWchar err[100];
		extern enum SpwRetVal SpwErrorVal;
		_stprintf(err,_T("SiOpen error: %d\n"),SpwErrorVal);
		MessageBox(NULL,err,_T("3DxTest: SiOpen Error"),MB_ICONERROR);
		#endif

		SiTerminate();  /* called to shut down the SpaceWare input library */
		res = 0;        /* could not open device */
		return res; 
	}

	//startThread(true);

	res = 1;        /* opened device succesfully */ 
	return res;
}

ofxConnexion::~ofxConnexion(){
	waitForThread(true);
}

/*--------------------------------------------------------------------------
 * Function: DispatchLoopNT()
 *
 * Description:
 *    This function contains the main message loop which constantly checks for 
 *    3D mouse Events and handles them apropriately. 
 *
 * Args: None
 *    
 *
 * Return Value:
 *    int  msg.wparam                  // event passed to window
 *
 *--------------------------------------------------------------------------*/
void ofxConnexion::threadedFunction()
{
	while(isThreadRunning()){
		pollMessage();
	}

	bool threadIsDone = false;
} 


void ofxConnexion::update(){
	pollMessage();
}

void ofxConnexion::pollMessage(){

	   int            num;      /* number of button pressed */
	   MSG            msg;      /* incoming message to be evaluated */
	   BOOL           handled;  /* is message handled yet */ 
	   SiSpwEvent     Event;    /* SpaceWare Event */ 
	   SiGetEventData EData;    /* SpaceWare Event Data */

	   handled = SPW_FALSE;     /* init handled */

	   /* start message loop */ 
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			handled = SPW_FALSE;
      
			/* init Window platform specific data for a call to SiGetEvent */
			SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
      
			/* check whether msg was a 3D mouse event and process it */
			if (SiGetEvent (devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
			{
				if (Event.type == SI_MOTION_EVENT)
				{
					SbMotionEvent(&Event);        /* process 3D mouse motion event */     
				}
				if (Event.type == SI_ZERO_EVENT)
				{
					SbZeroEvent();                /* process 3D mouse zero event */     
				}
				if (Event.type == SI_BUTTON_EVENT)
				{
					if ((num = SiButtonPressed (&Event)) != SI_NO_BUTTON)	
					{
						//SbButtonPressEvent(num);        /* process 3D mouse button event */
					}
					if ((num = SiButtonReleased (&Event)) != SI_NO_BUTTON){
						//SbButtonReleaseEvent(num);        /* process 3D mouse button event */
					}
				}
         
				if (Event.type == SI_DEVICE_CHANGE_EVENT){
					//HandleDeviceChangeEvent(&Event);
				}

				handled = SPW_TRUE;              /* 3D mouse event handled */ 
			}

			/* not a 3D mouse event, let windows handle it */
			if (handled == SPW_FALSE)
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
}

ConnexionData& ofxConnexion::getData(){
	return data;
}

/*----------------------------------------------------------------------
 * Function: SbMotionEvent()
 *
 * Description:
 *    This function recieves motion information and prints out the info 
 *    on screen.
 *    
 *
 * Args:
 *    SiSpwEvent *pEvent   Containts Data from a 3D mouse Event
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void ofxConnexion::SbMotionEvent(SiSpwEvent *pEvent)
{	
	data.translation[0] = pEvent->u.spwData.mData[SI_TX];
	data.translation[1] = pEvent->u.spwData.mData[SI_TY];
	data.translation[2] = pEvent->u.spwData.mData[SI_TZ];

	data.rotation[0] = pEvent->u.spwData.mData[SI_RX];
	data.rotation[1] = pEvent->u.spwData.mData[SI_RY];
	data.rotation[2] = pEvent->u.spwData.mData[SI_RZ];

	data.active = true;

	data.currentState.setPosition(ofVec3f(data.translation[0],data.translation[1],data.translation[2]));
	data.currentState.setOrientation(ofVec3f(data.rotation[0],data.rotation[1],data.rotation[2]));

	//cout << "current position " << data.currentState.getPosition() << "current orientation " << data.currentState.getOrientationEuler() << endl;
	//cout << ofGetElapsedTimef() << "\tMOTION EVENT " << endl;

	//len1= _stprintf( buff1, _T("TY: %d         "), pEvent->u.spwData.mData[SI_TY] );
	//len2= _stprintf( buff2, _T("TZ: %d         "), pEvent->u.spwData.mData[SI_TZ] );
	//len3= _stprintf( buff3, _T("RX: %d         "), pEvent->u.spwData.mData[SI_RX] );
	//len4= _stprintf( buff4, _T("RY: %d         "), pEvent->u.spwData.mData[SI_RY] );
	//len5= _stprintf( buff5, _T("RZ: %d         "), pEvent->u.spwData.mData[SI_RZ] );

   //TCHAR buff0[30];                            /* text buffer for TX */
   //TCHAR buff1[30];                            /* text buffer for TY */
   //TCHAR buff2[30];                            /* text buffer for TZ */
   //TCHAR buff3[30];                            /* text buffer for RX */
   //TCHAR buff4[30];                            /* text buffer for RY */
   //TCHAR buff5[30];                            /* text buffer for RZ */ 
   //TCHAR buff6[30];                            /* text buffer for Period */ 

   //int len0,len1,len2,len3,len4,len5,len6;	   /* length of each buffer */
   
   /* put the actual ball data into the buffers */
	//len0= _stprintf( buff0, _T("TX: %d         "), pEvent->u.spwData.mData[SI_TX] );
	//len1= _stprintf( buff1, _T("TY: %d         "), pEvent->u.spwData.mData[SI_TY] );
	//len2= _stprintf( buff2, _T("TZ: %d         "), pEvent->u.spwData.mData[SI_TZ] );
	//len3= _stprintf( buff3, _T("RX: %d         "), pEvent->u.spwData.mData[SI_RX] );
	//len4= _stprintf( buff4, _T("RY: %d         "), pEvent->u.spwData.mData[SI_RY] );
	//len5= _stprintf( buff5, _T("RZ: %d         "), pEvent->u.spwData.mData[SI_RZ] );
	//len6= _stprintf( buff6, _T(" P: %d         "), pEvent->u.spwData.period);

 //  
 //  /* get handle of our window to draw on */
	//hdc = GetDC(hWndMain);        
 //  
 //  /* print buffers */
 //  TextOut(hdc,  0 ,   0, _T("Motion Event                "), 28);
 //  TextOut(hdc, 15 , 100, buff0, len0);
 //  TextOut(hdc, 15 , 120, buff1, len1);
 //  TextOut(hdc, 15 , 140, buff2, len2);
 //  TextOut(hdc, 15 , 160, buff3, len3);
 //  TextOut(hdc, 15 , 180, buff4, len4);
 //  TextOut(hdc, 15 , 200, buff5, len5);
 //  TextOut(hdc, 15 , 220, buff6, len6);

 //  /* Dump to debugger output buffer to get a running log */
 //  _RPT3(_CRT_WARN,"%S %S %S"   , buff0, buff1, buff2);
 //  _RPT3(_CRT_WARN," %S %S %S", buff3, buff4, buff5);
 //  _RPT1(_CRT_WARN," %S\n", buff6);
   
   /*release our window handle */ 
//	ReleaseDC(hWndMain,hdc);
}


/*----------------------------------------------------------------------
 * Function: SbZeroEvent()
 *
 * Description:
 *    This function clears the previous data, no motion data was recieved
 *    
 *    
 *
 * Args:
 *    NONE
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void ofxConnexion::SbZeroEvent()
{	

	data.translation[0] = 0;
	data.translation[1] = 0;
	data.translation[2] = 0;

	data.rotation[0] = 0;
	data.rotation[1] = 0;
	data.rotation[2] = 0;

	data.active = false;

	data.currentState.setPosition(ofVec3f(data.translation[0],data.translation[1],data.translation[2]));
	data.currentState.setOrientation(ofVec3f(data.rotation[0],data.rotation[1],data.rotation[2]));
	//cout << ofGetElapsedTimef() << "\tZERO EVENT" << endl;

   /* get handle of our window to draw on */
   //hdc = GetDC(hWndMain);

   ///* print null data */   
   //TextOut(hdc,  0 ,   0, _T("Zero Event                  "), 28);
   //TextOut(hdc, 15 , 100, _T("TX: 0          "), 15);
   //TextOut(hdc, 15 , 120, _T("TY: 0          "), 15);
   //TextOut(hdc, 15 , 140, _T("TZ: 0          "), 15);
   //TextOut(hdc, 15 , 160, _T("RX: 0          "), 15);
   //TextOut(hdc, 15 , 180, _T("RY: 0          "), 15);
   //TextOut(hdc, 15 , 200, _T("RZ: 0          "), 15);
   //TextOut(hdc, 15 , 220, _T(" P: 0          "), 15);

   ///*release our window handle */ 
   //ReleaseDC(hWndMain,hdc);
   //
}
/*
bool ConnexionData::getButton(int button) {
	return (buttonState & (1 << button)) != 0;
}

ofVec3f clampNorm(ofVec3f& vec) {
    return ofVec3f(ofClamp(vec.x, -1, 1),
                   ofClamp(vec.y, -1, 1),
                   ofClamp(vec.z, -1, 1));
}

// these two functions assume your speed is set to max
// the actual values returned go above 2600 (sometimes significantly)
// but it seems to be an interaction between different axes
// causing this, and not a change in the axis itself
ofVec3f ConnexionData::getNormalizedPosition() {
    ofVec3f result(position[0],
                   position[1],
                   position[2]);
    result /= 2600.;
    return clampNorm(result);
}

ofVec3f ConnexionData::getNormalizedRotation() {
    ofVec3f result(rotation[0],
                   rotation[1],
                   rotation[2]);
    result /= 2600.;
    return clampNorm(result);
}

ofEvent<ConnexionData> ofxConnexion::connexionEvent;
ConnexionData ofxConnexion::connexionData;
UInt16 ofxConnexion::clientId;

void ofxConnexion::start() {
	InstallConnexionHandlers(driverHandler, 0L, 0L);
	clientId = RegisterConnexionClient(kConnexionClientWildcard, NULL, kConnexionClientModeTakeOver, kConnexionMaskAll);
}

void ofxConnexion::stop() {
	UnregisterConnexionClient(clientId);
	CleanupConnexionHandlers();
}

// http://www.3dconnexion.com/forum/viewtopic.php?t=4266
#define OFX_CONNEXION_LED_ON (0x00010001)
#define OFX_CONNEXION_LED_OFF (0x00010000)

void ofxConnexion::setLed(bool state) {
	SInt32 result;
	ConnexionClientControl(clientId, kConnexionCtlSetLEDState, state ? OFX_CONNEXION_LED_ON : OFX_CONNEXION_LED_OFF, &result);
}

void ofxConnexion::driverHandler(io_connect_t connection, natural_t messageType, void *messageArgument) {
	ConnexionDeviceStatePtr msg = (ConnexionDeviceStatePtr)messageArgument;
	switch(messageType) {
		case kConnexionMsgDeviceState:
			switch (msg->command) {
				case kConnexionCmdHandleAxis: {
					memcpy(connexionData.position, &(msg->axis[0]), 3 * sizeof(short));
					memcpy(connexionData.rotation, &(msg->axis[3]), 3 * sizeof(short));
				}
				case kConnexionCmdHandleButtons: {
					connexionData.deviceId = msg->address;
					connexionData.buttonState = msg->buttons;
					
					ofNotifyEvent(connexionEvent, connexionData);
				}
			}
	}
}
*/
