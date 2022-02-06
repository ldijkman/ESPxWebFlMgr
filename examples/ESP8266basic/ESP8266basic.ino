/*
 * 
 * 
 *  http://filemanager.local:8080    => windows, android users => https://github.com/ldijkman/randomnerd_esp32_wifi_manager/blob/main/HELP.TXT
 * 
 *  
 
 https://www.youtube.com/watch?v=3kg8DjFIe7k
 
 https://www.youtube.com/watch?v=5wrMgU-uW78

 https://youtu.be/K94DvxUGsOoxUGsOo
 
 https://www.youtube.com/watch?v=pOvma-AF3lg
 
 
 
 * 
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
//               unzip in arduino/tools directory...
//                 restart arduino ide...
//                   now you should have in the menu  => Arduino IDE => Tools => ESP8266 LittleFS Data Upload

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
#include <ESP8266mDNS.h>


String ssid = "Bangert_30_Andijk"; // wifirouter to connect to name broadcasted in the air
String pass = "ookikwilerin";        // wifi router password

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
 if (!MDNS.begin("filemanager")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  MDNS.addService("http", "tcp", 8080);

  Serial.print("http://filemanager.local:8080");
  
  filemgr.begin();
}


void loop() {
  filemgr.handleClient();
  MDNS.update();   // looks like this is needed only for esp8266 otherwise i dont see mdns url in bonjourbrowser not needed for esp32
}
