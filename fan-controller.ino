#include <Arduino.h>
#include "display.h"
#include "buzzer.h"

#include "config.h"


Sensor* sensors;
Fan* fans;
Buzzer buzzer;
Display display;


void setup() {
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);
	analogWrite(ledPin, 0);

	pinMode(buttonPin, INPUT_PULLUP);

	buzzer = Buzzer(buzzerPin);

	sensors = new Sensor[sensorsLength];
	for(uint8_t i = 0 ; i < sensorsLength ; i++)
		sensors[i] = Sensor(&sensorDefs[i]);

	fans = new Fan[fansLength];
	for(uint8_t i = 0 ; i < fansLength ; i++){
		fans[i] = Fan(&fanDefs[i], &sensors[fanDefs[i].sensorIndex]);
	}

	if(hasDisplay){
		display = Display(sensors, sensorsLength, fans, fansLength);
	}
}

void loop() {
	static uint8_t counter = 0;
	static Mode mode = Mode::Auto;


	// average temp over a second
	for(uint8_t i = 0 ; i < sensorsLength ; i++){
		sensors[i].measureTemp();
	}

	// Handle button actions
	if(digitalRead(buttonPin) == 0){
		auto startPress = millis();
		bool longPress = false;

		while(digitalRead(buttonPin) == 0){
			auto duration = millis() - startPress;
			if(duration > 300){
				longPress = true;
				break;
			}
		}

		if(longPress){
			// Change mode
			mode = (Mode)(((int)mode + 1) % 4);
			if(buzzer.isActive()){
				if(hasDisplay)
					buzzer.beep(".");
				else{
					switch(mode){
						case Mode::Auto: buzzer.beep(".-");   break;
						case Mode::Low:  buzzer.beep(".-.."); break;
						case Mode::High: buzzer.beep("...."); break;
						case Mode::Full: buzzer.beep("..-."); break;
						default: buzzer.beep("----------");
					}
				}
			}

			// Will force fan speed set immediately with current temp values
			counter = 0;

			//Switch off warning led
			digitalWrite(ledPin, false);

			Serial.print("Mode;");
			Serial.println(modeToStr(mode));
		}
		else{
			// Display temps & speeds
			Serial.print("Mode;");
			Serial.println(modeToStr(mode));

			for(uint8_t i = 0 ; i < sensorsLength ; i++){
				auto& sensor = sensors[i];
				Serial.print("Sensor;");
				Serial.print(sensor.def->name);
				Serial.print(";");
				Serial.print(sensor.smoothedTemp());
				Serial.print("°C");
				Serial.println("");
			}

			for(uint8_t i = 0 ; i < fansLength ; i++){
				auto& fan = fans[i];
				Serial.print("Fan;");
				Serial.print(fan.def->name);
				Serial.print(";");
				Serial.print(fan.currentSpeed());
				Serial.print("%");
				if(fan.hasTacho()){
					Serial.print(";");
					Serial.print(fan.getRPM());
					Serial.print("RPM");
				}
				Serial.println("");
			}

			// Beep first temp
			if(buzzer.isActive())
				buzzer.beepNumber(sensors[0].smoothedTemp());
		}

		// Wait release
		while(digitalRead(buttonPin) == 0){}
	}

	// Set fan speeds
	if(counter == 0){
		// Fan control
		for(uint8_t i = 0 ; i < fansLength ; i++){
			fans[i].setModeSpeed(mode);
		}

		if(hasDisplay){
			// Update display
			display.update(mode);
		}

		bool ledBlink = false;
		bool ledFlash = false;
		bool buzzerLongBeep = false;
		bool buzzerShortBeep = false;


		// Sensor temp warning & malfunction detection
		for(uint8_t i = 0 ; i < sensorsLength ; i++){
			auto temp = sensors[i].smoothedTemp();
			if(temp < -273.0 || temp > 273.0){
				// -273.1 means the sensor is disconnected
				// +273.1 means the sensor is short circuited
				buzzerShortBeep = true;
			}

			auto& maxTemp = sensors[i].def->maxTemp;
			if(!isnan(maxTemp) && temp >= maxTemp){
				buzzerLongBeep = true;
			}
		}

		if(mode == Mode::Low)
			ledBlink = true;

		static bool stateSwitch = false;
		stateSwitch = !stateSwitch;

		// Led
		if(ledBlink)
			digitalWrite(ledPin, stateSwitch);
		else if(ledFlash){
			digitalWrite(ledPin, 1);
			delay(50);
			digitalWrite(ledPin, 0);
		}
		else{
			digitalWrite(ledPin, 0);
		}

		// Buzzer
		if(buzzerLongBeep){
			// Buzz if dangerous temp
			if(buzzer.isActive()) buzzer.set(stateSwitch);
			else buzzer.set(false);
		}
		else if(buzzerShortBeep){
			buzzer.beep(".");
		}


		// Tachometer counters reset
		for(uint8_t i = 0 ; i < fansLength ; i++){
			if(fans[i].hasTacho()){
				fans[i].resetTacho();
			}
		}
	}

	counter = (counter + 1 ) % 20;
	if(counter == 0){
		for(uint8_t i = 0 ; i < sensorsLength ; i++){
			sensors[i].resetTemp();
		}
	}
	delay(50);
}
