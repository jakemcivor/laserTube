#define LASER_TUBE_VERSION "1.05"

#include "Laser.h"
#include "Interface.h"
#include "Sound.h"
#include "WebInterface.h"
#include "WifiConfig.h"

// WIFI

// Create WifiConfig.h with the following:
//#ifndef _WIFICONFIG_H_
//#define _WIFICONFIG_H_
//const char* ssid = "SSID"; e.g. VanHack.ca
//const char* password = "********";
//#endif //_WIFICONFIG_H_

// INTERFACE
#define BUTTON_PIN D5
#define DEBOUNCE_MS 25

//LASER
#define FRAMES_PER_SECOND 60
#define NUM_LEDS 137
#define LED_PIN D4 // connected to level-shifter to drive LED strip

//SOUND
#define BUSY_PIN D6
#define RX_PIN D1
#define TX_PIN D2
#define SOUND_VOLUME 15

#define SCENE_CHARGING_LEN_MS 6500
#define SCENE_CHARGED_LEN_MS 0
#define SCENE_FIRING_LEN_MS 1253

#define SCENE_CHARGING_INDEX 23
#define SCENE_CHARGED_INDEX 1
#define SCENE_FIRING_INDEX 8




void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println("Vancouver Hack Space Laser Tube " LASER_TUBE_VERSION);

  setupWiFi();
  setupWebServer();
  setupButton(); // setup physical interface button
  setupSound();
  setupLaser();

}

void loop() {
  server.handleClient();
  handleButton();
  doSoundLoop();
  doScene();
  delay(0); // release to os(?)
}
