#ifndef __INCLUDED_FAN__
#define __INCLUDED_FAN__




enum class Mode
{
	Auto,
	Low,
	High,
	Full,
};
const char* modeToStr(Mode mode){
	switch(mode){
		case Mode::Auto: return "Auto";
		case Mode::Low:  return "Low";
		case Mode::High: return "High";
		case Mode::Full: return "Full";
	}
	return "UNKNOWN";
}



class Sensor {
public:
	Sensor(const char* name, size_t pin)
	: name(name), pin(pin){

	}

	double rawTemp() const {
		double tempLog = log(10000.0 * (1024.0 / analogRead(pin) - 1));
		return 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempLog * tempLog ))* tempLog ) - 273.15;
	}


	double smoothedTemp() const {
		return temp;
	}

	void measureTemp(){
		temp = (temp * (double)counter + rawTemp()) / ((double)counter + 1.0);
		counter++;
	}
	void resetTemp(){
		counter = 0;
	}

	const char* name;
	const size_t pin;

private:
	double temp = 0.0;
	size_t counter = 0;
};




class Fan {
public:
	struct Point {
		double temp;
		unsigned short speed;// 0 -> 100
	};

	Fan(const char* name, size_t pin, size_t sensor, Point* speedCurve, size_t speedCurveLength)
	: name(name), pin(pin), sensor(sensor), speedCurve(speedCurve), speedCurveLength(speedCurveLength){

		minSpeed = 100;
		maxSpeed = 0;
		for(int j = 0; j < speedCurveLength; j++){
			auto&& speed = speedCurve[j].speed;

			if(speed > 0 && speed < minSpeed)
				minSpeed = speed;
			if(speed < 100 && speed > maxSpeed)
				maxSpeed = speed;
		}
	}
	const char* name;

	const size_t pin;
	const Sensor* sensor;

	const Point* speedCurve;
	const size_t speedCurveLength;

	int currentSpeed() const {
		return speed;
	}

	int setSpeed(int speed){
		this->speed = speed;

		switch (pin) {
			case 9:
				OCR1A = speed / 100.0 * 320;
				break;
			case 10:
				OCR1B = speed / 100.0 * 320;
				break;
			default:
				analogWrite(pin, speed / 100.0 * 255);
				break;
		}
	}


	int setModeSpeed(Mode mode){
		switch(mode){
			case Mode::Auto:
				setSpeed(calcSpeed(sensor->smoothedTemp()));
				break;
			case Mode::Low:
				setSpeed(minSpeed);
				break;
			case Mode::High:
				setSpeed(maxSpeed);
				break;
			case Mode::Full:
				setSpeed(100);
				break;
		}
	}


private:
	short speed, minSpeed, maxSpeed;

	int calcSpeed(double temp) const {
		Point* start = nullptr;
		Point* end = nullptr;

		for(int i = 0 ; i < speedCurveLength ; i++){

			if(speedCurve[i].temp > temp){
				end = &speedCurve[i];
				break;
			}
			start = &speedCurve[i];
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
};




#endif