// cc by DM5XX @ GNU GPLv3
// LLAP! 

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "Adafruit_MCP23017.h"
#include "ArduinoJson.h"

//#define WFM

#ifdef WFM
  #include "WiFiManager.h"
#endif

#define DEBUG

byte numberOfRelayBoards = 1;
String customUrl;
String contentUrl;

ESP8266WebServer server(80);

#ifdef WFM
  WiFiManager mywifiManager;
#else
  const char* ssid = "mmmedia";
  const char* password = "tp4004mmatrixx6";
#endif

byte pinsOrder[16] = { 0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15 };

struct RelayCard
{
  Adafruit_MCP23017 mcp;
  
  byte pins[16];
  byte pinStatus[16];
};

String MAIN_page0 = "<!DOCTYPE html><HTML><HEAD><meta http-equiv=\"Cache-control\" content=\"no-cache\"><meta http-equiv=\"Expires\" content=\"0\"><script type=\"text/javascript\" src=\"http://code.jquery.com/jquery-1.11.2.min.js\"></script><script>";
String MAIN_page1 = "</script><script src=\"http://h.mmmedia-online.de/ultraswitch_v1.js\"></script>";
String MAIN_page2 = "<link rel=\"shortcut icon\" href=\"http://h.mmmedia-online.de/favicon.ico\"><TITLE>RemoteQth UltraSwitch</TITLE></HEAD><BODY><div id=\"content\"></div><script>addContent();</script></BODY></HTML>";

/////////////////// DONT USE NESTES CLASSES! SPLIT IT INTO .h and .cpp - But for tutorial reasons, its done inline /////////////
class CustomRelayHandler : public RequestHandler {

  RelayCard _cards[8];
  byte _numberOfRelayBoards;

  // easy way to convert binary arrays to bitshifted decimal values...
  unsigned int getEncodedSum(byte* bin)
  {
	  unsigned int i = 1;
	  unsigned int sum = 0;

	  for (int m = 0; m < 16; m++)  // do it from the back...
	  {
      if(bin[m] == 1)
        sum = sum + pow(2, m);
	  }
	  return sum;
  }

public:
  CustomRelayHandler(){ }
  void Init(RelayCard relay[8], byte numberOfRelayBoards)
  {
    _numberOfRelayBoards = numberOfRelayBoards;

    for(byte a = 0; a < numberOfRelayBoards; a++)
    {
      _cards[a] = relay[a];
      _cards[a].mcp.begin(a);      // use default address 0
      
      for (byte p = 0; p < 16; p++)
      {
        _cards[a].mcp.pinMode(p, OUTPUT);
        _cards[a].mcp.digitalWrite(p, HIGH);
        delay(20);
      }
    }    
  }
  
  bool canHandle(HTTPMethod method, String uri) {
    return (uri.startsWith("/Set/") || uri.startsWith("/Sa/") || uri.startsWith("/SetConfig/"));
  }

  bool handle(ESP8266WebServer& sserver, HTTPMethod requestMethod, String requestUri) {   

#ifdef DEBUG
    Serial.println(requestUri);
    Serial.println(sizeof(requestUri));
#endif
    char buf[sizeof(requestUri)];
    requestUri.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    byte counter = 0;
    unsigned long params[3];
    String _localIp;
    String _localGw;
    String _localContentIp;
    int _localnumberBoards;
    String command;
    String _customUrl;
    
    if (!canHandle(requestMethod, requestUri))
      return false;
    
    if(!requestUri.startsWith("/SetConfig/"))
    {
      while ((str = strtok_r(p, "/", &p)) != NULL) // delimiter is the semicolon
      {        
        if(counter > 0)
          params[counter-1] = strtoul(str, NULL, 10);
        else
          command = str;
        counter++;
      }  
    }
    else
    {
      command = getValue(requestUri, '/', 1);
      _localIp = getValue(requestUri, '/', 2);
      _localGw = getValue(requestUri, '/', 3);
      _localContentIp = getValue(requestUri, '/', 4);
      _localnumberBoards = getValue(requestUri, '/', 5).toInt();
      _customUrl = getValue(requestUri, '/', 6);
    }

#ifdef DEBUG    
    Serial.print("LocalIp: ");
    Serial.println(_localIp);
    Serial.print("GateWay: ");
    Serial.println(_localGw);
    Serial.print("Number: ");
    Serial.println(_localnumberBoards);
    Serial.print("CustomUrl: ");
    Serial.println(_customUrl);
    Serial.print("ContenIp: ");
    Serial.println(_localContentIp);
    Serial.print("Param0: ");
    Serial.println(params[0]);
    Serial.print("Param1: ");
    Serial.println(params[1]);
    Serial.print("Param2: ");
    Serial.println(params[2]);
#endif

    if(command == "Set")
    {

      _cards[params[0]].pinStatus[params[1]] = params[2]; // set the statusflag

#ifdef DEBUG
      Serial.print("Pinstatus: ");
      Serial.println(_cards[params[0]].pinStatus[params[1]]);

      Serial.print("PinsNr: ");
      Serial.println(_cards[params[0]].pins[params[1]]);
#endif

      if(params[2] == 1) // write the correct pins
        _cards[params[0]].mcp.digitalWrite(_cards[params[0]].pins[params[1]], LOW);
      else
        _cards[params[0]].mcp.digitalWrite(_cards[params[0]].pins[params[1]], HIGH);      
    }
    else if (command =="Sa")
    {
      for (int bits = 15; bits > -1; bits--) 
      {
        byte value = 0;

        if (params[1] & (1 << bits))
          value = 1;
        
          _cards[params[0]].pinStatus[bits] = value;

          if(value == 1) // write the correct pins
            _cards[params[0]].mcp.digitalWrite(_cards[params[0]].pins[bits], LOW);
          else
            _cards[params[0]].mcp.digitalWrite(_cards[params[0]].pins[bits], HIGH);         
      }
    }
    else if (command == "SetConfig")
    {
      IPAddress _ip;
      IPAddress _ipGw;
      IPAddress _contentIp;
      int _stringLen;

      _ip.fromString(_localIp);
      _ipGw.fromString(_localGw);
      _contentIp.fromString(_localContentIp);
      _stringLen = _customUrl.length()+1;

      char  buf[_stringLen];
      _customUrl.toCharArray(buf, _stringLen);
      
      //cleanfirst
      EEPROM.begin(4095);
        for (int i = 0 ; i < 4095 ; i++) {
          EEPROM.write(i, 0);
        }          
      EEPROM.commit();
      EEPROM.end();

      EEPROM.begin(4095);

#ifdef DEBUG
      Serial.print("Sizeof: ");
      Serial.println(_stringLen);
#endif
      EEPROM.write(0, _ip[0]);
      EEPROM.write(1, _ip[1]);
      EEPROM.write(2, _ip[2]);
      EEPROM.write(3, _ip[3]);
      EEPROM.write(4, _ipGw[0]);
      EEPROM.write(5, _ipGw[1]);
      EEPROM.write(6, _ipGw[2]);
      EEPROM.write(7, _ipGw[3]);
      EEPROM.write(8, _contentIp[0]);
      EEPROM.write(9, _contentIp[1]);
      EEPROM.write(10, _contentIp[2]);
      EEPROM.write(11, _contentIp[3]);
      EEPROM.write(12, _localnumberBoards);
      EEPROM.write(13, _stringLen);

      for(int c = 0; c < _stringLen; c++)
      {
        EEPROM.write(c+14, buf[c]);
      }
      
    
      EEPROM.commit();
      EEPROM.end();

      server.send(200, "application/json", "{\"Config_Set_Reboot\" : \"OK\"}");
      delay(1000);
      ESP.restart();
    }
    getPinStatus();
    return true;
  }
  
  void getPinStatus(){
    
      StaticJsonBuffer<400> jsonBuffer;  
      JsonObject& jroot = jsonBuffer.createObject();
      char bufferStatus[200];
      
      jroot["B"] = _numberOfRelayBoards;
      JsonArray& data = jroot.createNestedArray("V");
    
      for (int a = 0; a < _numberOfRelayBoards; a++)
      {
        data.add(getEncodedSum(_cards[a].pinStatus));  
      }
      jroot["M"] = ESP.getFreeHeap();

      jroot.printTo(bufferStatus);
      server.send(200, "application/json", bufferStatus);
    }

  void reset(){
    
    for (int b = 0; b < _numberOfRelayBoards; b++)
    {
      for (int a = 0; a < 16; a++)
      {
        _cards[b].pinStatus[a] = 0;
        _cards[b].mcp.digitalWrite(_cards[b].pins[a], HIGH); 
      }  
    }

    server.send(200, "application/json", "{\"Reset\" : \"OK\"}");
   }

   String getValue(String data, char separator, int index)
   {
       int found = 0;
       int strIndex[] = { 0, -1 };
       int maxIndex = data.length() - 1;
   
       for (int i = 0; i <= maxIndex && found <= index; i++) {
           if (data.charAt(i) == separator || i == maxIndex) {
               found++;
               strIndex[0] = strIndex[1] + 1;
               strIndex[1] = (i == maxIndex) ? i+1 : i;
           }
       }
       return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
   }  
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Board 0 muss immer existieren... :P
RelayCard relay0;
RelayCard relay1;
RelayCard relay2;
RelayCard relay3;
RelayCard relay4;
RelayCard relay5;
RelayCard relay6;
RelayCard relay7;

RelayCard relayArray[8];

CustomRelayHandler mmm;

void handleRoot() {
  //String s1 = MAIN_page0; //Read HTML contents
  String v1 = "var configAddress='http://"+customUrl+"';";
  String v2 = "var numberOfRelayBoards="+String(numberOfRelayBoards)+";";
  

  //String s2 = MAIN_page1; //Read HTML contents
  String v3 = "<script src=\"http://"+contentUrl+"/h/ultraswitch_content_v1.js\"></script>";
  String v4 = "<link rel=\"Stylesheet\" href=\"http://"+contentUrl+"/h/ultraswitch_v1.css\" type=\"text/css\">";
  //String s3 = MAIN_page2; //Read HTML contents
  
  server.send(200, "text/html", MAIN_page0+v1+v2+MAIN_page1+v3+v4+MAIN_page2); //Send web page
}

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
#endif

IPAddress _ip = IPAddress(192, 168, 0, 5);
IPAddress _gw = IPAddress(192, 168, 0, 1);
IPAddress _co = IPAddress(31, 31, 231, 42);
IPAddress _sn = IPAddress(255, 255, 255, 0);

#ifndef WFM
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

EEPROM.begin(4095);

// Normalpart... uncomment until emergency part if there is a problem and use EMERGENCY part below... 
_ip[0] = EEPROM.read(0);
_ip[1] = EEPROM.read(1);
_ip[2] = EEPROM.read(2);
_ip[3] = EEPROM.read(3);
_gw[0] = EEPROM.read(4);
_gw[1] = EEPROM.read(5);
_gw[2] = EEPROM.read(6);
_gw[3] = EEPROM.read(7);
_co[0] = EEPROM.read(8);
_co[1] = EEPROM.read(9);
_co[2] = EEPROM.read(10);
_co[3] = EEPROM.read(11);


numberOfRelayBoards = EEPROM.read(12);


int _strLen = EEPROM.read(13);
int offSet;
char buf[_strLen];

for(int q = 0; q < _strLen; q++)
{
  offSet = 14+q;  
  buf[q] = EEPROM.read(offSet);
}

customUrl = String(buf);
contentUrl = _co.toString();


///EMERGENCY Part
/*
numberOfRelayBoards = 2;

customUrl = _ip.toString();
contentUrl = _co.toString();
int _strLen = 14;
*/

#ifdef DEBUG
  Serial.print("IP: ");
  Serial.println(_ip.toString());

  Serial.print("GW: ");
  Serial.println(_gw.toString());

  Serial.print("CO: ");
  Serial.println(contentUrl);

  Serial.print("Boards: ");
  Serial.println(numberOfRelayBoards);

  Serial.print("Länge: ");
  Serial.println(_strLen);

  Serial.print("CustomURL: ");
  Serial.println(customUrl);
#endif

EEPROM.end();

//Uncomment below to reset saved settings => Something went wrong while wifi configuration...
//mywifiManager.resetSettings();

delay(100);

if((_ip[0] == 192 && _ip[1] == 168) || (_ip[0] == 10) || (_ip[0] == 172 && _ip[1] > 15 && _ip[1] < 32))
{
  #ifdef WFM
    mywifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
  #endif
}
else
{
  customUrl = "127.0.0.1";
  contentUrl = "31.31.231.42";
  numberOfRelayBoards = 1;
}

#ifdef WFM
  mywifiManager.autoConnect();
#endif

  for (int a = 0; a < 16; a++)
  {
    relay0.pins[a] = pinsOrder[a];
    relay1.pins[a] = pinsOrder[a];              
    relay2.pins[a] = pinsOrder[a];              
    relay3.pins[a] = pinsOrder[a];              
    relay4.pins[a] = pinsOrder[a];              
    relay5.pins[a] = pinsOrder[a];              
    relay6.pins[a] = pinsOrder[a];              
    relay7.pins[a] = pinsOrder[a];              
}  
  
  relayArray[0] = relay0;
  relayArray[1] = relay1;
  relayArray[2] = relay2;
  relayArray[3] = relay3;
  relayArray[4] = relay4;
  relayArray[5] = relay5;
  relayArray[6] = relay6;
  relayArray[7] = relay7;

  mmm.Init(relayArray, numberOfRelayBoards);

  server.addHandler(&mmm);
  server.on("/", handleRoot);
  server.on("/Get", [](){
    mmm.getPinStatus();
  });
  server.on("/Reset", [](){
    mmm.reset();      
  });
  server.begin();

#ifdef DEBUG
  Serial.print("ESP-IP: ");
  Serial.print(WiFi.localIP());
#endif
}

void loop() {
  server.handleClient();
}
