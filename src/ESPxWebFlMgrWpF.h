// inline guard. Still broken by design?
#ifndef ESPxWebFlMgrWpF_h
#define ESPxWebFlMgrWpF_h

static const char ESPxWebFlMgrWpFormIntro[] PROGMEM = 
R"==x==(<textarea id="tect" name="code" rows="5" cols="50" >)==x==";




static const char ESPxWebFlMgrWpFormExtro1[] PROGMEM = 
R"==x==(</textarea><br><button title="Save file" onclick="sved(')==x==";

// noot sure what the <script> part is for.
static const char ESPxWebFlMgrWpFormExtro2[] PROGMEM = 
R"==x==(');" >Save File</button>&nbsp;<button title="Abort editing" onclick="abed();">Abort editing</button><br><br>

<script id="info">document.getElementById('o3').innerHTML = "File:";</script>
)==x==";



#endif
