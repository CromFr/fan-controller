#ifndef __INCLUDED_FAN__
#define __INCLUDED_FAN__

#include "config.h"


struct TempProbe {
	const char* name;
	size_t pin;
};

struct Point {
	double temp;
	unsigned short speed;// 0 -> 100
};

struct Fan {
	const char* name;

	size_t pin;
	size_t tempProbe;

	Point* speedCurve;
	size_t speedCurveLength;

	unsigned short low, high;
};

double TempProbe_getTemp(const TempProbe& probe){
	double tempLog = log(10000.0*((1024.0/analogRead(probe.pin)-1)));
	return 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempLog * tempLog ))* tempLog ) - 273.15;
}


int Fan_calcSpeed(const Fan& fan, double temp){
	Point* start = nullptr;
	Point* end = nullptr;

	for(int i = 0 ; i < fan.speedCurveLength ; i++){

		if(fan.speedCurve[i].temp > temp){
			end = &fan.speedCurve[i];
			break;
		}
		start = &fan.speedCurve[i];
	}

	if(start == nullptr){
		return end->speed;
	}
	if(end == nullptr){
		return start->speed;
	}

	//interpolate
	double pitch = (double)(end->speed - start->speed) / (double)(end->temp - start->temp);
	double offset = start->speed - pitch * start->temp;
	return pitch * temp + offset;
}


#endif