#ifndef _LASER_H_
#define _LASER_H_

#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

extern byte frequency;
extern byte period;
extern byte brightness;
extern byte volume;
extern byte fire;


void setupLaser(void); // function to setup laser
void fireLaser(void); //called by web  / button interface to change state
void keepOnLasing(void); // function to update laser //todo

// Laser Scenes
// Each function defines a state with an accompanying animation
void sceneIdle(void); // idle loop when nothing is happening
void scenceCharging(void); //
void sceneCharged(void); //ready to be fired
void sceneFiring(void); // firing scene

void doScene(void);
void doSoundLoop(void);
void setScene(int sceneNum); 


#endif //_LASER_H_
