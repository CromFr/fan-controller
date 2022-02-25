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
		// Max temperature, that will make the buzzer beep as warning. Set to NAN to disable
		maxTemp: 50.0,
	},
	{
		name: "Air",
		pin: A3,
		maxTemp: NAN,
	},
};
const uint8_t sensorsLength = sizeof(sensorDefs) / sizeof(SensorDef);


// This is an example curve for speed control
static SpeedCurve defaultCurve = {
	//  temp °C => speed %
	{ 30.0, 20 },
	{ 45.0, 60 },
	{ 50.0, 70 },
	{ 55.0, 100 },
};
static uint8_t defaultCurveLength = sizeof(defaultCurve) / sizeof(SpeedCurvePoint);

// List of fans
const FanDef fanDefs[] = {
	{
		// Fan name used when sending its speed to serial
		name: "Front rad",
		// Fan pin. Must be PWM compatible
		pin: 11,
		// Tachometer fan pin. This pin MUST support interruptions
		tachoPin: 2,

		// Associated sensor index in `sensorDefs` list. Will be used to calculate fan speed with speedCurve.
		sensorIndex: 0,
		// Curve used to calculate fan speed in Auto mode
		speedCurve: defaultCurve,
		// Number of points on speedCurve
		speedCurveLength: defaultCurveLength,
		// true to start the fan at 100% for half a second
		kickStart: false,
	},
	{
		name: "Top rad",
		pin: 10,
		tachoPin: 3,
		sensorIndex: 0,
		speedCurve: defaultCurve,
		speedCurveLength: defaultCurveLength,
		kickStart: false,
	},
	{
		name: "Rear fan",
		pin: 9,
		tachoPin: -1,
		sensorIndex: 1,
		speedCurve: SpeedCurve{
			{ 30.0, 30 },
			{ 40.0, 70 },
			{ 45.0, 100 },
		},
		speedCurveLength: 3,
		kickStart: false,
	},
	{
		name: "Bot fan",
		pin: 6,
		tachoPin: -1,
		sensorIndex: 1,
		speedCurve: SpeedCurve{
			{ 30.0, 15 },
			{ 40.0, 20 },
			{ 45.0, 50 },
		},
		speedCurveLength: 3,
		kickStart: true,
	},
};
const uint8_t fansLength = sizeof(fanDefs) / sizeof(FanDef);


static const PROGMEM uint8_t deltaIcon[] = {
	0b00000000,
	0b01100000,
	0b01011000,
	0b01000110,
	0b01011000,
	0b01100000,
	0b00000000,
};
const CustomDef customDefs[] = {
	{
		name: "Delta",
		icon: deltaIcon,
		value: [](char* buffer, const Sensor* sensors, const Fan* fans){
			auto val = sensors[0].smoothedTemp() - sensors[1].smoothedTemp();
			dtostrf(val,5,1,buffer);
		},
	},
};
const uint8_t customLength = sizeof(customDefs) / sizeof(CustomDef);

// Is SSD1306 128x64 display connected?
const bool hasDisplay = true;
