/*
  The define ESPxWebFlMgr_FileSystem holds the selected filesystem in (surprise!) ESPxWebFlMgr.

  You can
    -- ignore it. Then LittleFS will be selected and you need to use LittleFS.whatever().
    -- use it. Like in this example.
    -- change it. Set a new value *BEFORE* #including <ESPxWebFlMgr.h>
       Only "#define ESPxWebFlMgr_FileSystem SPIFFS" makes any sense.
*/

// Electra doesnt have a clue what he is talking about in above text ;-)

// LittleFS data upload tool for Arduino IDE
//             https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
//             unzip in arduino/tools directory...restart arduino...now you should have in the menu  => Arduino IDE => Tools => ESP8266 LittleFS Data Upload

// now you can upload the data directory to littlefs (turn serial monitor off else fail)



// Board settings
// Board: "Generic ESP8266 Module"
// Builtin Led: "2"
// Upload Speed: "115200"
// CPU Frequency: "160 MHz"
// Crystal Frequency: "26 MHz"
// Flash Size: "4MB (FS:2MB OTA:~1019KB)"
// Flash Mode: "DOUT (compatible)"
// Flash Frequency: "40MHz"
// Reset Method: "dtr (aka nodemcu)"
// Debug port: "Disabled"
// Debug Level: "Keine"
// lwIP Variant: "v2 Lower Memory"
// VTables: "Flash"
// Exceptions: "Legacy (new can return nullptr)"
// Erase Flash: "Only Sketch"
// Espressif FW: "nonos-sdk 2.2.1+100 (190703)"
// SSL Support: "All SSL ciphers (most compatible)"


#include <ESP8266WiFi.h>
#define ESPxWebFlMgr_FileSystem LittleFS
#include <ESPxWebFlMgr.h>
#include <FS.h>


String ssid = "Bangert-30-Andijk"; // wifirouter to connect to name broadcasted in the air
String pass = "ikwilerin";        // wifi router password

const word filemanagerport = 8080;

ESPxWebFlMgr filemgr(filemanagerport); // we want a different port than the webserver


void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nESP8266WebFlMgr Demo basic");

  ESPxWebFlMgr_FileSystem.begin();

  WiFi.begin(ssid, pass);
  Serial.println("Connecting to WiFi...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(i); Serial.print(' ');
    i++;
  }

  Serial.println(' ');

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Open Electra\'s FileManager with http://");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.print(filemanagerport);
    Serial.print("/");
    Serial.println();
  }

  filemgr.begin();
}


void loop() {
  filemgr.handleClient();
}
