#include "ledcontroller.h"

#define ECHO_IN_1 7
#define ECHO_OUT_1 8

#define ECHO_IN_2 9
#define ECHO_OUT_2 11


#define LED_PIN 6
#define LIGHT_SENSOR 5
#define LIGHT_SENSOR_VALUE 450

#define READ_TRESH 2
#define DISTANCE_TRESHHOLD 30

#define READ_DELAY 150
#define READ_CALIB 20

#define FADE_OFF 0
#define FADE_LOW 50
#define FADE_HIGH 255
#define FADE_SPEED 20

#define FADE_ON_DURATION 5000


LEDController led(LED_PIN, 4, 3);

long calib_sensor_1 = 0;
long calib_sensor_2 = 0;

int lampStatus = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("started");

  led.setNonBlocking(false);

  led.setColor(1, 0, 0, 0);

  pinMode(ECHO_IN_1, INPUT);
  pinMode(ECHO_IN_2, INPUT);
  pinMode(ECHO_OUT_1, OUTPUT);
  pinMode(ECHO_OUT_2, OUTPUT);

  for (int i = 0; i < READ_CALIB; i++) {
    Serial.println(i);
    calib_sensor_1 += readSensor(ECHO_IN_1, ECHO_OUT_1);
    calib_sensor_2 += readSensor(ECHO_IN_2, ECHO_OUT_2);
  }

  calib_sensor_1 = calib_sensor_1 / READ_CALIB;
  calib_sensor_2 = calib_sensor_1 / READ_CALIB;

  Serial.print("Calibration Sensor 1: ");
  Serial.println(calib_sensor_1);
  Serial.print("Calibration Sensor 2: ");
  Serial.println(calib_sensor_2);

  led.setColor(FADE_OFF, 0, 0, 0);
}

void loop() {

  if (analogRead(LIGHT_SENSOR) > LIGHT_SENSOR_VALUE) {
    // dunkel

    //Serial.println("Distanz pruefen");

    boolean near1 = isNear(ECHO_IN_1, ECHO_OUT_1, calib_sensor_1, READ_TRESH);
    boolean near2 = isNear(ECHO_IN_2, ECHO_OUT_2, calib_sensor_2, READ_TRESH);

    if (near1 || near2) {
      if (lampStatus != 2) {
        Serial.println("Person anwesend");
        lampStatus = 2;
        led.setColor(FADE_HIGH, 0, 0, FADE_SPEED);
      } else {
        Serial.println("Person immernoch anwesend");
      }
      delay(FADE_ON_DURATION);
    } else {

      if (lampStatus != 1) {
        Serial.println("Person abwesend");
        lampStatus = 1;
        led.setColor(FADE_LOW, 0, 0, FADE_SPEED);
      } else {
        Serial.println("Person immernoch abwesend");
      }
      
    }
      
  
  } else {
    if (lampStatus != 0) {
      Serial.println("Lampe dunkel");
      led.setColor(FADE_OFF, 0, 0, FADE_SPEED);
      lampStatus = 0;
    }
    delay(10);
  }

}

long readSensor(int in, int out) {
  digitalWrite(out, LOW);
  delayMicroseconds(10);
  digitalWrite(out, HIGH);
  delayMicroseconds(10);
  digitalWrite(out, LOW);
  long duration = pulseIn(in, HIGH);
  long distance = duration / 58;
  if (distance > 350) {
    distance = 350;
  }
  delay(READ_DELAY);
  return distance;
}

boolean isNear(int in, int out, int calib, int count) {
  boolean near = true;
  for (int i = 0; i < count; i++) {
    long distance = readSensor(in, out);
    if (distance > calib - DISTANCE_TRESHHOLD) {
      near = false;
    }
  }
  return near;
}
