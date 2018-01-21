#ifndef __INCLUDED_SENSOR__
#define __INCLUDED_SENSOR__

#include <Arduino.h>


struct SensorDef{
	char* name;
	uint8_t pin;
};


class Sensor {
public:
	Sensor(){}
	Sensor(const SensorDef* def);

	double rawTemp() const;


	double smoothedTemp() const;

	void measureTemp();
	void resetTemp();

	const SensorDef* def;

private:
	double temp = 0.0;
	uint16_t counter = 0;
};

#endif