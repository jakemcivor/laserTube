#include "interface.h"

//Button myBtn(BUTTON_PIN, false, false, DEBOUNCE_MS); //// http://platformio.org/lib/show/77/Button/installation
Button myBtn(BUTTON_PIN); // Arduino button library

void setupButton() {
  myBtn.begin(); // setup case push button
  
}

void handleButton() {
  if (myBtn.pressed()){
     Serial.println("**** Button pressed, charging!");
     fireLaser();
    
  }
}

// http://platformio.org/lib/show/77/Button/installation
//void handleButton() {
//  myBtn.read();
//  if (myBtn.wasReleased()) {
//    // if button is pressed, start charging sequence
//    Serial.println("**** Button pressed, charging!");
//    setScene(SCENE_CHARGING);
//  }
//}
