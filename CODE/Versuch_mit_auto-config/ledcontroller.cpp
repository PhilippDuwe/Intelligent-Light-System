/*
  ledcontroller.cpp - Steuerung von RGB LED Leisten über MOSFETs
  Erstellt von Maik Sicks | Erstellt am 26.11.2019
*/

#include "Arduino.h"
#include "ledcontroller.h"

LEDController::LEDController(int _pin_r, int _pin_g, int _pin_b) {
  pin_r = _pin_r;
  pin_g = _pin_g;
  pin_b = _pin_b;

  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
}

void LEDController::setInvert(boolean _invert) {
  invert = _invert;
}

void LEDController::setUpdateInterval(int _updateInterval) {
  updateInterval = _updateInterval;
}

void LEDController::setNonBlocking(boolean _nonBlocking) {
  nonBlocking = _nonBlocking;
}

void LEDController::setColor(int _r, int _g, int _b, int _fade) {

  if (_fade <= updateInterval) {
    isFading = false;
    setRGB(_r, _g, _b);
  } else {
    isFading = true;

    fade_duration = _fade;
    fade_steps = fade_duration / updateInterval;
    fade_step = 0;

    fade_start_r = curr_r;
    fade_start_g = curr_g;
    fade_start_b = curr_b;

    fade_end_r = _r;
    fade_end_g = _g;
    fade_end_b = _b;

    if (!nonBlocking) {
      while (isFading) {
        execute();
        delay(1);
      }
    }
    
  }
  
}

void LEDController::setColor(int _r, int _g, int _b) {
  setColor(_r, _g, _b, 0);
}

void LEDController::stopAll() {
  isFading = false;
}

void LEDController::execute() {

  // nur alle <updateInterval> Millisekunden aktualisieren
  if (millis() % updateInterval != 0) {
    if (doUpdate) {
      doUpdate = false;

      // LEDs aktualisieren ...
      if (isFading) {

        fade_step++;

        float multiply = (float) fade_step / (float) fade_steps;

        // Farbe berechnen ( start_farbe + (different_start_ende * fortschritt) )
        setRGB(
          fade_start_r + ((fade_end_r - fade_start_r) * multiply),
          fade_start_g + ((fade_end_g - fade_start_g) * multiply),
          fade_start_b + ((fade_end_b - fade_start_b) * multiply)
        );

        if (fade_step >= fade_steps) {
          isFading = false;
        }
        
      }
  
    }
  } else {
    doUpdate = true;
  }
}

void LEDController::setRGB(int _r, int _g, int _b) {
  curr_r = _r;
  curr_g = _g;
  curr_b = _b;
  
  analogWrite(pin_r, (invert ? 255 - _r : _r));
  analogWrite(pin_g, (invert ? 255 - _g : _g));
  analogWrite(pin_b, (invert ? 255 - _b : _b));
}
