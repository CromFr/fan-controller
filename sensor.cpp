#include "sensor.h"

Sensor::Sensor(const char* name, uint8_t pin)
: name(name), pin(pin){

}

void Sensor::setup(){
	pinMode(pin, INPUT);
}

double Sensor::rawTemp() const {
	double tempLog = log(10000.0 * (1024.0 / analogRead(pin) - 1));
	return 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempLog * tempLog ))* tempLog ) - 273.15;
}


double Sensor::smoothedTemp() const {
	return temp;
}

void Sensor::measureTemp(){
	temp = (temp * (double)counter + rawTemp()) / ((double)counter + 1.0);
	counter++;
}
void Sensor::resetTemp(){
	counter = 0;
}