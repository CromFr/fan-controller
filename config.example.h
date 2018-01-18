#ifndef __INCLUDED_CONFIG__
#define __INCLUDED_CONFIG__
#include "fan.h"
#include "display.h"

int buttonPin = 2;
int buzzerPin = A1;// beep beep beep. Must be analog or PWM compatible. -1 to disable beeping.
int ledPin = LED_BUILTIN;//Blink when in low speed mode


// List of temperature sensors
Sensor sensors[] = {
	Sensor{
		name: "Water", // Sensor name used when sending its temperature to serial
		pin: A0, // Sensor pin. Must be analog
	},
};
size_t sensorsLength = sizeof(sensors) / sizeof(Sensor);


// This is an example curve for speed control
Fan::Point defaultCurve[] = {
	//  temp °C => speed %
	Fan::Point{ 30.0, 0 },
	Fan::Point{ 30.0, 30 },
	Fan::Point{ 45.0, 60 },
	Fan::Point{ 50.0, 80 },
	Fan::Point{ 55.0, 100 },
};
size_t defaultCurveLength = sizeof(defaultCurve) / sizeof(Fan::Point);

// List of fans
Fan fans[] = {
	Fan{
		// Fan name used when sending its speed to serial
		name: "Front",
		// Fan pin. Must be PWM compatible
		pin: 9,

		// Associated sensor in `sensors` list. Will be used to calculate fan speed with speedCurve.
		sensor: &sensors[0],
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,
	},
};
size_t fansLength = sizeof(fans) / sizeof(Fan);


// Display temp and fan speed to a SSD1306 display
bool hasDisplay = true;
// You have to modify Adafruit_SSD1306.h to configure the display (I know, its a mess)

#endif