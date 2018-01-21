#include "display.h"

#include <ssd1306.h>
// #include <ssd1306_interface.h>
void ssd1306_sendCommand(uint8_t command);
#include <nano_gfx.h>
#include <font6x8.h>

#define CHAR_WIDTH 6.0
#define CHAR_HEIGHT 8.0


static const PROGMEM uint8_t bitmapTemp[] = {
	0b000000000,
	0b000000000,
	0b001100000,
	0b011111111,
	0b011111111,
	0b001100000,
	0b000000000,
};
static const PROGMEM uint8_t bitmapFan[3][7] = {
	{
		0b00110000,
		0b00110011,
		0b00011111,
		0b00011100,
		0b01111100,
		0b01100110,
		0b00000110,
	}, {
		0b00011100,
		0b00011000,
		0b01001011,
		0b01111111,
		0b01101001,
		0b00001100,
		0b00011100,
	}, {
		0b00000110,
		0b01100110,
		0b01111100,
		0b00011100,
		0b00011111,
		0b00110011,
		0b00110000,
	}
};
static const PROGMEM uint8_t bitmapDegree[] = {
	0b00000000,
	0b00000110,
	0b00001001,
	0b00001001,
	0b00000110,
};

uint8_t canvasData[128 * 64];


static void setContrast(uint8_t dim){

	extern void  (*ssd1306_endTransmission)();
	extern void  (*ssd1306_sendByte)(uint8_t data);
	extern void (*ssd1306_commandStart)();

	ssd1306_commandStart();
	ssd1306_sendByte(0x81);
	ssd1306_sendByte(dim);
	ssd1306_endTransmission();
}

Display::Display(Sensor* sensors, size_t sensorsLength, Fan* fans, size_t fansLength){
	this->sensors = sensors;
	this->sensorsLength = sensorsLength;
	this->fans = fans;
	this->fansLength = fansLength;

	ssd1306_128x64_i2c_init();
	ssd1306_clearScreen();
	ssd1306_setFixedFont(ssd1306xled_font6x8);
	ssd1306_printFixed(ssd1306_displayWidth() / 2 - 5 * CHAR_WIDTH,  ssd1306_displayHeight() / 2 - CHAR_HEIGHT / 2,
		"Booting...", STYLE_NORMAL);

	setContrast(0);
}

void Display::update(Mode mode){
	//Notes
	// Characters are 5 * 7
	// They are spaced 6 * 8

	if(cnt == 0){
		ssd1306_clearScreen();

	}

	uint8_t x = 0;
	uint8_t y = 0;

	if(mode != lastState.mode || cnt == 0){

		if(mode == Mode::Low)
			ssd1306_negativeMode();

		ssd1306_clearBlock(0, 0, ssd1306_displayWidth(), CHAR_HEIGHT);

		x = ssd1306_displayWidth() / 2.0 - (5 + strlen(modeToStr(mode))) * CHAR_WIDTH / 2.0;
		ssd1306_printFixed(x, y, "Mode:", STYLE_NORMAL);
		ssd1306_printFixed(x + 5 * CHAR_WIDTH, y, modeToStr(mode), STYLE_NORMAL);

		if(mode == Mode::Low)
			ssd1306_positiveMode();

		lastState.mode = mode;
	}

	y += CHAR_HEIGHT;

	char buffer[24];

	for(uint8_t i = 0 ; i < sensorsLength ; i++){
		auto& sensor = sensors[i];

		if(cnt == 0){
			ssd1306_clearBlock(0, y / 8, ssd1306_displayWidth(), y / 8 + 1);
			// Icon
			ssd1306_drawBitmap(0, y / 8, 7, 8, bitmapTemp);
			// Name
			ssd1306_printFixed(9, y, sensor.name, STYLE_NORMAL);
			// °
			ssd1306_drawBitmap(ssd1306_displayWidth() - 2 * CHAR_WIDTH + 1, y / 8, 5, 8, bitmapDegree);
			// C
			ssd1306_printFixed(ssd1306_displayWidth() - 1 * CHAR_WIDTH, y, "C", STYLE_NORMAL);
		}

		// Temp value
		dtostrf(sensor.smoothedTemp(), 6, 1, buffer);
		ssd1306_printFixed(
			ssd1306_displayWidth() - (2 + 6) * CHAR_WIDTH, y,
			buffer, STYLE_NORMAL);

		y += CHAR_HEIGHT;

		for(uint8_t j = 0 ; j < fansLength ; j++){
			auto& fan = fans[j];
			if(fan.sensor == &sensor){
				if(cnt == 0){
					ssd1306_clearBlock(0, y / 8, ssd1306_displayWidth(), y / 8 + 1);

					// Name
					ssd1306_printFixed(15, y, fan.name, STYLE_NORMAL);
					// %
					ssd1306_printFixed(ssd1306_displayWidth() - 1 * CHAR_WIDTH, y, "%", STYLE_NORMAL);
				}

				// Icon
				if(fan.currentSpeed() > 0)
					ssd1306_drawBitmap(6, y / 8, 7, 8, bitmapFan[cnt % 3]);
				else
					ssd1306_drawBitmap(6, y / 8, 7, 8, bitmapFan[0]);

				//Speed
				sprintf(buffer, "%3d", fan.currentSpeed());
				ssd1306_printFixed(ssd1306_displayWidth() - (3 + 1) * CHAR_WIDTH, y,
					buffer, STYLE_NORMAL);

				y += CHAR_HEIGHT;
			}
		}
	}

	cnt++;
}
