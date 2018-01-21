#include "config.h"

const uint8_t buttonPin = 0;
const uint8_t ledPin = 12   ;//Blink when in low speed mode
const uint8_t buzzerPin = 8; // beep beep beep. Must be analog or PWM compatible. -1 to disable beeping.

// List of temperature sensors
const SensorDef sensorDefs[] = {
	{
		// Sensor name used when sending its temperature to serial
		name: "Water",
		// Sensor pin. Must be analog
		pin: A2,
	},
	{
		name: "Air",
		pin: A3,
	},
};
const uint8_t sensorsLength = sizeof(sensorDefs) / sizeof(SensorDef);


// This is an example curve for speed control
static SpeedCurve defaultCurve = {
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

		// Associated sensor index in `sensorDefs` list. Will be used to calculate fan speed with speedCurve.
		sensorIndex: 0,
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,
	},
	{
		name: "Top rad",
		pin: 10,
		tachoPin: 3,
		sensorIndex: 0,
		speedCurve: defaultCurve,
		speedCurveLength: defaultCurveLength,
	},
	{
		name: "Rear fan",
		pin: 11,
		tachoPin: -1,
		sensorIndex: 1,
		speedCurve: SpeedCurve{
			{ 30.0, 30 },
			{ 40.0, 70 },
			{ 45.0, 100 },
		},
		speedCurveLength: 3,
	},
};
const uint8_t fansLength = sizeof(fanDefs) / sizeof(FanDef);


// Is SSD1306 128x64 display connected?
const bool hasDisplay = true;
