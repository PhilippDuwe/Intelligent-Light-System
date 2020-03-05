/*
  ledcontroller.h - Steuerung von RGB LED Leisten über MOSFETs
  Erstellt von Maik Sicks | Erstellt am 26.11.2019
*/

#ifndef ledcontroller_h
#define ledcontroller_h

#include "Arduino.h"

class LEDController {
  public:
    /* Konstruktor */
    LEDController(int _pin_r, int _pin_g, int _pin_b);

    void setInvert(boolean _invert);
    void setUpdateInterval(int _updateIntval);
    void setNonBlocking(boolean _nonBlocking);

    void setColor(int _r, int _g, int _b, int _fade);
    void setColor(int _r, int _g, int _b);

    void stopAll();

    void execute();
    
  private:
    /* LED pins */
    int pin_r;
    int pin_g;
    int pin_b;

    /* Current color */
    int curr_r = 0;
    int curr_g = 0;
    int curr_b = 0;

    boolean invert = false;
    boolean doUpdate = true;
    int updateInterval = (1000 / 60);

    boolean nonBlocking = true;
    
    boolean isFading = false;
    int fade_duration = 0;
    
    int fade_steps = 0;
    int fade_step = 0;
    
    int fade_start_r = 0;
    int fade_start_g = 0;
    int fade_start_b = 0;
    int fade_end_r = 0;
    int fade_end_g = 0;
    int fade_end_b = 0;
    

    void setRGB(int _r, int _g, int _b);
};

#endif
