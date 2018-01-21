#ifndef __INCLUDED_FAN__
#define __INCLUDED_FAN__

#include <Arduino.h>
#include "sensor.h"


enum class Mode
{
	Auto,
	Low,
	High,
	Full,
};
const char* modeToStr(Mode mode);

// This can handle 2 tachometers
#define TACHOMETER_MAX 2
extern volatile uint16_t tachoCounters[TACHOMETER_MAX];
extern void (*onTachoCbs[TACHOMETER_MAX])();
extern uint8_t onTachoCbsCnt;




class Fan {
public:
	struct Point {
		int8_t temp;
		uint8_t speed;// 0 -> 100
	};

	Fan(const char* name, uint8_t pin, size_t sensor, Point* speedCurve, size_t speedCurveLength, uint8_t tachoPin = -1);
	void setup();


	uint8_t currentSpeed() const {
		return speed;
	}

	void setSpeed(uint8_t speed);


	void setModeSpeed(Mode mode);

	void resetTacho(){
		tachoCounter = 0;
		tachoTimer = millis();
	}
	uint16_t getRPM();



	const char* name;

	const uint8_t pin;
	const uint8_t tachoPin;
	const Sensor* sensor;

	const Point* speedCurve;
	const uint8_t speedCurveLength;
private:
	unsigned long tachoTimer;
	volatile uint16_t* tachoCounter;

	uint8_t speed, minSpeed, maxSpeed;

	uint8_t calcSpeed(double temp) const;
};




#endif