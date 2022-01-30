ESP8266 FileManager <img src="https://github.com/ldijkman/randomnerd_esp32_wifi_manager/raw/main/images/esp_8266_nodemcu.png" width="200" height="200">https://www.google.com/search?q=ESP8266+nodemcu

#### NOTE: issue on the persistance of credentials in the core3.x of 8266, which does not work
#### Solution => https://github.com/ldijkman/ESPxWebFlMgr/issues/3

---


<b>SIEMENS</b> related, People first read the license at the bottom ?!

# &euro; 100 Euro for the first that gives a working code 

## for this Filemanager on ESPAsyncWebServer? (ESP8266)

like it is now, working on ESP8266WebServer

---

Code submissions => https://github.com/ldijkman/ESPxWebFlMgr/discussions (link to sourcecode?)

---

for proffessional coders it maybe not enough, but it may trigger other ;-)

---


Electra Begs, Can SomeOne Please?

 Make this work with ESPAsyncWebServer?

## ESPxWebFlMgr by Holger Lembke

### HowTo run this WebFilemanager on a ESP8266 LittleFS

download zip => https://github.com/ldijkman/ESPxWebFlMgr/archive/refs/heads/master.zip

arduino ide=>sketch=>include library=>add zip library   (ESPxWebFlMgr-master.zip)

arduino ide=>file=>examples=>ESP8266 Web File Manager => ESP8266basic   example

line 44 fill ssid and pasword for your wifirouter connection

upload data directory to ESP8266 LittleFS (Descripton HowToDoTHAT in ESP8266basic   example)

program the ESP8266 and Browse to the IP that is given in serial monitor

https://www.google.com/search?q=ESP8266 minimal 32mbit==4MByte version? Better with USB connection

---

https://github.com/ldijkman/ESPxWebFlMgr/blob/master/examples/ESP8266basic/ESP8266basic.ino

i use Arduino IDE 1.8.19 Linux ARM 32 Bits on RaspBerry pi400 https://www.arduino.cc/en/software

---

# Edit Electra's Skin online inbrowser WebFileManager

Would like to run this on ESPAsyncWebServer 

So that we can change the Look and Feel of Electra with skins (HTML / CSS)

Modify Electra like you like her to be

- Without any ESP/Arduino programming for a bigger audience
- Edit a HTML/CSS design in jsfiddle.net or codepen.io or other online/offline designer
- wifimanager.html https://jsfiddle.net/luberth/4avpquhs/show
- index.html https://jsfiddle.net/luberth/sx61b08t/show
- schedule.html https://jsfiddle.net/luberth/ow3zceyn/show
- 
- - and upload or edit copy/paste it with the  WebFileManager Editor on the ESPx LittleFS

- Got to Please Electra https://github.com/ldijkman/randomnerd_esp32_wifi_manager

Some Changes, WishList ?:
- run on ESPAsyncWebServer, Make Electra Happy?
- <del>extra colomn/row for preview files in browser</del> == i have added a V button
- <del>maybe small icons instead of letters</del> == done with a font, later images maybe less and no external traffic
- <del>there is no E edit for .txt files </del>=> i see in code i can set #define fileManagerEditEverything
- a touch or click on dropzone should open a filebrowsebox for uploading (try to drag/drop files on phone, i cannot, dontknow)
- maybe a new file create button
- unzip button
- textarea colored simple syntax higlighting

can you help shorten above wishlist?


<img src="https://github.com/ldijkman/randomnerd_esp32_wifi_manager/blob/main/images/20220122_053531.jpg">

### note: texteditor window size can be changed by dragging right bottom corner

<img src="https://github.com/ldijkman/randomnerd_esp32_wifi_manager/blob/main/images/20220122_053548.jpg">

### note: texteditor window size can be changed by dragging right bottom corner

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/20220123_150041.jpg">

### note: texteditor window size can be changed by dragging right bottom corner

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/Screenshot_20220123-163211_Chrome.jpg" width="70%">

- X extra colomn/row for preview files in browser == i have added a V button
- 
<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/Screenshot_20220123-213316_Chrome.jpg">

- X extra colomn/row for preview files in browser == i have added a V button

Trying icon font =  no rename maybe  A&curarr;B  ,  hee a &#8623; Electra &#8623; character

maybe less size/no external traffic when i make images from the font == now images, --files.png = not shown in filemanager list

added a drop file zone icon middle left

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/2022-01-27-051809_1360x768_scrot.png">

editor => codemirror syntax highlight and line numbers,   no external Js, compressed local

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/2022-01-29-035103_1360x768_scrot.png">

the eye previev opens iframed with a backlink

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/2022-01-30-073702_1360x768_scrot.png">

---

# WOW, Can Electra have her own JSFiddle Clone

# YES! Running on ESP8266 

fork https://github.com/ldijkman/jsbin-jsfiddle-clone  original https://github.com/swapnilmishra/jsbin-jsfiddle-clone

## Edit Electra's Skin locally (View as you Type, realtime WYSIWYG)

even better as jsfiddle or codepen

all small script files, the biggest codemirror.js i allready have for the editor

![2022-01-30-165841_1360x768_scrot](https://user-images.githubusercontent.com/45427770/151707142-ac420d06-1d6b-4711-b356-e91887cbeabf.png)

### looks like 160kb extra file uncompressed

![2022-01-30-182031_1360x768_scrot](https://user-images.githubusercontent.com/45427770/151710059-1e967987-f2a4-486b-9e5e-521ba28bf125.png)


---

Alexander W. on Facebook commented => looks like LCARS

Wikipedia => In the Star Trek fictional universe, LCARS is a computer operating system. Within Star Trek chronology

an LCARS index /  css skin would be nice for Electra = Start Trek Electra house controlled devices

https://www.google.com/search?q=lcars

<img src="https://github.com/ldijkman/ESPxWebFlMgr/blob/master/img/ezgif-6-def92c1f6f.gif">

# Electra LCARS Star-Trek Skin??? 
## https://jsfiddle.net/luberth/sx61b08t/show

---

This is a fork from original FileManager by Holger Lembke

original https://github.com/holgerlembke/ESPxWebFlMgr
---

* Manage your files with a simple web based interface.
* Supports Arduino ESP8266 and Arduino ESP32.
* Has local editing, renaming, downloading and compression.

![this is it](https://raw.githubusercontent.com/holgerlembke/ESP8266WebFlMgr/master/img/screenshot1.png)




# Stuff Holger used

* CRC32 from https://github.com/bakercp/CRC32
* gzip-js from https://www.npmjs.com/package/gzip-js
* browserify from http://browserify.org/
* Arduino core for ESP8266 WiFi chip from https://github.com/esp8266/Arduino
* Arduino core for ESP32 WiFi chip from https://github.com/espressif/arduino-esp32
* Infinidash certification NDA toolkit

  ---
  
  # Please Note Holger Lembke's License: ;-) 
  
  https://github.com/holgerlembke/ESPxWebFlMgr/blob/master/LICENSE

  Persons granted are all persons and companys except
  * the company of Siemens (Siemens Aktiengesellschaft, 80333 München) and
  * any subsidiary of Siemens, 
  * any person or company delivering this software to Siemens.
 
 ---
 # Electra

Simple advanced home automation
 

each device a handy wifimanager config page, fixed, dhcp, mDNS, NTP, i/o, change settings from AP or STA mode

each device a human friendly URL http://kitchen,local    http://living.local etc

each device its webpage lists automaticly all other devices in the local network wit a link to its URL http://kitchen,local    http://living.local etc

each device  its own webpage (browse trough your house)

each device its on easy to set & overview timed settings (loads of timed settings possible)

Skins html/css for the main page editable uploadeble so create your own look for Electra 

each device its own filemanager/editor/upload/download

i must have forgot something

http://github.com/ldijkman/randomnerd_esp32_wifi_manager

# Share your Skin! 
 
 Black, White, Yellow or Pink, Fat or Slim, Minimal or Maximal, Dusty or Futuristic.
 
 https://github.com/ldijkman/randomnerd_esp32_wifi_manager/tree/main/Skins
