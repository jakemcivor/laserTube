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
void fireLaser(void);
void keepOnLasing(void); // function to update laser
void sceneIdle(void); // idle loop when nothing is happening
void sceneFiring(void); // firing scence
void sceneCharged(void);
void scenceCharging(void);
void doScene(void);
void doSoundLoop(void);
void setScene(int sceneNum); 


#endif //_LASER_H_
