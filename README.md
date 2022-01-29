## NOTE: issue on the persistance of credentials in the core3.x of 8266, which does not work
## https://github.com/ldijkman/ESPxWebFlMgr/issues/3

---


<b>SIEMENS</b> related, People first read the license at the bottom ?!

# 100 &euro;uro for the first that gives a working code 

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



## Usage
* Click on the filename to download the file.
* D is delete 
* R is rename
* E is in-place-edit
* C is Web-compatible GZIP compress. (luberth=>but what can we do with gzip???)
* V is PreView
* Drop one or multiple file onto the drop zone to upload.
* Click on "Download all files" got get the entire file system content as one big ZIP file. 


### "build in web page" 

The file manager comes in two flavors. This is the default modus: just use it. Everything works automatically.

### "external web page"

For those who need to save about 10k code space the web page can also be moved into the file system space. Copy the content of
the folder __filemanager__ on the device and comment out the line __#define fileManagerServerStaticsInternally__

### File System: LittleFS or SPIFFS

ESPxWebFlMgr should be full LittleFS by now. Report any SPIFFS zombies.

<del>ESPxWebFlMgr is currently in a transition phase from SPIFFS to LittleFS. Current status: ESP8266 is default LitteFS with fallback to SPIFFS, ESP32 SPIFFS only (I assume it is easy to switch, but I didn't do any tests...)<del>

### Build in GZIPPER

The GZIPPER can compress the files in you devices file system into gzip-web-compatible files.

To use the "on the fly" GZIPPER you need to upload the file "gzipper.js" from "gzip files" folder. Click the "C"-button to compress a file. If the compression factor meets your expectations delete the source file.

## System files

Because the file system is feature limited there is no real concept for separating "internal files" from "web server allowed files".

My solution of choice is that all internal files start with "/." (slash dot). Your idea might differ.

Use __.setSysFileStartPattern(String)__ to define a pattern that identifies internal files.

Use __.setViewSysFiles(bool)__ to choose whether the file manager shows these files or not.

## The Editor

It is basic. It works. You. will. create. backups. before. editing!

# Examples

## Example __basic__ (ESP8266 only)

It shows the use at its simplest. 

Intended use: putting configuration files on your ESP8266 and download data files without any ado.

## Example __basiconoff__ (ESP8266 only)

Some sort of real life demand: Turn off the file manager after 180 seconds.

## Example __basicwsagzip__ (ESP8266 and ESP32)

More real life:
* implements a file based web server (cut and paste ready on its own sketch tab)
* allows serving gzipped files

![this is it](https://raw.githubusercontent.com/holgerlembke/ESP8266WebFlMgr/master/img/screenshot2.png)

WARNING: The webserver has no access control. It servers everything from your ESP8266 file system.

Put the sample files from "simplewebpage" for a simple web page on the server.

# Stuff I used

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

