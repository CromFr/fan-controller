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
	for(size_t i = 0 ; i < sensorsLength ; i++)
		sensors[i] = Sensor(&sensorDefs[i]);

	fans = new Fan[fansLength];
	for(size_t i = 0 ; i < fansLength ; i++){
		fans[i] = Fan(&fanDefs[i], &sensors[fanDefs[i].sensorIndex]);
	}

	if(hasDisplay){
		display = Display(sensors, sensorsLength, fans, fansLength);
	}
}

void loop() {
	static size_t counter = 0;
	static Mode mode = Mode::Auto;


	// average temp over a second
	for(size_t i = 0 ; i < sensorsLength ; i++){
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

			for(size_t i = 0 ; i < sensorsLength ; i++){
				auto& sensor = sensors[i];
				Serial.print("Sensor;");
				Serial.print(sensor.def->name);
				Serial.print(";");
				Serial.print(sensor.smoothedTemp());
				Serial.print("°C");
				Serial.println("");
			}

			for(size_t i = 0 ; i < fansLength ; i++){
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
		for(size_t i = 0 ; i < fansLength ; i++){
			fans[i].setModeSpeed(mode);
		}

		if(hasDisplay){
			//Update display
			display.update(mode);
		}


		for(size_t i = 0 ; i < fansLength ; i++){
			if(fans[i].hasTacho()){
				fans[i].resetTacho();
			}
		}

		bool hasTempWarning = false;
		for(size_t i = 0 ; i < sensorsLength ; i++){
			auto& maxTemp = sensors[i].def->maxTemp;
			if(!isnan(maxTemp) && sensors[i].smoothedTemp() >= maxTemp){
				hasTempWarning = true;
			}
		}

		static bool state = false;
		state = !state;
		if(mode == Mode::Low || hasTempWarning){
			// Blink warning led
			digitalWrite(ledPin, state);
		}

		if(hasTempWarning){
			// Buzz if dangerous temp
			if(buzzer.isActive()) buzzer.set(state);
			else buzzer.set(false);
		}

	}

	counter = (counter + 1 ) % 20;
	if(counter == 0){
		for(size_t i = 0 ; i < sensorsLength ; i++){
			sensors[i].resetTemp();
		}
	}
	delay(50);
}
