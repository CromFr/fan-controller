#ifndef __INCLUDED_DISPLAY__
#define __INCLUDED_DISPLAY__

#include <Arduino.h>
#include "fan.h"
#include "sensor.h"


class Display {
public:
	Display(){};
	Display(Sensor* sensors, size_t sensorsLength, Fan* fans, size_t fansLength);

	void update(Mode mode);

private:

	Sensor* sensors;
	size_t sensorsLength;
	Fan* fans;
	size_t fansLength;

	uint8_t cnt;


	struct {
		Mode mode;
		uint16_t* rpm;
	} lastState;
	bool lastStateInit = true;
};



#endif