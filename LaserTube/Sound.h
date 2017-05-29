#ifndef _SOUND_H_
#define _SOUND_H_

#include <SoftwareSerial.h>
//#include <DFRobotDFPlayerMini.h> //alternative library
//#include <DFPlayer_Mini_Mp3.h> //alternative library
#include <DFMiniMp3.h>


class Mp3Notify //Notification class
{
public:
  static void OnError(uint16_t errorCode) { }

  static void OnPlayFinished(uint16_t globalTrack) { }

  static void OnCardOnline(uint16_t code) { }

  static void OnCardInserted(uint16_t code) { }

  static void OnCardRemoved(uint16_t code) { }
};

void setupSound(void); // function to setup sounds
void playSoundFile(int); // plays a sound by its track number
void doSoundLoop(); // runs the hardware interface loop
void updateVolume(int); // update volume from web interface

#endif //_SOUND_H_
