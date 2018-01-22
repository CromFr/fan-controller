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


struct SpeedCurvePoint {
	double temp;
	uint8_t speed;// 0 -> 100
};

typedef SpeedCurvePoint SpeedCurve[];

struct FanDef{
	const char* name;

	uint8_t pin;
	uint8_t tachoPin;

	uint8_t sensorIndex;

	SpeedCurvePoint* speedCurve;
	uint8_t speedCurveLength;
};


class Fan {
public:
	Fan(){};

	Fan(const FanDef* _def, const Sensor* sensor);


	uint8_t currentSpeed() const {
		return speed;
	}

	void setSpeed(uint8_t speed);
	void setModeSpeed(Mode mode);

	bool hasTacho() const {
		return def->tachoPin != (uint8_t)-1;
	}
	void resetTacho(){
		*tachoCounter = 0;
		tachoTimer = millis();
	}
	uint16_t getRPM() const;



	const FanDef* def = nullptr;
	const Sensor* sensor;
private:
	unsigned long tachoTimer;
	volatile uint32_t* tachoCounter;

	uint8_t speed, minSpeed, maxSpeed;

	uint8_t calcSpeed(double temp) const;
};




#endif