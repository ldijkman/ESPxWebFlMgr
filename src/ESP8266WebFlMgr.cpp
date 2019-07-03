#include <Arduino.h>
#include <inttypes.h>
#include <ESP8266WebFlMgr.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <FS.h>

// getting access to the nice mime-type-table and getContentType()
#include "detail/RequestHandlersImpl.h"

//*****************************************************************************************************
ESP8266WebFlMgr::ESP8266WebFlMgr(word port) {
  _Port = port;
}

//*****************************************************************************************************
ESP8266WebFlMgr::~ESP8266WebFlMgr() {
  end();
}

//*****************************************************************************************************
void ESP8266WebFlMgr::begin() {
  fileManager = new ESP8266WebServer(_Port);

#ifdef fileManagerServerStaticsInternally
  fileManager->on("/", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerIndexpage, this));
  fileManager->on("/fm.css", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerCSS, this));
  fileManager->on("/fm.js", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerJS, this));
#endif

  fileManager->on("/i", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerFileListInsert, this));
  fileManager->on("/c", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerCommandExecutor, this));
  fileManager->on("/e", HTTP_GET, std::bind(&ESP8266WebFlMgr::fileManagerFileEditorInsert, this));
  // file receiver with attached file to form
  fileManager->on("/r", HTTP_POST, std::bind(&ESP8266WebFlMgr::fileManagerReceiverOK, this),
                                   std::bind(&ESP8266WebFlMgr::fileManagerReceiver, this));

  fileManager->onNotFound(std::bind(&ESP8266WebFlMgr::fileManagerNotFound, this));

  fileManager->begin();
}

//*****************************************************************************************************
void ESP8266WebFlMgr::end() {
  if (fileManager) {
    delete fileManager; 	
    fileManager=NULL;
  }
}

//*****************************************************************************************************
void ESP8266WebFlMgr::handleClient() {
  if (fileManager) {
    fileManager->handleClient();
  }
}

//*****************************************************************************************************
void ESP8266WebFlMgr::setViewSysFiles(bool vsf) {
  _ViewSysFiles = vsf;
}

//*****************************************************************************************************
bool ESP8266WebFlMgr::getViewSysFiles(void) {
  return _ViewSysFiles;
}

//*****************************************************************************************************
void ESP8266WebFlMgr::setSysFileStartPattern(String sfsp) {
  _SysFileStartPattern = sfsp;
}

//*****************************************************************************************************
String ESP8266WebFlMgr::getSysFileStartPattern(void) {
  return _SysFileStartPattern;
}


//*****************************************************************************************************
// privates start here
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerNotFound(void) {
  String uri = fileManager->uri();

  if (uri == "/") {
    uri = "/fm.html";
  }

  String contentTyp = StaticRequestHandler::getContentType(uri);

  if (SPIFFS.exists(uri)) {
    File f = SPIFFS.open(uri, "r");
    if (f) {
      if (fileManager->streamFile(f, contentTyp) != f.size()) {
        // Serial.println(F("Sent less data than expected!"));
        // We should panic a little bit.
      }
      f.close();
    }
  } else
  {
    fileManager->send(404, F("text/plain"), F("URI not found."));
  }
}

//*****************************************************************************************************
String ESP8266WebFlMgr::dispIntDotted(size_t i) {
  String res = "";
  while (i != 0) {
    int r = i % 1000;
    res = String(i % 1000) + res;
    i /= 1000;
    if ( (r < 100) && (i > 0) ) {
      res = "0" + res;
      if (r < 10) {
        res = "0" + res;
      }
    }
    if (i != 0) {
      res = "." + res;
    }
  }
  return res;
}

//*****************************************************************************************************
String ESP8266WebFlMgr::dispFileString(size_t fs) {
  if (fs < 0) {
    return "-0";
  }

  if (fs == 0) {
    return "0 B";
  }

  if (fs < 1000) {
    return String(fs) + " B";
  }

  String units[] = { "B", "kB", "MB", "GB", "TB" };
  int digitGroups = (int) (log10(fs) / log10(1024));
  return String(fs / pow(1024, digitGroups)) + " " + units[digitGroups] + " <small>(" + dispIntDotted(fs) + " B)</small>";
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerIndexpage(void) {
  fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
  fileManager->send(200, F("text/html"), String());

  //++++++++++++++++++++++
  static const char s1[] PROGMEM = R"==x==(
<!DOCTYPE html>
<html lang="en">
  <head>
    <link rel="stylesheet" type="text/css" href="/fm.css">
    <script src="/fm.js"></script>  
    <script src="/gzipper.js"></script>  
    <title>FileManager</title>
  </head>
  <body>
    <div id="gc">
        <div class="o1">&nbsp;</div>
        <div class="o2">&nbsp;</div>
        <div class="o3"><nobr id="oo3">&nbsp;</nobr></div>
        <div class="o4">&nbsp;</div>

        <div class="m1">
            <div class="s11">&nbsp;</div>
            <div class="s12">
              <div class="s13">&nbsp;</div>
            </div>
        </div>
        <div class="m2" ondrop="dropHandler(event);" ondragover="dragOverHandler(event);">
          File<br />
          Drop<br />
          Zone<br />
        </div>
        <div class="m3">
            <div class="s31">&nbsp;</div>
            <div class="s32">
              <div class="s33">&nbsp;</div>
            </div>
        </div>

        <div class="u1">&nbsp;</div>
        <div class="u2">&nbsp;</div>
        <div class="u3" id="msg">&nbsp;</div>
        <div class="u4">&nbsp;</div>
        <div class="c" id="fi">
          File list should appear here.
        </div>
    </div>
  </body>
</html>  

  )==x==";

  fileManager->sendContent(FPSTR(s1));
  fileManager->sendContent("");
  delay(1);
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerJS(void) {
  static const char s1[] PROGMEM = R"==x==(

function compressurlfile(source) {
  document.getElementById('msg').innerHTML = "Fetching file...";
  var request = new XMLHttpRequest();
  request.onreadystatechange = function () {
      var DONE = this.DONE || 4;
      if (this.readyState === DONE) {
          var data = this.responseText;
          var gzip = require('gzip-js'),options={level: 9,name: source,timestamp: parseInt(Date.now()/1000, 10)};
          var out = gzip.zip(data, options);
          var bout = new Uint8Array(out); // out is 16 bits...
          
          document.getElementById('msg').innerHTML = "Sending compressed file...";
          var sendback = new XMLHttpRequest(); 
          sendback.onreadystatechange = function () {
            var DONE = this.DONE || 4;
            if (this.readyState === DONE) {
                getfileinsert();
            }
          };
          sendback.open('POST', '/r'); 
          var formdata = new FormData();  
          var blob = new Blob([bout], {type: "application/octet-binary"});
          formdata.append(source+'.gz', blob, source+'.gz');
          sendback.send(formdata);    
      }
  };
  request.open('GET', source, true);
  request.send(null);
}

function getfileinsert() {
  var request = new XMLHttpRequest();
  request.onreadystatechange = function () {
      var DONE = this.DONE || 4;
      if (this.readyState === DONE) {
		  var res = this.responseText.split("##");
          document.getElementById('fi').innerHTML = res[0];
          document.getElementById("oo3").innerHTML = res[1];
          document.getElementById('msg').innerHTML = "";
      }
  };
  request.open('GET', '/i', true);
  request.send(null);
}

function executecommand(command) {
    var xhr = new XMLHttpRequest(); 
    xhr.onreadystatechange = function () {
      var DONE = this.DONE || 4;
      if (this.readyState === DONE) {
          getfileinsert();
      }
    };
    xhr.open('GET', '/c?'+command, true);
    xhr.send(null);    
}

function downloadfile(filename) {
   window.location.href="/c?dwn="+filename;
}

function deletefile(filename) {
  if (confirm("Really delete "+filename)) {
    document.getElementById('msg').innerHTML = "Please wait. Delete in progress...";
    executecommand("del="+filename);
  }
}

function renamefile(filename) {
  var newname = prompt("new name for "+filename,filename);
  if (newname != null) {
    document.getElementById('msg').innerHTML = "Please wait. Rename in progress...";
    executecommand("ren="+filename+"&new="+newname);
  }
}

var editxhr;

function editfile(filename) {
  document.getElementById('msg').innerHTML = "Please wait. Creating editor...";

  editxhr = new XMLHttpRequest();
  editxhr.onreadystatechange = function () {
      var DONE = this.DONE || 4;
      if (this.readyState === DONE) {
          document.getElementById('fi').innerHTML = editxhr.responseText;
          document.getElementById("oo3").innerHTML = "Edit "+filename;
          document.getElementById('msg').innerHTML = "";
      }
  };
  editxhr.open('GET', '/e?edit='+filename, true);
  editxhr.send(null);
}

function sved(filename) {
  var content=document.getElementById('tect').value;
  // utf-8
  content = unescape(encodeURIComponent(content));
  
  var xhr = new XMLHttpRequest();

  xhr.open("POST", "/r", true);
  
  var boundary = '-----whatever';
  xhr.setRequestHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

  var body = "" +     
      '--' + boundary + '\r\n' + 
      'Content-Disposition: form-data; name="uploadfile"; filename="' + filename + '"' + '\r\n' + 
      'Content-Type: text/plain' + '\r\n' + 
      '' + '\r\n' + 
      content + '\r\n' + 
      '--' + boundary + '--\r\n' + 
      ''; 

  // ajax does not do xhr.setRequestHeader("Content-length", body.length);

  xhr.onreadystatechange = function () {
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      getfileinsert();
    }
  }

  xhr.send(body);  
}

function abed() {
  getfileinsert();
}

function uploadFile(file) {
    var xhr = new XMLHttpRequest(); 
    xhr.onreadystatechange = function () {
      var DONE = this.DONE || 4;
      if (this.readyState === DONE) {
          getfileinsert();
      }
    };
    xhr.open('POST', '/r'); 
    var formdata = new FormData();  
    formdata.append('uploadfile', file);
    xhr.send(formdata);    
}

function dropHandler(ev) {
  console.log('File(s) dropped');

  document.getElementById('msg').innerHTML = "Please wait. Transferring file...";

  // Prevent default behavior (Prevent file from being opened)
  ev.preventDefault();

  if (ev.dataTransfer.items) {
    // Use DataTransferItemList interface to access the file(s)
    for (var i = 0; i < ev.dataTransfer.items.length; i++) {
      // If dropped items aren't files, reject them
      if (ev.dataTransfer.items[i].kind === 'file') {
        var file = ev.dataTransfer.items[i].getAsFile();
        uploadFile(file);
        console.log('.1. file[' + i + '].name = ' + file.name);
      }
    }
  } else {
    // Use DataTransfer interface to access the file(s)
    for (var i = 0; i < ev.dataTransfer.files.length; i++) {
      console.log('.2. file[' + i + '].name = ' + ev.dataTransfer.files[i].name);
    }
  }
}

function dragOverHandler(ev) {
  console.log('File(s) in drop zone'); 

  // Prevent default behavior (Prevent file from being opened)
  ev.preventDefault();
}

window.onload=getfileinsert;


  )==x==";

  fileManager->send(200, F("text/javascript"), FPSTR(s1));
  delay(1);
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerCSS(void) {
  static const char s1[] PROGMEM = R"==g==(

div {
  margin: 1px;
  padding: 0px;
  font-family: 'Segoe UI', Verdana, sans-serif;
}

#gc {
  display: grid;
  grid-template-columns: 80px 25% auto 30px;
  grid-template-rows: 20px 30px auto 30px 20px;
  grid-template-areas: "o1 o2 o3 o4" "m1 c c c" "m2 c c c" "m3 c c c" "u1 u2 u3 u4";
}

.o1 {
  grid-area: o1;
  background-color: #9999CC;
  border-top-left-radius: 20px;
  margin-bottom: 0px;
}

.o2 {
  grid-area: o2;
  background-color: #9999FF;
  margin-bottom: 0px;
}

.o3 {
  grid-area: o3;
  background-color: #CC99CC;
  margin-bottom: 0px;
}

.o4 {
  grid-area: o4;
  background-color: #CC6699;
  border-radius: 0 10px 10px 0;
  margin-bottom: 0px;
}

.m1 {
  grid-area: m1;
  margin-top: 0px;
  background-color: #9999CC;
  display: grid;
  grid-template-columns: 60px 20px;
  grid-template-rows: 20px;
  grid-template-areas: "s11 s12";  
}

.s12 {
  margin: 0px;
  background-color: #9999CC;
}

.s13 {
  margin: 0px;
  border-top-left-radius: 20px;
  background-color: white;  
  height: 30px;
}

.m2 {
  display: flex;
  justify-content: center; 
  align-items: center;
  grid-area: m2;
  background-color: #CC6699;
  width: 60px;
}

.m3 {
  grid-area: m3;
  margin-bottom: 0px;
  background-color: #9999CC;
  display: grid;
  grid-template-columns: 60px 20px;
  grid-template-rows: 20px;
  grid-template-areas: "s31 s32";  
}

.s32 {
  margin: 0px;
  background-color: #9999CC;
}

.s33 {
  margin: 0px;
  border-bottom-left-radius: 20px;
  background-color: white;  
  height: 30px;
}

.u1 {
  grid-area: u1;
  background-color: #9999CC;
  border-bottom-left-radius: 20px;
  margin-top: 0px;
}

.u2 {
  grid-area: u2;
  background-color: #CC6666;
  margin-top: 0px;
}

.u3 {
  grid-area: u3;
  background-color: #FF9966;
  margin-top: 0px;
}

.u4 {
  grid-area: u4;
  background-color: #FF9900;
  border-radius: 0 10px 10px 0;
  margin-top: 0px;
}

.c {
  grid-area: c;
}

#fi .b {
  background-color: Transparent;
  border: 1px solid #9999FF;  
  border-radius: 1px;
  padding: 0px;
  width: 30px;
  cursor: pointer;
}

#fi .b:hover {
  background-color: #9999FF;
  color: white;
}

.cc {
  width: min-content;
  margin: 10px 0px;
}

.gc div {
  padding: 1px;  
}

.ccg, ccu {
  height: 1.5em;
}

.ccg {
  background-color: #A5A5FF;
}

.ccu {
  background-color: #FE9A00;
}

.ccl {
  border-radius: 5px 0 0 5px;
  cursor: pointer;
}

.ccl:hover {
  border-radius: 5px 0 0 5px;
  color: white;
  cursor: pointer;
}

.ccr {
  border-radius: 0 5px 5px 0;
}

.cct {
  text-align: right;
}

.gc {
  display: grid;
  grid-template-columns: repeat(3, max-content);  
}  

  )==g==";

  fileManager->send(200, F("text/css"), FPSTR(s1));
  delay(1);
}

//*****************************************************************************************************
String ESP8266WebFlMgr::CheckFileNameLengthLimit(String fn) {
  // SPIFFS file name limit. Is there a way to get the max length from SPIFFS/LittleFS?
  //                                      SPIFFS_OBJ_NAME_LEN is spiffs.... but not very clean.
  if (fn.length() > 32) {
    int len = fn.length();
    fn.remove(29);
    fn += String(len);
  }

  return fn;
}

//*****************************************************************************************************
String ESP8266WebFlMgr::colorline(int i) {
  if (i % 2 == 0) {
    return "ccu";
  } else {
    return "ccg";
  }
}

//*****************************************************************************************************
boolean ESP8266WebFlMgr::allowAccessToThisFile(const String filename) {
  return ! filename.startsWith(_SysFileStartPattern);
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerFileListInsert(void) {
  fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
  fileManager->send(200, F("text/html"), String());

  fileManager->sendContent(F("<div class=\"cc\"><div class=\"gc\">"));

  // List files
  Dir dir = SPIFFS.openDir("/");
  int i = 0;
  while (dir.next()) {
    String fn = dir.fileName();
    /** /
      Serial.print("FN: >");
      Serial.print(fn);
      Serial.print("<");
      Serial.println();
      /**/
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      String fc = "<div class=\"ccl " + colorline(i) + "\""
                  "onclick=\"downloadfile('" + fn + "')\""
                  ">&nbsp;&nbsp;" + fn + "</div>";

      File f = dir.openFile("r");
      fc += "<div class=\"cct " + colorline(i) + "\">&nbsp;" + dispIntDotted(f.size()) + "&nbsp;</div>";
      f.close();

      fc += "<div class=\"ccr " + colorline(i) + "\">&nbsp;"
            "<button title=\"Delete\" onclick=\"deletefile('" + fn + "')\" class=\"b\">D</button> "
            "<button title=\"Rename\" onclick=\"renamefile('" + fn + "')\" class=\"b\">R</button> ";

      // no gziped version and (zipper or gziped zipper) exists
      if ( (! (fn.endsWith(".gz")) ) &&
           ((SPIFFS.exists("/gzipper.js.gz")) || (SPIFFS.exists("/gzipper.js"))) ) {
        fc += "<button title=\"Compress\" onclick=\"compressurlfile('" + fn + "')\" class=\"b\">C</button> ";
      }
      // for editor
#ifndef fileManagerEditEverything
      String contentTyp = StaticRequestHandler::getContentType(fn);
	  if ( (contentTyp.startsWith("text/")) ||
	       (contentTyp.startsWith("application/j"))  ) // json, javascript and everything else....
#endif		   
	  {
        fc+="<button title=\"Edit\" onclick=\"editfile('" + fn + "')\" class=\"b\">E</button> ";
	  }

      fc += "&nbsp;&nbsp;</div>";

      fileManager->sendContent(fc);
      i++;
    }
  }
  //SPIFFS statistik
  FSInfo info;
  SPIFFS.info(info);

  String sinfo = "&nbsp; Size: " +
                 dispFileString(info.totalBytes) +
                 ", used: " +
                 dispFileString(info.usedBytes);
  /*
    fileManager->sendContent(F(" FS blocksize: "));
    fileManager->sendContent(String(info.blockSize));
    fileManager->sendContent(F(", pageSize: "));
    fileManager->sendContent(String(info.pageSize));
  */
  fileManager->sendContent(F("</div></div>"));

  fileManager->sendContent(F("##"));
  fileManager->sendContent(sinfo);

  fileManager->sendContent("");
  delay(1);
}

//*****************************************************************************************************
String ESP8266WebFlMgr::escapeHTMLcontent(String html) {
	//html.replace("<","&lt;");
	//html.replace(">","&gt;");
	html.replace("&","&amp;");
	
	return html;
}

// in place editor
//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerFileEditorInsert(void) {
  // Serial.println("Edit");

  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "edit") ) {
	  
    String fn = fileManager->arg(0);
    if ( (! _ViewSysFiles) && (!allowAccessToThisFile(fn)) ) {
      fileManager->send(404, F("text/plain"), F("Illegal."));
	  return;
    }
 
    fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
    fileManager->send(200, F("text/html"), String());

	fileManager->sendContent(F("<form><textarea id=\"tect\" rows=\"25\" cols=\"80\">"));

    /*
    if (SPIFFS.exists(fn)) {
      Serial.println("FN send");
      File f = SPIFFS.open(fn, "r");
      if (f) {
        fileManager->client().write(f);
        f.close();
      }
	}
	*/
    if (SPIFFS.exists(fn)) {
      File f = SPIFFS.open(fn, "r");
      if (f) {
        do {
          String l = f.readStringUntil('\n')+'\n'; 
		  l = escapeHTMLcontent(l);
          fileManager->sendContent(l);
		} while (f.available());
        f.close();
      }
	}

    fileManager->sendContent(F("</textarea></form><button title=\"Save file\" onclick='sved(\""));
	fileManager->sendContent(fn);
	fileManager->sendContent(F("\");' >Save</button>"));
    fileManager->sendContent(F("&nbsp;<button title=\"Abort editing\" onclick=\"abed();\" >Abort editing</button>"));
    //fileManager->sendContent(F("</form>"));
	
    fileManager->sendContent(F("<script id=\"info\">document.getElementById('oo3').innerHTML = \""));
    fileManager->sendContent("File: ");
    fileManager->sendContent(F("\";</script>"));

    fileManager->sendContent("");
  } else {
    fileManager->send(404, F("text/plain"), F("Illegal."));
  }

  delay(1);
}

// Drag and Drop
//   https://developer.mozilla.org/en-US/docs/Web/API/HTML_Drag_and_Drop_API/File_drag_and_drop
//   https://www.ab-heute-programmieren.de/drag-and-drop-upload-mit-html5/#Schritt_3_Eine_Datei_hochladen
//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerReceiverOK(void) {
  // Serial.println("fileManagerReceiverOK");	
  fileManager->send(200);
  delay(1);
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerReceiver(void) {
  //Serial.println("fileManagerReceiver");	

  HTTPUpload& upload = fileManager->upload();
  // Serial.println("Server upload Status: " + String(upload.status));

  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    // Serial.print("handleFileUpload Name: ");
    // Serial.println(filename);

    if (! ( (_ViewSysFiles) || (allowAccessToThisFile(filename)) ) ) {
      filename = "/illegalfilename";
    }

    // cut length
    filename = CheckFileNameLengthLimit(filename);

    fsUploadFile = SPIFFS.open(filename, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // Serial.print("handleFileUpload Data: ");
    // Serial.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      // fsUploadFile = NULL;
    }
    // Serial.print("handleFileUpload Size: ");
    // Serial.println(upload.totalSize);
  }
  delay(1);
}

//*****************************************************************************************************
void ESP8266WebFlMgr::fileManagerCommandExecutor(void) {
	// https://www.youtube.com/watch?v=KSxTxynXiBs
  /** /
    for (uint8_t i = 0; i < fileManager->args(); i++) {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(fileManager->argName(i));
    Serial.print(": ");
    Serial.print(fileManager->arg(i));
    Serial.println();
    }
    /**/

  // no Args: DIE!
  if (fileManager->args() == 0) {
    return;
  }

  // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "dwn", download
  // must happen in the context of the webpage... thus via "window.location.href="/c?dwn="+filename;"
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "dwn") ) {
    String fn = fileManager->arg(0);
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      File f = SPIFFS.open(fn, "r");
      if (f) {
        fileManager->sendHeader(F("Content-Type"), F("text/text"));
        fileManager->sendHeader(F("Connection"), F("close"));
        fileManager->sendHeader(F("Content-Disposition"), "attachment; filename=" + fn.substring(1));
        fileManager->sendHeader(F("Content-Transfer-Encoding"), F("binary"));
        if (fileManager->streamFile(f, "application/octet-stream") != f.size()) {
          Serial.println(F("Sent less data than expected!"));
        }
        f.close();
        return;
      }
    }
  }

  // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "del", delete
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "del") ) {
    String fn = fileManager->arg(0);
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      SPIFFS.remove(fn);
    }
  }

  // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "ren", rename
  if ( (fileManager->args() == 2) && (fileManager->argName(0) == "ren") ) {
    String fn = fileManager->arg(0);
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      String fn2 = CheckFileNameLengthLimit(fileManager->arg(1));
      if ( (_ViewSysFiles) || (allowAccessToThisFile(fn2)) ) {
        SPIFFS.rename(fn, fn2);
      }
    }
  }

  // dummy answer
  fileManager->send(200, "text/plain", "");
  delay(1);
}

