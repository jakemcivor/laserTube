#include "Laser.h"

// Instantiate strip
CRGB leds[NUM_LEDS];

// Instantiate pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const int iterationCount = 2000;
const int SCENE_COOLDOWN = 0;
const int SCENE_IDLE = 1;
const int SCENE_CHARGING = 2;
const int SCENE_CHARGED = 3;
const int SCENE_FIRING = 4;
const int SCENE_PRETTYLIGHTS = 5;

long laserTimer;

const int CHARGE_PIXELS = 20; //number of pixels in bottom of tube near coil

const int minBrightness = 8;

int sceneBrightness = 1;
int r = 32;
int g = 32;
int b = 32;

// AMPLITUDE = brightness amplitude for standy
byte amplitude = 10;

// FREQUENCY = laser charge frequencyrightness amplitude for standy
byte frequency = 10; //todo unused

// PERIOD: Standby frequency period
byte period = 4; //todo unused

// VOLUME: Sound volume. Range  0 - 30
byte volume = 25;

// BRIGHTNESS: Pixel Brightness
byte brightness = 128; 

// Fire: Fire laser flag
byte fire = 1; 

// PRETTYLIGHTS: Pretty lights flag
byte prettylights = 0;

int sceneState = SCENE_IDLE; // start with idle scene
uint64_t count = 0;
long sceneStartMillis = 0;


void setupLaser(){
    pixels.begin(); // Initializes NeoPixel Library
    Serial.println("NeoPixel array started");
    setScene(SCENE_COOLDOWN);
}


void fireLaser(void){

  // if we are in idle state: fire; otherwise, do nothing
  if (sceneState == SCENE_IDLE){
    setScene(SCENE_CHARGING);
  }

}
void Laser(void)
{
//todo laser energy charging model
}



void keepOnLasing(void)
{
//  if (millis() > laserTimer + 1000 / FRAMES_PER_SECOND)
//  {
//    laserTimer = millis();
//    Laser(); //update model state (based on time)
//    for (int i = 0; i < NUM_LEDS; i++)
//    {
//      pixels.setPixelColor(i, pixels.Color(leds[i].r, leds[i].g, leds[i].b));
//    }
//
//    pixels.setBrightness(sceneBrightness);// set pixel brightness
//    pixels.show(); //send updated pixel color to hardware
//
//  }
}

//////////////////////////////////////
//////////////////////////////////////
// IDLE SCENCE
//////////////////////////////////////
void sceneIdle() {
    // idle loop when nothing else is happening
  count++;
  
  if (count % iterationCount == 0) {
    Serial.println("-- idle");
  }

  if (sceneBrightness < 1) { pixels.setBrightness(1); sceneBrightness = 1; }

  int iteration = count % iterationCount;
  int lbrightness = (sceneBrightness < 72 ? sceneBrightness++ : map(iteration, 0, iterationCount, -72, 72));
  int r = 0;
  int g = 0; // abs(lbrightness);
  int b = abs(lbrightness);; // 64;
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    delay(0);
  }

  pixels.show(); //send updated pixel color to hardware

}

void sceneCharging() {
  // charging up animation

  long sceneEnd = sceneStartMillis + SCENE_CHARGING_LEN_MS;
  count++;
  laserTimer = millis();
  if (laserTimer > sceneEnd) {
    Serial.println("Finished charging");
    // if we've been charging for 5 sec, we're charged
    //setScene(SCENE_CHARGED); //skip charged SCENE FOR NOW
    setScene(SCENE_FIRING); 
    return;
  }
  //int intensity = count % NUM_LEDS;

  if (sceneBrightness < 32) { pixels.setBrightness(32); sceneBrightness = 32; }

  int sceneBrightness = map(laserTimer, sceneStartMillis, sceneEnd, 200, 0);
  for (int i = 0; i < NUM_LEDS; i++) {
    int localr = map(laserTimer, sceneStartMillis, sceneEnd, r, 0);
    int localg = map(laserTimer, sceneStartMillis, sceneEnd, g, 0);
    b = sceneBrightness;
    pixels.setPixelColor(i, pixels.Color(localr, localg, b));
    delay(0);
  }

//add progress-bar style charge-up 
  int chargeLevel = map(laserTimer, sceneStartMillis, sceneEnd, 1, CHARGE_PIXELS);
  for (int i = 0; i < chargeLevel; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,255));
    delay(0);
  }
// add increasing number of random flashes
  for (int i=0; i < chargeLevel; i++){
    pixels.setPixelColor(random(chargeLevel,NUM_LEDS),pixels.Color(sceneBrightness,sceneBrightness,sceneBrightness));
    delay(0);
  }

  //pixels.setBrightness(brightness);// set pixel brightness
  pixels.show(); //send updated pixel color to hardware

}

//////////////////////////////////////
//////////////////////////////////////
// Charged SCENE
//////////////////////////////////////

void sceneCharged() {
  // charged loop
  count++;
  laserTimer= millis();
  if (laserTimer > sceneStartMillis + SCENE_CHARGED_LEN_MS) {
    // if we've been charged for 2 sec, fire
    Serial.println("Ready to fire");
    setScene(SCENE_FIRING);
    return;
  }

  r = 0; g = 0; b = 255;
  for (int i = 0; i < CHARGE_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    delay(0);
  }
  for (int i = CHARGE_PIXELS; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    delay(0);
  }

  pixels.show(); //send updated pixel color to hardware
}

//////////////////////////////////////
//////////////////////////////////////
// FIRING SCENE
//////////////////////////////////////

void sceneFiring() {
  // firing animation
  count++;
  laserTimer = millis();
  long sceneEnd = sceneStartMillis + SCENE_FIRING_LEN_MS;
  if (laserTimer > sceneEnd) {
    // if we're finished firing go back to idle
    Serial.println("Finished firing, cooling down");
    sceneBrightness = 1;
    setScene(SCENE_COOLDOWN);
    return;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    int currentLED = count % NUM_LEDS;
    if (i >= currentLED + 3 || i <= currentLED - 3) {
      r = 0; g = 0; b = map(laserTimer, sceneStartMillis, sceneEnd, 255, 0);
     } else {
      r = 0; g = 0; b = 0;
    }
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    delay(0);
  }

  pixels.show(); //send updated pixel color to hardware
}

void sceneCooldown(){
  count ++;
  laserTimer = millis();
    if (laserTimer > sceneStartMillis + SCENE_COOLDOWN_LEN_MS) {
    // if we've been charged for 2 sec, fire
    Serial.println("Cooled down");
    setScene(SCENE_IDLE);
    return;
  }
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      delay(0);
  }
  
  pixels.show();
}


void setScene(int sceneNum) {
    count = 0; //reset counter
    if (sceneNum < SCENE_COOLDOWN || sceneNum > SCENE_PRETTYLIGHTS) {
      Serial.println("Invalid scene, setting Idle scene");
      sceneNum = SCENE_IDLE; // return to idle if we're looping around
    }
    Serial.print("Setting scene to ");
    Serial.println(sceneNum);

    switch (sceneNum) {
      case SCENE_CHARGING:
        // fire the sound file for charging scene
        playSoundFile(SCENE_CHARGING_INDEX + 1);
        break;
      case SCENE_CHARGED:
        // fire the sound file for charged scene
        playSoundFile(SCENE_CHARGED_INDEX + 1);
        break;
      case SCENE_FIRING:
        // fire the sound file for the firing scene
        playSoundFile(SCENE_FIRING_INDEX + 1);
        break;
      case SCENE_COOLDOWN:
       //playSoundFile(SCENCE_COOLDOWN_INDEX +1);
       break;
    }


    sceneState = sceneNum;
    sceneStartMillis = millis();
}

void doScene() {
  // animates the current scene
  // simple state machine
  switch (sceneState) {
    case SCENE_IDLE: // idle
      sceneIdle();
      break;
    case SCENE_CHARGING: // charging
      sceneCharging();
      break;
    case SCENE_CHARGED:
      sceneCharged();
      break;
    case SCENE_FIRING:
      sceneFiring();
      break;
    case SCENE_COOLDOWN:
      sceneCooldown();
      break;
    case SCENE_PRETTYLIGHTS:
      scenePrettyLights();
      break;
  }
}

void prettyLights(){
  Serial.println("Activating Pretty Lights Mode");
  setScene(SCENE_PRETTYLIGHTS);
}

void scenePrettyLights() {
  rainbowCycle(20);
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


