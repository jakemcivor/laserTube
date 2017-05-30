#ifndef _WEBINTERFACE_H_
#define _WEBINTERFACE_H_

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>



extern byte frequency;
extern byte period;
extern byte brightness;
extern byte volume;
extern byte fire;

extern ESP8266WebServer webServer;
extern DNSServer dnsServer;

void setupWiFi(void);
void setupAP(void);
void setupWebServer(void);

#endif //_WEBINTERFACE_H_
