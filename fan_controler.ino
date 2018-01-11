// #include "buzzer.h"
// #include "speed_curve.h"

enum class Mode
{
	Auto,
	Low,
	High,
	Full,
};
const char* modeToStr(size_t mode){
	switch((Mode)mode){
		case Mode::Auto: return "Auto";
		case Mode::Low:  return "Low";
		case Mode::High: return "High";
		case Mode::Full: return "Full";
	}
	return "UNKNOWN";
}

Mode mode;
double* temp;



void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	analogWrite(LED_BUILTIN, 0);

	pinMode(PIN_BUZZER, OUTPUT);
	pinMode(PIN_BUTTON, INPUT_PULLUP);

	for(auto& probe : probes){
		pinMode(probe.pin, INPUT);
	}
	temp = new double[probesLength];

	for(auto& fan : fans){
		pinMode(fan.pin, OUTPUT);
	}
}


size_t counter = 0;

void loop() {

	counter = (counter + 1 ) % 20;

	for(size_t i = 0 ; i < probesLength ; i++){
		temp[i] = (temp[i] * (double)counter + TempProbe_getTemp(probes[0])) / ((double)counter + 1.0);
	}

	if(digitalRead(PIN_BUTTON) == 0){
		auto startPress = millis();
		bool longPress = false;

		while(digitalRead(PIN_BUTTON) == 0){
			auto duration = millis() - startPress;
			if(duration > 500){
				longPress = true;
				break;
			}
		}

		if(longPress){
			mode = (Mode)(((int)mode + 1) % 4);
			switch(mode){
				case Mode::Auto: beep(".-");   break;
				case Mode::Low:  beep(".-.."); break;
				case Mode::High: beep("...."); break;
				case Mode::Full: beep("..-."); break;
				default: beep("----------");
			}

			counter = 0;

			Serial.print("Mode  ");
			Serial.println(modeToStr((size_t)mode));
		}
		else{
			Serial.print("Temp ");
			for(size_t i = 0 ; i < probesLength ; i++){
				Serial.print(" ");
				Serial.print(probes[i].name);
				Serial.print(": ");
				Serial.print(temp[i]);
			}
			Serial.println("");

			Serial.print("Speed");
			for(size_t i = 0 ; i < fansLength ; i++){
				Serial.print(" ");
				Serial.print(fans[i].name);
				Serial.print(": ");
				Serial.print(Fan_calcSpeed(fans[i], temp[i]));
			}
			Serial.println("");

			// Beep first temp
			beepNumber(temp[0]);
		}

		// Wait release
		while(digitalRead(PIN_BUTTON) == 0){}
	}


	if(counter == 0){
		switch(mode){
			case Mode::Auto:
				for(size_t i = 0 ; i < fansLength ; i++){
					analogWrite(fans[i].pin, Fan_calcSpeed(fans[i], temp[fans[i].tempProbe]) / 100.0 * 255);
				}
				break;
			case Mode::Low:
				for(size_t i = 0 ; i < fansLength ; i++){
					analogWrite(fans[i].pin, (fans[i].low / 100.0) * 255);
				}
				break;
			case Mode::High:
				for(size_t i = 0 ; i < fansLength ; i++){
					analogWrite(fans[i].pin, (fans[i].high / 100.0) * 255);
				}
				break;
			case Mode::Full:
				for(size_t i = 0 ; i < fansLength ; i++){
					analogWrite(fans[i].pin, 255);
				}
				break;

		}
	}

	delay(50);
}
