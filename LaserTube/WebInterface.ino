#include "WebInterface.h"

ESP8266WebServer server(80);

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
  server.send(200, "text/html", message);
}

void handleNotFound(void)
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
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
  server.on("/", []() {
    Serial.print("HTTP REQUEST > ");

    for (uint8_t i = 0; i < server.args(); i++)
    {
      if (server.argName(i) == "frequency")
      {
        frequency = (byte) server.arg(i).toInt();
      }
      else if (server.argName(i) == "period")
      {
        period = (byte) server.arg(i).toInt();
      }
      else if (server.argName(i) == "brightness")
      {
        brightness = (byte) server.arg(i).toInt();
      }
      else if (server.argName(i) == "volume")
      {
        volume = (byte) server.arg(i).toInt();
        updateVolume(volume);
      }
      else if (server.argName(i) == "fire")
      {
        fireLaser();
      }
      else
      {
        Serial.println("unknown argument! ");
      }
      Serial.print(server.argName(i));
      Serial.print(": ");
      Serial.print(server.arg(i));
      Serial.print(" > ");
    }
    Serial.println("done");

    showControlScreen();
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}
