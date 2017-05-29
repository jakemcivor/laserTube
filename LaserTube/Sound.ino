#include "Sound.h"
SoftwareSerial soundSerial(RX_PIN, TX_PIN); // RX, TX

DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(soundSerial);


// Function to initialize 
void setupSound(void)
{
  pinMode(BUSY_PIN, INPUT);
  Serial.println("Setting up sound controller");
  mp3.begin();

  // Add delay to allow player to setup
  delay(1000);
  updateVolume(SOUND_VOLUME);
  Serial.println("Finished setting up sound controller");


}

//Update notification class
void doSoundLoop() {
    mp3.loop();
}

// Plays the nth sound in the main folder
void playSoundFile(int soundFile) {
  mp3.playGlobalTrack(soundFile);
}

void updateVolume(int soundVolume){
  if (soundVolume < 30){
  mp3.setVolume(soundVolume);
  Serial.println("Volume set to: " + soundVolume);
  }
  else {
  mp3.setVolume(30);
  Serial.println("Volume set to max: 30");
  }
}

