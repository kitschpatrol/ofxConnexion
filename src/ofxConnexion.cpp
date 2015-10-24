#include "ofxConnexion.h"

bool ConnexionData::getButton(int button) {
  return (buttonState & (1 << button)) != 0;
}

ofVec3f clampNorm(ofVec3f &vec) {
  return ofVec3f(ofClamp(vec.x, -1, 1), ofClamp(vec.y, -1, 1), ofClamp(vec.z, -1, 1));
}

// these two functions assume your speed is set to max
// the actual values returned go above 2600 (sometimes significantly)
// but it seems to be an interaction between different axes
// causing this, and not a change in the axis itself
ofVec3f ConnexionData::getNormalizedPosition() {
  // TODO look up speed from ConnexionDevicePrefsPtr?
  ofVec3f result(position[0], position[1], position[2]);
  result /= 2600.0;
  return clampNorm(result);
}

ofVec3f ConnexionData::getNormalizedRotation() {
  // TODO look up speed from ConnexionDevicePrefsPtr?
  ofVec3f result(rotation[0], rotation[1], rotation[2]);
  result /= 2600.;
  return clampNorm(result);
}

//-------------

ofEvent<ConnexionData> ofxConnexion::axisUpdateEvent;
ofEvent<int> ofxConnexion::buttonPressedEvent;
ofEvent<int> ofxConnexion::buttonReleasedEvent;

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

  switch (messageType) {
    case kConnexionMsgDeviceState:
      switch (msg->command) {
        case kConnexionCmdHandleAxis: {
          memcpy(connexionData.position, &(msg->axis[0]), 3 * 16); // each axis value is int16_t
          memcpy(connexionData.rotation, &(msg->axis[3]), 3 * 16); // each axis value is int16_t
          ofNotifyEvent(axisUpdateEvent, connexionData);
          break;
        }
        case kConnexionCmdHandleButtons: {
          connexionData.deviceId = msg->address;
          connexionData.buttonState = msg->buttons;
          static uint32_t lastButtonState = 0;

          // detect button changes (packed into int32_t)
          if (connexionData.buttonState != lastButtonState) {
            for (int i = 0; i < 32; i++) {
              bool isButtonOnNow = connexionData.buttonState & (1 << i);
              bool isButtonOnBefore = lastButtonState & (1 << i);

              if (isButtonOnNow && !isButtonOnBefore) {
                ofNotifyEvent(buttonPressedEvent, i);
              } else if (!isButtonOnNow && isButtonOnBefore) {
                ofNotifyEvent(buttonReleasedEvent, i);
              }
            }
          }

          lastButtonState = connexionData.buttonState;
          break;
        }
      }
  }
}
