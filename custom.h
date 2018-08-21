#ifndef __INCLUDED_CUSTOM__
#define __INCLUDED_CUSTOM__

class Sensor;
class Fan;

struct CustomDef{
	char* name;
	const PROGMEM uint8_t* icon;
	void (*value)(char* buffer, const Sensor* sensors, const Fan* fans);
};

#endif