// inline guard. Did I mention that c/c++ is broken by design?
#ifndef ESPxWebFlMgrWp_h
#define ESPxWebFlMgrWp_h

// this file has been created by makeESPxWebFlMgrWp\do.cmd

//*****************************************************************************************************
static const char ESPxWebFlMgrWpindexpage[] PROGMEM = R"==x==(
<!DOCTYPE html>
<html  lang="en">
  <head>
    <title>FileManager</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta name="copyright" content="Dirk Luberth Dijkman Bangert 30 Andijk The Netherlands, https://github.com/ldijkman/ESPxWebFlMgr" />
    <link rel="icon" type="image/png" href="favicon.png">
    <link rel="icon" sizes="192x192" href="Electra_192x192.png">
    <link rel="stylesheet" type="text/css" href="/bg.css">
    <link rel="stylesheet" type="text/css" href="/fm.css">
    <script src="/fm.js"></script> 
    <script src="/gzipper.js"></script>
    <!-- CodeMirror -->
    <link rel="stylesheet" href="--codemirror.css">
    <link rel="stylesheet" href="--night.css">
    <script src="--codemirror_compressed.js"></script>
    <script src="--xml.js"></script>
    <!-- CodeMirror -->
  </head>
  <body class="background">
    <div id="gc">
        <div class="o1">&nbsp;</div>
        <div class="o2">&nbsp;&nbsp;<a href="http://github.com/ldijkman/randomnerd_esp32_wifi_manager" target="Electra">&#8623; Electra's FileManager &#8623;</a>&nbsp;</div>
        <div class="o3" id="o3">&nbsp;</div>
        <div class="o4">&nbsp;</div>
        <div class="m1">
            <div class="s11">&nbsp;</div>
            <div class="s12">
            <div class="s13 background">&nbsp;</div>
            </div>
        </div>
        <div class="m2" ondrop="dropHandler(event);" ondragover="dragOverHandler(event);" onclick="fileopen();">
        <input type="file" id="fileinput" hidden/>
           <img src="--filedrop.png" width="70" height="70" title="dragg files here to upload files">
          <br>
       
        </div>
        <div class="m3">
            <div class="s31">&nbsp;</div>
            <div class="s32">
            <div class="s33 background">&nbsp;</div>
            </div>
        </div>

        <div class="u1">&nbsp;</div>
        <div class="u2"><input type="button"  style="background-color:#CC6666; border-color:#CC6666; color:white; height:19px;" value="New" onclick="newfile();">&nbsp;&nbsp;&nbsp;<input type="button"  style="background-color:#CC6666; border-color:#CC6666; color:white; height:18px;" value="Download All" onclick="downloadall();"></div>
        <div class="u3" id="msg">Loading...</div>
        <div class="u4">&nbsp;</div>
        <div class="c" id="fi">
          File list should appear here.
        </div>
    </div>
<center>
<a href="https://github.com/ldijkman/ESPxWebFlMgr" target="ESPxWebFlMgr">ESPxWebFlMgr Copyright &copy; 2019 Holger Lembke</a><br>
<a href="https://github.com/ldijkman/randomnerd_esp32_wifi_manager" target="Electra">Electra Copyright &copy; 2022 Dirk Luberth Dijkman</a><br>
<a href="https://github.com/holgerlembke/ESPxWebFlMgr/blob/master/LICENSE" target="SIEMENS">Not Allowed to be used by SIEMENS.</a><br>
</center>
<div id="updater"></div><br>
<input type="button"  style="background-color:#CC6666; border-color:#CC6666; color:white; height:19px;" value='Update "ESP8266basic.ino.bin" must be in file list' onclick="update();">
<br><br>
<input type="button"  style="background-color:#CC6666; border-color:#CC6666; color:white; height:19px;" value="show hidden files" onclick="showhidden();">
 
 <input type="button"  style="background-color:#CC6666; border-color:#CC6666; color:white; height:19px;" value="hide hidden files" onclick="hidehidden();">
<br>
<br>
<input type="checkbox" id="showhide" onclick="showhide()">
<label for="myCheck"><font color="orange">Show Hidden</font></label> 

   </body>
</html>  

  )==x==";

static const char ESPxWebFlMgrWpjavascript[] PROGMEM = R"==x==(

function compressurlfile(source) {
  msgline("Fetching file...");
  var request = new XMLHttpRequest();
  request.onreadystatechange = function () {
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      var data = this.responseText;
      var gzip = require('gzip-js'), options = { level: 9, name: source, timestamp: parseInt(Date.now() / 1000, 10) };
      var out = gzip.zip(data, options);
      var bout = new Uint8Array(out); // out is 16 bits...

      msgline("Sending compressed file...");
      var sendback = new XMLHttpRequest();
      sendback.onreadystatechange = function () {
        var DONE = this.DONE || 4;
        if (this.readyState === DONE) {
          getfileinsert();
        }
      };
      sendback.open('POST', '/r');
      var formdata = new FormData();
      var blob = new Blob([bout], { type: "application/octet-binary" });
      formdata.append(source + '.gz', blob, source + '.gz');
      sendback.send(formdata);
    }
  };
  request.open('GET', source, true);
  request.send(null);
}

function getfileinsert() {
  msgline("Fetching files infos...");
  var request = new XMLHttpRequest();
  request.onreadystatechange = function () {
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      var res = this.responseText.split("##");
      document.getElementById('fi').innerHTML = res[0];
      document.getElementById("o3").innerHTML = res[1];
      msgline("");
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
  xhr.open('GET', '/c?' + command, true);
  xhr.send(null);
}

function downloadfile(filename) {
  window.location.href = "/c?dwn=" + filename;
}

function deletefile(filename) {
  if (confirm("Really delete " + filename)) {
    msgline("Please wait. Delete in progress...");
    executecommand("del=" + filename);
  }
}


//////////////////////////
function update(filename) {
  if (confirm("Really update with ESP8266basic.ino.bin")) {
    msgline("<b>Please wait. update in progress...<b>");
    document.getElementById("updater").innerHTML = '<font size="3" color="yellow">Please wait. update in progress...page is reloading. will reconnect</font>';
    executecommand("update=" + filename);
         setTimeout(function () {
        document.location.reload()
    }, 1000);
  }
}
///////////////////////////////////////////////////

function renamefile(filename) {
  var newname = prompt("new name for " + filename, filename);
  if (newname != null) {
    msgline("Please wait. Rename in progress...");
    executecommand("ren=" + filename + "&new=" + newname);
  }
}




///////////////////////////////////////////////////////////////////////////////
function newfile(filename) {
  var newname = prompt("create new file ", filename);
  //alert("newfile, Electra says i do no know how to do that");
   console.log(newname);
  if (newname != null) {
    msgline("Please wait. Rename in progress...");
    executecommand("new=" + newname);
  }
}
//////////////////////////////////////////////////////////////////////////////






var editxhr;
var editor;

function editfile(filename) {
  msgline("Please wait. Creating editor...");

  editxhr = new XMLHttpRequest();
  editxhr.onreadystatechange = function () {
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      document.getElementById('fi').innerHTML = editxhr.responseText;
      document.getElementById("o3").innerHTML = "&nbsp;Edit " + filename;
      msgline("");
      
     
    editor = CodeMirror.fromTextArea(document.getElementById("tect"), {
      lineNumbers: true,
      theme: "night",
    });

    }
  };
  editxhr.open('GET', '/e?edit=' + filename, true);
  editxhr.send(null);
}

function sved(filename) {
  //var content = document.getElementById('tect').value;  // for textarea without codemirror
   //editor = CodeMirror.fromTextArea(document.getElementById("tect"));
  var content = editor.getDoc().getValue("\n");
  
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
    '--' + boundary + '--\r\n' +        // \r\n fixes upload delay in ESP8266WebServer
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

var uploaddone = true; // hlpr for multiple file uploads

function uploadFile(file, islast) {
  uploaddone = false;
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    // console.log(xhr.status);
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      if (islast) {
        getfileinsert();
        console.log('last file');
      }
      uploaddone = true;
    }
  };
  xhr.open('POST', '/r');
  var formdata = new FormData();
  formdata.append('uploadfile', file);
  // not sure why, but with that the upload to esp32 is stable.
 formdata.append('dummy', 'dummy');
  xhr.send(formdata);
}


////////////////////////////////////////////////////////////////////////
function fileopen(file, islast) {
///////
document.getElementById('fileinput').click();
//alert("fileopen, Electra says i do no know how to do that");
file = document.getElementById('fileinput').value;
 console.log(file);
 
   var xhr = new XMLHttpRequest();
  xhr.open('POST', '/r');
  var formdata = new FormData();
  formdata.append('uploadfile', file);
  // not sure why, but with that the upload to esp32 is stable.
  //formdata.append('dummy', 'dummy');
  xhr.send(formdata);


}
////////////////////////////////////////////////////////////////////////



var globaldropfilelisthlpr = null; // read-only-list, no shift()
var transferitem = 0;
var uploadFileProzessorhndlr = null;

function uploadFileProzessor() {
    if (uploaddone) {
        if (transferitem==globaldropfilelisthlpr.length) {
            clearInterval(uploadFileProzessorhndlr);
        } else {
            var file = globaldropfilelisthlpr[transferitem];
            msgline("Please wait. Transferring file "+file.name+"...");
            console.log('process file ' + file.name);
            transferitem++;
            uploadFile(file,transferitem==globaldropfilelisthlpr.length);
        }
    }
}

/*
function dropHandlerALT(ev) {
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
*/

function dropHandler(ev) {
  console.log('File(s) dropped');
  
  globaldropfilelisthlpr = ev.dataTransfer;
  transferitem = 0;

  msgline("Please wait. Transferring file...");

  // Prevent default behavior (Prevent file from being opened)
  ev.preventDefault();

  if (ev.dataTransfer.items) {
      var data = ev.dataTransfer;
      globaldropfilelisthlpr = data.files;
      uploadFileProzessorhndlr = setInterval(uploadFileProzessor,1000);
      console.log('Init upload list.');
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

function msgline(msg) {
  document.getElementById('msg').innerHTML = msg;
}

function downloadall() {
  msgline("Sending all files in one zip.");
  window.location.href = "/c?za=all";
  msgline("");
}


function showhide() {
  if (document.getElementById("showhide").checked==true)executecommand("showhidden=0");
  if (document.getElementById("showhide").checked==false)executecommand("showhidden=1");
}
function showhidden(value) {
executecommand("showhidden=0");
}
function hidehidden() {
 executecommand("showhidden=1");
}


//->
window.onload = getfileinsert;

  )==x==";


//*****************************************************************************************************
static const char ESPxWebFlMgrWpcss[] PROGMEM = R"==g==(
textarea
{
   // font: 12px "Times New Roman", Times, serif;
}

div {
  margin: 0px;
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
  white-space: nowrap;
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
  cursor: pointer;
  background-color: #CC6666;
  margin-top: 0px;
  padding-left: 10px;
  vertical-align: middle;
  font-size: 80%;
}

.u2:hover {
  background-color: #9999FF;
  color: white;
}

.u3 {
  grid-area: u3;
  padding-left: 10px;
  background-color: #FF9966;
  font-size: 80%;
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
  margin: 0px;
  //font-size: 10px;
}

#fi .b {
  background-color: Transparent;
  border: 0px solid #9999FF;  
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
  font-size: 15px; 
}

.ccg, ccu {
  height: 1.5em;
  font-size: 15px;
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

.CodeMirror {
      resize: both;
      overflow: auto !important;
    }
    
  )==g==";


#endif

