#ifndef __INCLUDED_DISPLAY__
#define __INCLUDED_DISPLAY__

#include "Adafruit_SSD1306.h"

#define OLED_RESET 4
Adafruit_SSD1306 hwdisplay(OLED_RESET);

#include "fan.h"



static uint8_t bitmapTemp[] = {
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b00011000,
};
static uint8_t bitmapFan[3][7] = {
	{
		0b01100000,
		0b01100110,
		0b00111110,
		0b00111000,
		0b11111000,
		0b11001100,
		0b00001100,
	}, {
		0b00111000,
		0b00110000,
		0b10010110,
		0b11111110,
		0b11010010,
		0b00011000,
		0b00111000,
	}, {
		0b00001100,
		0b11001100,
		0b11111000,
		0b00111000,
		0b00111110,
		0b01100110,
		0b01100000,
	}
};
static uint8_t bitmapDegree[] = {
	0b00110000,
	0b01001000,
	0b01001000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
};

class Display {
public:
	Display(Sensor* sensors, size_t sensorsLength, Fan* fans, size_t fansLength){
		this->sensors = sensors;
		this->sensorsLength = sensorsLength;
		this->fans = fans;
		this->fansLength = fansLength;

		hwdisplay.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	}

	void update(Mode mode){
		//Notes
		// Characters are 5 * 7
		// They are spaced 6 * 8

		hwdisplay.clearDisplay();
		hwdisplay.setTextSize(1);
		hwdisplay.setTextColor(WHITE);

		size_t y = 0;

		if(mode == Mode::Low){
			hwdisplay.fillRect(0, 0, SSD1306_LCDWIDTH, 8, WHITE);
			hwdisplay.setTextColor(BLACK);
		}
		hwdisplay.setCursor(SSD1306_LCDWIDTH / 2.0 - (5 + strlen(modeToStr(mode))) * 6.0 / 2.0, y);
		hwdisplay.print("Mode:");
		hwdisplay.print(modeToStr(mode));
		hwdisplay.setTextColor(WHITE);

		y += 8;
		hwdisplay.setCursor(0, y);


		for(size_t i = 0 ; i < sensorsLength ; i++){
			auto&& sensor = sensors[i];

			hwdisplay.drawBitmap(0, y, bitmapTemp, 7, 7, 1);

			hwdisplay.setCursor(9, y);
			hwdisplay.print(sensor.name);

			// ex: 42.1°C
			//
			int tempChars = (sensor.smoothedTemp() < 0 ? 1 : 0) // sign
				+ log10(fabs(sensor.smoothedTemp())) + 1 //  digits > 0
				+  2;// dot + decimals
			hwdisplay.setCursor(SSD1306_LCDWIDTH - 6 * (tempChars + 2), y);
			hwdisplay.print(sensor.smoothedTemp(), 1);
			hwdisplay.print(" C");
			hwdisplay.drawBitmap(SSD1306_LCDWIDTH - 12, y, bitmapDegree, 5, 7, 1);

			y += 8;

			for(size_t j = 0 ; j < fansLength ; j++){
				auto&& fan = fans[j];
				if(fan.sensor == &sensor){

					if(fan.currentSpeed() > 0)
						hwdisplay.drawBitmap(6, y, bitmapFan[cnt % 3], 7, 7, 1);
					else
						hwdisplay.drawBitmap(6, y, bitmapFan[0], 7, 7, 1);


					hwdisplay.setCursor(15, y);
					hwdisplay.print(fan.name);

					// ex: 42%
					int fanChars = (fan.currentSpeed() == 0 ? 0 : log10(fan.currentSpeed())) + 1; //  digits

					hwdisplay.setCursor(SSD1306_LCDWIDTH - 6 * (fanChars + 1), y);
					hwdisplay.print(fan.currentSpeed());
					hwdisplay.print("%");

					y += 8;
				}
			}
		}

		hwdisplay.display();
		cnt++;
	}

private:


	Sensor* sensors;
	size_t sensorsLength;
	Fan* fans;
	size_t fansLength;

	size_t cnt;
};



#endif