#ifndef __INCLUDED_SENSOR__
#define __INCLUDED_SENSOR__

#include <Arduino.h>

class Sensor {
public:
	Sensor(const char* name, uint8_t pin);

	void setup();

	double rawTemp() const;


	double smoothedTemp() const;

	void measureTemp();
	void resetTemp();

	const char* name;
	const uint8_t pin;

private:
	double temp = 0.0;
	uint16_t counter = 0;
};

#endif