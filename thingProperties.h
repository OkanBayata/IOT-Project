// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "3cab6fda-9aaf-43e0-ad83-fbaf666209f9";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onInCountChange();
void onInsideCountChange();
void onOutCountChange();

int inCount;
int insideCount;
int outCount;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(inCount, READWRITE, ON_CHANGE, onInCountChange);
  ArduinoCloud.addProperty(insideCount, READWRITE, ON_CHANGE, onInsideCountChange);
  ArduinoCloud.addProperty(outCount, READWRITE, ON_CHANGE, onOutCountChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
