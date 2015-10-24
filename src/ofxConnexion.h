#pragma once

#include "ofMain.h"
#include "3DconnexionClient/ConnexionClientAPI.h"

class ConnexionData {
public:
  // see ConnectionClient.h for sizes
  int16_t position[3];
  int16_t rotation[3];
  uint16_t deviceId;
  uint32_t buttonState;

  bool getButton(int button);

  // these assume you set the speed to max in the control panel
  ofVec3f getNormalizedPosition();
  ofVec3f getNormalizedRotation();
};

class ofxConnexion {
public:
  static ofEvent<ConnexionData> axisUpdateEvent;
  static ofEvent<int> buttonPressedEvent;
  static ofEvent<int> buttonReleasedEvent;
  static ConnexionData connexionData;

  static void start();
  static void stop();
  static void setLed(bool state);

protected:
  static UInt16 clientId;
  static void driverHandler(io_connect_t connection, natural_t messageType, void *messageArgument);
};
