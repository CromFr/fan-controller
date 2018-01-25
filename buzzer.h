#ifndef __INCLUDED_BUZZER__
#define __INCLUDED_BUZZER__

#include <Arduino.h>


class Buzzer {
public:
	Buzzer(){};
	Buzzer(uint8_t pin);

	void setup();

	bool isActive() const{
		return pin != -1;
	}


	void beep(const char* signal) const;

	void beepNumber(int number) const;

	void set(bool on) const;

private:
	uint8_t pin;
};






#endif