#ifndef __INCLUDED_CONFIG__
#define __INCLUDED_CONFIG__
#include "fan.h"

#define PIN_BUTTON 2
#define PIN_BUZZER A1



TempProbe probes[] = {
	TempProbe{
		name: "Water",
		pin: A0
	},
};
size_t probesLength = sizeof(probes) / sizeof(TempProbe);


Point defaultCurve[] = {
	//  temp °C => speed %
	Point{ 30.0, 0 },
	Point{ 30.0, 30 },
	Point{ 45.0, 60 },
	Point{ 50.0, 80 },
	Point{ 55.0, 100 },
};


Fan fans[] = {
	Fan{
		name: "WaterRadiator",
		pin: 9,
		tempProbe: 0,
		speedCurve: defaultCurve,
		speedCurveLength: sizeof(defaultCurve),
		low: 30, high: 80,
	},
};
size_t fansLength = sizeof(fans) / sizeof(Fan);




#endif