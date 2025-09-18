#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class WebServer {
private:
  ESP8266WebServer* server;
  
public:
  WebServer(ESP8266WebServer* server);
  
  void setupRoutes();
  void serveCompressedHTML(const char* htmlData, size_t htmlSize);
};

#endif
