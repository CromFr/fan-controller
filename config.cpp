#include "config.h"

const uint8_t buttonPin = 0;
const uint8_t ledPin = 12   ;//Blink when in low speed mode


const Buzzer buzzer = Buzzer{
	pin: 8 // beep beep beep. Must be analog or PWM compatible. -1 to disable beeping.
};

// List of temperature sensors
Sensor sensors[] = {
	Sensor{
		name: "Water", // Sensor name used when sending its temperature to serial
		pin: A2, // Sensor pin. Must be analog
	},
	Sensor{
		name: "Air",
		pin: A3,
	},
};
const uint8_t sensorsLength = sizeof(sensors) / sizeof(Sensor);


// This is an example curve for speed control
static Fan::Point defaultCurve[] = {
	//  temp °C => speed %
	Fan::Point{ 30, 0 },
	Fan::Point{ 30, 30 },
	Fan::Point{ 45, 60 },
	Fan::Point{ 50, 80 },
	Fan::Point{ 55, 100 },
};
static uint8_t defaultCurveLength = sizeof(defaultCurve) / sizeof(Fan::Point);

// List of fans
Fan fans[] = {
	Fan{
		// Fan name used when sending its speed to serial
		name: "Front rad 240",
		// Fan pin. Must be PWM compatible
		pin: 9,

		// Associated sensor in `sensors` list. Will be used to calculate fan speed with speedCurve.
		sensor: &sensors[0],
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,

		tachoPin: -1,
	},
	Fan{
		// Fan name used when sending its speed to serial
		name: "Top rad 360",
		// Fan pin. Must be PWM compatible
		pin: 10,

		// Associated sensor in `sensors` list. Will be used to calculate fan speed with speedCurve.
		sensor: &sensors[0],
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,

		tachoPin: -1,
	},
	Fan{
		// Fan name used when sending its speed to serial
		name: "Rear 120",
		// Fan pin. Must be PWM compatible
		pin: 11,

		// Associated sensor in `sensors` list. Will be used to calculate fan speed with speedCurve.
		sensor: &sensors[0],
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,

		tachoPin: -1,
	},
};
const uint8_t fansLength = sizeof(fans) / sizeof(Fan);


// Display temp and fan speed to a SSD1306 display
const bool hasDisplay = true;
// You have to modify Adafruit_SSD1306.h to configure the display (I know, its a mess)
