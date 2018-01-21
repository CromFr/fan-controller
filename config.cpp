#include "config.h"

const uint8_t buttonPin = 0;
const uint8_t ledPin = 12   ;//Blink when in low speed mode


const Buzzer buzzer = Buzzer{
	pin: 8 // beep beep beep. Must be analog or PWM compatible. -1 to disable beeping.
};

// List of temperature sensors
Sensor sensors[] = {
	Sensor{
		// Sensor name used when sending its temperature to serial
		name: "Water",
		// Sensor pin. Must be analog
		pin: A2,
	},
	Sensor{
		name: "Air",
		pin: A3,
	},
};
const uint8_t sensorsLength = sizeof(sensors) / sizeof(Sensor);


// This is an example curve for speed control
static SpeedCurvePoint defaultCurve[] = {
	//  temp °C => speed %
	{ 30.0, 0 },
	{ 30.0, 30 },
	{ 45.0, 60 },
	{ 50.0, 80 },
	{ 55.0, 100 },
};
static uint8_t defaultCurveLength = sizeof(defaultCurve) / sizeof(SpeedCurvePoint);

// List of fans
const FanDef fanDefs[] = {
	{
		// Fan name used when sending its speed to serial
		name: "Front rad",
		// Fan pin. Must be PWM compatible
		pin: 9,
		// Tachometer fan pin. This pin MUST support interruptions
		tachoPin: 2,

		// Associated sensor in `sensors` list. Will be used to calculate fan speed with speedCurve.
		sensor: &sensors[0],
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,
	},
	{
		name: "Top rad",
		pin: 10,
		tachoPin: 3,
		sensor: &sensors[0],
		speedCurve: defaultCurve,
		speedCurveLength: defaultCurveLength,
	},
	{
		name: "Rear fan",
		pin: 11,
		tachoPin: -1,
		sensor: &sensors[0],
		speedCurve: defaultCurve,
		speedCurveLength: defaultCurveLength,
	},
};
const uint8_t fansLength = sizeof(fanDefs) / sizeof(FanDef);


// Display temp and fan speed to a SSD1306 display
const bool hasDisplay = true;
// You have to modify Adafruit_SSD1306.h to configure the display (I know, its a mess)
