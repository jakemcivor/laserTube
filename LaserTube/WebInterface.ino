#include "WebInterface.h"

//
const byte DNS_PORT = 53;
IPAddress apIP(192,168,1,1);
DNSServer dnsServer;

ESP8266WebServer webServer(80);


// Function to set up wireless Access Point
void setupAP(){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(softAP_ssid, softAP_password); //ssid and password from WiFiConfig.h

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("**** Access point started ");
}


void showControlScreen(void)
{
  String message;
  message += "<html>";
  message += "<head><meta http-equiv=\"refresh\" content=\"20; url='/\"'><title>VHS Laser Tube Control</title></head>";
  message += "<body>";
  message += "<h3><a href=\"https://vanhack.ca\">VHS Laser Tube</a></h3>";
  message += "<form action=\"/\" method=\"get\">";
  message += "Amplitude <input type=\"text\" name=\"amplitude\" size=\"3\" value=\"" + (String) amplitude + "\"><input type=\"submit\" value=\"Submit\"> (0-255)";
  message += "</form>";
  message += "<form action=\"/\" method=\"get\">";
  message += "Period <input type=\"text\" name=\"period\" size=\"3\" value=\"" + (String) period + "\"><input type=\"submit\" value=\"Submit\"> (20-100)";
  message += "</form>";
  message += "<form action=\"/\" method=\"get\">";
  message += "Volume <input type=\"text\" name=\"volume\" size=\"3\" value=\"" + (String) volume + "\"><input type=\"submit\" value=\"Submit\"> (0-30)";
  message += "</form>";
  message += "<form action=\"/\" method=\"get\">";
  message += "<input type=\"hidden\" name=\"fire\" size=\"1\" value=\"" + (String) fire + "\"><input type=\"submit\" value=\"FIRE THE LASER!\">";
  message += "</form>";
  message += "</body>";
  message += "</html>";
  webServer.send(200, "text/html", message);
}

void handleNotFound(void)
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";
  for (uint8_t i=0; i<webServer.args(); i++)
  {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }
  webServer.send(404, "text/plain", message);
}


void setupWiFi(void)
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void setupWebServer(void)
{
  webServer.on("/", []() {
    Serial.print("HTTP REQUEST > ");

    for (uint8_t i = 0; i < webServer.args(); i++)
    {
      if (webServer.argName(i) == "frequency")
      {
        frequency = (byte) webServer.arg(i).toInt();
      }
      else if (webServer.argName(i) == "period")
      {
        period = (byte) webServer.arg(i).toInt();
      }
      else if (webServer.argName(i) == "brightness")
      {
        brightness = (byte) webServer.arg(i).toInt();
      }
      else if (webServer.argName(i) == "volume")
      {
        volume = (byte) webServer.arg(i).toInt();
        updateVolume(volume);
      }
      else if (webServer.argName(i) == "fire")
      {
        fireLaser();
      }
      else
      {
        Serial.println("unknown argument! ");
      }
      Serial.print(webServer.argName(i));
      Serial.print(": ");
      Serial.print(webServer.arg(i));
      Serial.print(" > ");
    }
    Serial.println("done");

    showControlScreen();
  });

  webServer.onNotFound(handleNotFound);

  webServer.begin();
  Serial.println("HTTP server started");
}
