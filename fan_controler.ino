#include "config.h"
// #include "buzzer.h"
// #include "speed_curve.h"





void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	analogWrite(ledPin, 0);

	pinMode(buzzerPin, OUTPUT);
	pinMode(buttonPin, INPUT_PULLUP);

	for(size_t i = 0 ; i < sensorsLength ; i++){
		auto&& sensor = sensors[i];

		pinMode(sensor.pin, INPUT);
	}

	for(size_t i = 0 ; i < fansLength ; i++){
		auto&& fan = fans[i];

		pinMode(fan.pin, OUTPUT);
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
			if(duration > 500){
				longPress = true;
				break;
			}
		}

		if(longPress){
			// Change mode
			mode = (Mode)(((int)mode + 1) % 4);
			if(buzzerPin != -1){
				switch(mode){
					case Mode::Auto: beep(".-");   break;
					case Mode::Low:  beep(".-.."); break;
					case Mode::High: beep("...."); break;
					case Mode::Full: beep("..-."); break;
					default: beep("----------");
				}
			}

			// Will force fan speed set immediately with current temp values
			counter = 0;

			//Switch off warning led
			digitalWrite(ledPin, false);

			Serial.print("Mode  ");
			Serial.println(modeToStr((size_t)mode));
		}
		else{
			// Display temps & speeds
			Serial.print("Temp ");
			for(size_t i = 0 ; i < sensorsLength ; i++){
				auto&& sensor = sensors[i];

				Serial.print(" ");
				Serial.print(sensor.name);
				Serial.print(": ");
				Serial.print(sensor.smoothedTemp());
			}
			Serial.println("");

			Serial.print("Speed");
			for(size_t i = 0 ; i < fansLength ; i++){
				auto&& fan = fans[i];

				Serial.print(" ");
				Serial.print(fan.name);
				Serial.print(": ");
				Serial.print(fan.currentSpeed());
			}
			Serial.println("");

			// Beep first temp
			if(buzzerPin != -1)
				beepNumber(sensors[0].smoothedTemp());
		}

		// Wait release
		while(digitalRead(buttonPin) == 0){}
	}

	// Set fan speeds
	if(counter == 0){
		for(size_t i = 0 ; i < fansLength ; i++){
			fans[i].setModeSpeed(mode);
		}

		if(mode == Mode::Low){
			// Blink warning led
			static bool ledPinState = false;
			digitalWrite(ledPin, ledPinState = !ledPinState);
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
