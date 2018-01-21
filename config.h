#ifndef __INCLUDED_CONFIG__
#define __INCLUDED_CONFIG__

#include <Arduino.h>
#include "buzzer.h"
#include "fan.h"
#include "sensor.h"
#include "display.h"

extern const uint8_t buttonPin;
extern const uint8_t ledPin;


extern const uint8_t buzzerPin;

extern const SensorDef sensorDefs[];
extern const uint8_t sensorsLength;

extern const FanDef fanDefs[];
extern const uint8_t fansLength;


extern const bool hasDisplay;

#endif