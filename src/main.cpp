#include <Arduino.h>
#include <string>

#include <SoftwareSerial.h>

#include "config.h"

std::string msg = "";
int range = 0;
unsigned long timestamp = millis();
bool alarm = 0;

SoftwareSerial sensor( PIN_RX, PIN_TX);

// put function declarations here:
void checkRange();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  sensor.begin(115200);

  pinMode(PIN_PRESENCE, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_ALARM, OUTPUT);

}

void loop() {
  digitalWrite(PIN_LED, !digitalRead(PIN_PRESENCE));
  checkRange();
  Serial.print(range);
  if(range > RANGEMIN && range < RANGEMAX) {
    if(millis() - timestamp > LOITERDELAY) {
      timestamp = millis() - (LOITERDELAY + 1);
      alarm = 1;
      Serial.print(", A   ");
    }
    else{
      Serial.print(",  B  ");
    }
  }
  else {
    if(alarm && millis() - timestamp < ALARMDELAY + LOITERDELAY) {
      Serial.print(",   C ");
    }
    else {
      timestamp = millis();
      alarm = 0;
      Serial.print(",    D");

    }
  }
  Serial.print(digitalRead(PIN_PRESENCE));
  Serial.println(alarm);
  digitalWrite(PIN_ALARM, alarm);
}

// put function definitions here:
void checkRange() {

  if (sensor.available() > 0)
  {
    char c = sensor.read();
    if (c != '\n')
    {
      msg += c;
    }
    else
    {
      if (msg.substr(0,6) == "Range ")
      {
        range = atoi(msg.substr(6).c_str());

      }
      msg.clear();

    }
  }
}