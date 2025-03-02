

#include <Arduino.h>
#include <inttypes.h>
#include <ESPxWebFlMgr.h>
#include <ESPxWebFlMgrWp.h>
#include <ESPxWebFlMgrWpF.h>

#include <CRC32.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <FS.h>
#endif

#ifdef ESP32
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <detail/RequestHandlersImpl.h>
#endif


String getContentType(const String& path) {
#ifdef ESP8266
  return mime::getContentType(path);
#endif
#ifdef ESP32
  return StaticRequestHandler::getContentType(path);
#endif
}


//*****************************************************************************************************
ESPxWebFlMgr::ESPxWebFlMgr(word port) {
  _Port = port;
}

//*****************************************************************************************************
ESPxWebFlMgr::~ESPxWebFlMgr() {
  end();
}

//*****************************************************************************************************
void ESPxWebFlMgr::begin() {
#ifdef ESP8266
  fileManager = new ESP8266WebServer(_Port);
#endif
#ifdef ESP32
  fileManager = new WebServer(_Port);
#endif

#ifdef fileManagerServerStaticsInternally
  fileManager->on("/", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerIndexpage, this));
  fileManager->on("/fm.css", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerCSS, this));
  fileManager->on("/fm.js", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerJS, this));
#endif
  fileManager->on("/bg.css", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerGetBackGround, this));

  fileManager->on("/i", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerFileListInsert, this));
  fileManager->on("/c", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerCommandExecutor, this));
  fileManager->on("/e", HTTP_GET, std::bind(&ESPxWebFlMgr::fileManagerFileEditorInsert, this));
  // file receiver with attached file to form
  fileManager->on("/r", HTTP_POST, std::bind(&ESPxWebFlMgr::fileManagerReceiverOK, this),
                  std::bind(&ESPxWebFlMgr::fileManagerReceiver, this));

  fileManager->onNotFound(std::bind(&ESPxWebFlMgr::fileManagerNotFound, this));

  fileManager->begin();
}

//*****************************************************************************************************
void ESPxWebFlMgr::end() {
  if (fileManager) {
    delete fileManager;
    fileManager = NULL;
  }
}

//*****************************************************************************************************
void ESPxWebFlMgr::handleClient() {
  if (fileManager) {
    fileManager->handleClient();
  }
}

//*****************************************************************************************************
void ESPxWebFlMgr::setViewSysFiles(bool vsf) {
  _ViewSysFiles = vsf;
}

//*****************************************************************************************************
bool ESPxWebFlMgr::getViewSysFiles(void) {
  return _ViewSysFiles;
}

//*****************************************************************************************************
void ESPxWebFlMgr::setSysFileStartPattern(String sfsp) {
  _SysFileStartPattern = sfsp;
}

//*****************************************************************************************************
String ESPxWebFlMgr::getSysFileStartPattern(void) {
  return _SysFileStartPattern;
}


//*****************************************************************************************************
// privates start here
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerGetBackGround(void) {
  fileManager->send(200, F("text/css"), ".background {background-color: "+_backgroundColor+";}");
}

//*****************************************************************************************************
void ESPxWebFlMgr::setBackGroundColor(const String backgroundColor) {
  _backgroundColor = backgroundColor;
}

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerNotFound(void) {
  String uri = fileManager->uri();

  if (uri == "/") {
    uri = "/fm.html";
  }

  String contentTyp = getContentType(uri);

  if (ESPxWebFlMgr_FileSystem.exists(uri)) {
    File f = ESPxWebFlMgr_FileSystem.open(uri, "r");
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
String ESPxWebFlMgr::dispIntDotted(size_t i) {
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
size_t ESPxWebFlMgr::totalBytes(void) {
#ifdef ESP8266
  FSInfo info;
  ESPxWebFlMgr_FileSystem.info(info);
  return info.totalBytes;
#endif

#ifdef ESP32
  return ESPxWebFlMgr_FileSystem.totalBytes();
#endif
}

//*****************************************************************************************************
size_t ESPxWebFlMgr::usedBytes(void) {
#ifdef ESP8266
  FSInfo info;
  ESPxWebFlMgr_FileSystem.info(info);
  return info.usedBytes;
#endif

#ifdef ESP32
  return ESPxWebFlMgr_FileSystem.usedBytes();
#endif
}

//*****************************************************************************************************
String ESPxWebFlMgr::dispFileString(size_t fs) {
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
void ESPxWebFlMgr::fileManagerIndexpage(void) {
  fileManager->send(200, F("text/html"), FPSTR(ESPxWebFlMgrWpindexpage));
  delay(1);
}

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerJS(void) {
  fileManager->send(200, F("text/javascript"), FPSTR(ESPxWebFlMgrWpjavascript));
  delay(1);
}

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerCSS(void) {
  fileManager->send(200, F("text/css"), FPSTR(ESPxWebFlMgrWpcss));
  delay(1);
}

//*****************************************************************************************************
String ESPxWebFlMgr::CheckFileNameLengthLimit(String fn) {
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
String ESPxWebFlMgr::colorline(int i) {
  if (i % 2 == 0) {
    return "ccu";
  } else {
    return "ccg";
  }
}

//*****************************************************************************************************
boolean ESPxWebFlMgr::allowAccessToThisFile(const String filename) {
  return ! filename.startsWith(_SysFileStartPattern);
}

//*****************************************************************************************************
//[make FS from esp8266 and esp32 compatible]**********************************************************
// this is the way MS DOS 3.x (?) did it with Int21 findfirst/findnext/findclose
#ifdef ESP8266
File ESPxWebFlMgr::nextFile(Dir &dir) {
  dir.next();
  return dir.openFile("r");
}
File ESPxWebFlMgr::firstFile(Dir &dir) {
  dir = ESPxWebFlMgr_FileSystem.openDir("/");
  return nextFile(dir);
}
#endif
#ifdef ESP32
#define Dir File
File ESPxWebFlMgr::nextFile(Dir &dir) {
  return dir.openNextFile();
}
File ESPxWebFlMgr::firstFile(Dir &dir) {
  dir = ESPxWebFlMgr_FileSystem.open("/","r");
  return nextFile(dir);
}
#endif

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerFileListInsert(void) {
  fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
  fileManager->send(200, F("text/html"), String());

  fileManager->sendContent(F("<div class=\"cc\"><div class=\"gc\">"));

  bool gzipperexists = ( (ESPxWebFlMgr_FileSystem.exists("/gzipper.js.gz")) || 
                         (ESPxWebFlMgr_FileSystem.exists("/gzipper.js")) );

  // List files
  int i = 0;
  Dir dir;
  File file = firstFile(dir);
  while (file) {
    String fn = file.name();
    /** /
      Serial.print("FN: >");
      Serial.print(fn);
      Serial.print("<");
      Serial.println();
      /**/
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      /*
        String fc = "<div id=\"file"+String(i)+"\" "
                  "data-filename=\""+fn+"\""
                  "class=\"ccl " + colorline(i) + "\""
                  "onclick=\"downloadfile('" + fn + "')\""
                  ">&nbsp;&nbsp;" + fn + "</div>";
      */

      String fc = "<div "
                  "class=\"ccl " + colorline(i) + "\""
                  "onclick=\"downloadfile('" + fn + "')\""
                  ">&nbsp;&nbsp;" + fn + "</div>";

      // File f = dir.openFile("r");
      fc += "<div class=\"cct " + colorline(i) + "\">&nbsp;" + dispIntDotted(file.size()) + "&nbsp;</div>";

      fc += "<div class=\"ccr " + colorline(i) + "\">&nbsp;"
            "<button title=\"Delete\" onclick=\"deletefile('" + fn + "')\" class=\"b\"><img src=\"--trash.png\" width=\"20\" height=\"20\"></button> "
            "<button title=\"Rename\" onclick=\"renamefile('" + fn + "')\" class=\"b\"><img src=\"--rename.png\" width=\"40\" height=\"18\"></button>&nbsp;&nbsp; ";

      // no gziped version and (zipper or gziped zipper) exists
      if ( (! (fn.endsWith(".gz")) ) && gzipperexists) {
        fc += "<button title=\"Compress\" onclick=\"compressurlfile('" + fn + "')\" class=\"b\"><img src=\"--zip.png\" width=\"20\" height=\"20\"></button> ";
      }
      // for editor
#ifndef fileManagerEditEverything
      String contentTyp = getContentType(fn);
      if ( (contentTyp.startsWith("text/")) ||
           (contentTyp.startsWith("application/j"))  ) // boldly assume: json, javascript and everything else is edible....
#endif
      {
        fc += "<button title=\"Edit\" onclick=\"editfile('" + fn + "')\" class=\"b\"><img src=\"--edit.png\" width=\"20\" height=\"20\"></button> ";
      }
      {
        fc += "<button title=\"View\" onclick=\"window.location.href='--iframe.html?file=" + fn + "';\" class=\"b\"><img src=\"--eye.png\" width=\"20\" height=\"20\"></button> ";
      } 
      fc += "&nbsp;&nbsp;</div>";

      fileManager->sendContent(fc);
      i++;
    }
    file = nextFile(dir);
  }

  // fileManager->sendContent("<span id=\"filecount\" data-count=\""+String(i)+"\"></span>");

  String sinfo = "&nbsp; Size: " +
                 dispFileString(totalBytes()) +
                 ", used: " +
                 dispFileString(usedBytes());
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
String ESPxWebFlMgr::escapeHTMLcontent(String html) {
  //html.replace("<","&lt;");
  //html.replace(">","&gt;");
  html.replace("&", "&amp;");

  return html;
}

// in place editor
//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerFileEditorInsert(void) {
  //Serial.println("Edit");

  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "edit") ) {

    String fn = "/"+fileManager->arg(0);
    if ( (! _ViewSysFiles) && (!allowAccessToThisFile(fn)) ) {
      fileManager->send(404, F("text/plain"), F("Illegal."));
      return;
    }

    fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
    fileManager->send(200, F("text/html"), String());

    fileManager->sendContent(ESPxWebFlMgrWpFormIntro);

    if (ESPxWebFlMgr_FileSystem.exists(fn)) {
      File f = ESPxWebFlMgr_FileSystem.open(fn, "r");
      if (f) {
        do {
          String l = f.readStringUntil('\n') + '\n';
          l = escapeHTMLcontent(l);
          fileManager->sendContent(l);
        } while (f.available());
        f.close();
      }
    }

    fileManager->sendContent(ESPxWebFlMgrWpFormExtro1);
    fileManager->sendContent(fn);
    fileManager->sendContent(ESPxWebFlMgrWpFormExtro2);

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
void ESPxWebFlMgr::fileManagerReceiverOK(void) {
  // Serial.println("fileManagerReceiverOK");
  fileManager->send(200);
  delay(1);
}

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerReceiver(void) {
//  Serial.println("fileManagerReceiver");

  HTTPUpload& upload = fileManager->upload();
//  Serial.println("Server upload Status: " + String(upload.status));

  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
//    Serial.print("handleFileUpload Name: ");
//    Serial.println(filename);

    if (! ( (_ViewSysFiles) || (allowAccessToThisFile(filename)) ) ) {
      filename = "/illegalfilename";
    }

    // cut length
    filename = CheckFileNameLengthLimit(filename);

    fsUploadFile = ESPxWebFlMgr_FileSystem.open(filename, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
//    Serial.print("handleFileUpload Data: ");
//    Serial.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      // fsUploadFile = NULL;
    }
//    Serial.print("handleFileUpload Size: ");
//    Serial.println(upload.totalSize);
  }
  delay(1);
}

struct __attribute__ ((__packed__)) zipFileHeader {
  uint32_t signature; // 0x04034b50;
  uint16_t versionneeded;
  uint16_t bitflags;
  uint16_t comp_method;
  uint16_t lastModFileTime;
  uint16_t lastModFileDate;
  uint32_t crc_32;
  uint32_t comp_size;
  uint32_t uncompr_size;
  uint16_t fname_len;
  uint16_t extra_field_len;
};

struct __attribute__ ((__packed__)) zipDataDescriptor {
  uint32_t signature; // 0x08074b50
  uint32_t crc32;
  uint32_t comp_size;
  uint32_t uncompr_size;
};

struct __attribute__ ((__packed__)) zipEndOfDirectory {
  uint32_t signature; // 0x06054b50;
  uint16_t nrofdisks;
  uint16_t diskwherecentraldirectorystarts;
  uint16_t nrofcentraldirectoriesonthisdisk;
  uint16_t totalnrofcentraldirectories;
  uint32_t sizeofcentraldirectory;
  uint32_t ofsetofcentraldirectoryrelativetostartofarchiv;
  uint16_t commentlength;
};

struct __attribute__ ((__packed__)) zipCentralDirectoryFileHeader {
  uint32_t signature; // 0x02014b50
  uint16_t versionmadeby;
  uint16_t versionneededtoextract;
  uint16_t flag;
  uint16_t compressionmethode;
  uint16_t lastModFileTime;
  uint16_t lastModFileDate;
  uint32_t crc_32;
  uint32_t comp_size;
  uint32_t uncompr_size;
  uint16_t fname_len;
  uint16_t extra_len;
  uint16_t comment_len;
  uint16_t diskstart;
  uint16_t internalfileattr;
  uint32_t externalfileattr;
  uint32_t relofsoflocalfileheader;
  // nun filename, extra field, comment
};

//*****************************************************************************************************
int ESPxWebFlMgr::WriteChunk(const char* b, size_t l) {
//  Serial.print(" Chunk: " + String(l) + " ");

  const char * footer = "\r\n";
  char chunkSize[11];
  sprintf(chunkSize, "%zx\r\n", l);
  fileManager->client().write(chunkSize, strlen(chunkSize));
  fileManager->client().write(b, l);
  fileManager->client().write(footer, 2);

  return strlen(chunkSize) + l + 2;
}

//*****************************************************************************************************
/* https://en.wikipedia.org/wiki/Zip_(file_format)
   https://www.fileformat.info/tool/hexdump.htm
   https://hexed.it/?hl=de
   HxD https://mh-nexus.de/de/

   This code needs some memory:
     4 * <nr. of files> + copybuffersize

   Uses no compression, because, well, code size. Should be good for 4mb.
*/
void ESPxWebFlMgr::getAllFilesInOneZIP(void) {
  const byte copybuffersize = 100;

  fileManager->setContentLength(CONTENT_LENGTH_UNKNOWN);
  // fileManager->sendHeader(F("Content-Type"), F("text/text"));
  // fileManager->sendHeader(F("Transfer-Encoding"), F("chunked"));
  // fileManager->sendHeader(F("Connection"), F("close"));
  fileManager->sendHeader(F("Content-Disposition"), F("attachment; filename=alles.zip"));
  fileManager->sendHeader(F("Content-Transfer-Encoding"), F("binary"));
  fileManager->send(200, F("application/octet-stream"), "");

  // Pass 0: count files
  int files = 0;
  {
    Dir dir;
    File file = firstFile(dir);
    while (file) {
      String fn = file.name();
      if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
        files++;
      }
      file = nextFile(dir);
    }
    // Serial.println("Files: "+String(files));
  }
  // Store the crcs
  uint32_t crc_32s[files];

  // Pass 1: local headers + file
  {
    zipFileHeader zip;
    zip.signature = 0x04034b50;
    zip.versionneeded = 0;
    zip.bitflags = 1 << 3;
    zip.comp_method = 0; // stored
    zip.lastModFileTime = 0x4fa5;
    zip.lastModFileDate = 0xe44e;
    zip.extra_field_len = 0;

    int i = 0;
    Dir dir;
    File file = firstFile(dir);
    while (file) {
      String fn = file.name();

      if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
        if (fn.indexOf("/")==0) {
          fn.remove(0,1); // "/" filenames beginning with "/" dont work for Windows....
        }  

        zip.comp_size = 0;
        zip.uncompr_size = 0;
        zip.crc_32 = 0;
        zip.fname_len = fn.length();
        WriteChunk((char*)&zip, sizeof(zip));
        WriteChunk(fn.c_str(), zip.fname_len);

//        Serial.print("Send: " + fn);
        // File f = dir.open("r",FILE_READ);
        int len = file.size();

        // send crc+len later...
        zipDataDescriptor datadiscr;
        datadiscr.signature = 0x08074b50;
        datadiscr.comp_size = len;
        datadiscr.uncompr_size = len;

        const char * footer = "\r\n";
        char chunkSize[11];
        sprintf(chunkSize, "%zx\r\n", len);
        fileManager->client().write(chunkSize, strlen(chunkSize));

        { // pff.
          CRC32 crc;
          byte b[copybuffersize];
          int lenr = len;
          while (lenr > 0) {
            byte r = (lenr > copybuffersize) ? copybuffersize : lenr;
            file.read(b, r);
            crc.update(b, r);
            fileManager->client().write(b,r);
            lenr -= r;
          }
          datadiscr.crc32 = crc.finalize();
          crc_32s[i] = datadiscr.crc32;
        }

        fileManager->client().write(footer, 2);

        WriteChunk((char*)&datadiscr, sizeof(datadiscr));

        // f.close();
        i++;
/** /
        Serial.print(" ");
        Serial.print(l);
        Serial.println();
/**/
      }
      file = nextFile(dir);
    }
  }

  // Pass 2: Central directory Structur
  {
    zipEndOfDirectory eod;
    eod.signature =  0x06054b50;
    eod.nrofdisks = 0;
    eod.diskwherecentraldirectorystarts = 0;
    eod.nrofcentraldirectoriesonthisdisk = 0;
    eod.totalnrofcentraldirectories = 0;
    eod.sizeofcentraldirectory = 0;
    eod.ofsetofcentraldirectoryrelativetostartofarchiv = 0;
    eod.commentlength = 0;

    zipCentralDirectoryFileHeader CDFH;

    CDFH.signature = 0x02014b50;
    CDFH.versionmadeby = 0;
    CDFH.versionneededtoextract = 0;
    CDFH.flag = 0;
    CDFH.compressionmethode = 0; // Stored
    CDFH.lastModFileTime = 0x4fa5;
    CDFH.lastModFileDate = 0xe44e;
    CDFH.extra_len = 0;
    CDFH.comment_len = 0;
    CDFH.diskstart = 0;
    CDFH.internalfileattr = 0x01;
    CDFH.externalfileattr = 0x20;
    CDFH.relofsoflocalfileheader = 0;

    int i = 0;

    Dir dir;
    File file = firstFile(dir);
    while (file) {
      String fn = file.name();

      if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
        if (fn.indexOf("/")==0) {
          fn.remove(0,1); // "/" filenames beginning with "/" dont work for Windows....
        }  
//        Serial.print("CDFH: " + fn);
        // File f = dir.open("r",FILE_READ);
        int len = file.size();

        CDFH.comp_size = len;
        CDFH.uncompr_size = len;
        CDFH.fname_len = fn.length();
        CDFH.crc_32 = crc_32s[i];

        // f.close();

        WriteChunk((char*)&CDFH, sizeof(CDFH));
        WriteChunk(fn.c_str(), CDFH.fname_len);

        int ofs = sizeof(zipFileHeader) + len + CDFH.fname_len + sizeof(zipDataDescriptor);

        // next position
        CDFH.relofsoflocalfileheader += ofs;

        // book keeping
        eod.nrofcentraldirectoriesonthisdisk++;
        eod.totalnrofcentraldirectories++;
        eod.ofsetofcentraldirectoryrelativetostartofarchiv += ofs;
        eod.sizeofcentraldirectory += sizeof(CDFH) + CDFH.fname_len;

        i++;
      }
      file = nextFile(dir);
    }

//    Serial.print("EOD: ");
    WriteChunk((char*)&eod, sizeof(eod));
//    Serial.println();
  }

  const char * endchunk = "0\r\n\r\n";
  fileManager->client().write(endchunk, 5);

  fileManager->sendContent("");
  delay(1);
}

//*****************************************************************************************************
void ESPxWebFlMgr::fileManagerCommandExecutor(void) {
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
  // one arg, "za", zip all and download
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "za") ) {
    getAllFilesInOneZIP();
    // does it all
    return;
  }

  // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "dwn", download
  // must happen in the context of the webpage... thus via "window.location.href="/c?dwn="+filename;"
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "dwn") ) {
    String fn = fileManager->arg(0);
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      File f = ESPxWebFlMgr_FileSystem.open(fn, "r");
      if (f) {
        fileManager->sendHeader(F("Content-Type"), F("text/text"));
        fileManager->sendHeader(F("Connection"), F("close"));
        if (fn.indexOf("/")==0) {
          fileManager->sendHeader(F("Content-Disposition"), "attachment; filename=" + fn.substring(1));
        } else {
          fileManager->sendHeader(F("Content-Disposition"), "attachment; filename=" + fn);
        }
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
      ESPxWebFlMgr_FileSystem.remove("/"+fn);    // Add slash
    }
  }
  
   // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, showhidden= 0 or 1
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "showhidden") ) {
    String fn = fileManager->arg(0);
    Serial.print("showhidden=");Serial.println(fn);
    if(fn=="0")setSysFileStartPattern("\\");
    if(fn=="1")setSysFileStartPattern("\--");
  }
  
  //////////////////////////////////////////////////////////////////////////////////////
    // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "update", update
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "update") ) {
    String fn = fileManager->arg(0);
    //if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      //ESPxWebFlMgr_FileSystem.remove("/"+fn);    // Add slash
    //}
    
    Serial.println("Update Electra");
    
    File file = LittleFS.open("/ESP8266basic.ino.bin", "r");
    
    if (!file)
    {
        Serial.println(PSTR("Failed to open file for reading"));
       // answer = 0;
    }
  
    size_t fileSize = file.size();

        if (!Update.begin(fileSize))
        {
            Serial.println(PSTR("Not enough space for update"));
        }  
        
     Serial.println("Update start from /ESP8266basic.ino.bin");
     Serial.println("Please Wait");
     Update.writeStream(file);
        
          if (Update.end())
            {
                Serial.println("Succesful update");
                Serial.println("reboot now");
                Serial.println("");
                ESP.restart();
            }
            else
            {

                Serial.println(PSTR("Error Occurred: ") + String(Update.getError()));
            }
        

        file.close();
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////

  // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "ren", rename
  if ( (fileManager->args() == 2) && (fileManager->argName(0) == "ren") ) {
    String fn = fileManager->arg(0);
    if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      String fn2 = CheckFileNameLengthLimit(fileManager->arg(1));
      if ( (_ViewSysFiles) || (allowAccessToThisFile(fn2)) ) {
        ESPxWebFlMgr_FileSystem.rename("/"+fn, "/"+fn2);
      }
    }
  }
  


/////////////////////////////////////////////////////////////////////////////  
    // +--++--++--++--++--++--++--++--++--++--++--++--++--++--++--+
  // one arg, "newfile", r
  if ( (fileManager->args() == 1) && (fileManager->argName(0) == "new") ) {
    String fn = fileManager->arg(0);
    Serial.print("file create ");Serial.println(fn);
    //if ( (_ViewSysFiles) || (allowAccessToThisFile(fn)) ) {
      //String fn = CheckFileNameLengthLimit(fileManager->arg(1));
      //if ( (_ViewSysFiles) || (allowAccessToThisFile(fn2)) ) {
        ESPxWebFlMgr_FileSystem.open("/"+fn, "w");                  // no safety checks
        File file = LittleFS.open("/"+fn, "w");                     // no safety checks
        file.print("Electra was Here ! ;-)");                       // zip does not like empty files dos not unzip
        
      //}    
    //}
  }
////////////////////////////////////////////////////////////////////////////
  
  
  
  


  // dummy answer
  fileManager->send(200, "text/plain", "");
  delay(1);
}

