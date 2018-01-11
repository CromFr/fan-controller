#ifndef __INCLUDED_BUZZER__
#define __INCLUDED_BUZZER__

#include "config.h"



void beep(const char* signal){
	char c;
	while(c = *(signal++)){

		switch(c){
			case '.':
				digitalWrite(buzzerPin, 1);
				delay(30);
				digitalWrite(buzzerPin, 0);
				break;
			case '-':
				digitalWrite(buzzerPin, 1);
				delay(150);
				digitalWrite(buzzerPin, 0);
				break;
			default:
				delay(100);
				break;

		}
		if(*signal != 0)
			delay(100);
	}
}

void beepNumber(int number){

	for(int i = pow(10, (int)log10(number)); i > 0; i /= 10){
		int digit = number / i;
		number -= digit * i;
		switch(digit){
			case 0: beep("--"); break;
			case 1: beep("."); break;
			case 2: beep(".."); break;
			case 3: beep("..."); break;
			case 4: beep(".-"); break;
			case 5: beep("-"); break;
			case 6: beep("-."); break;
			case 7: beep("-.."); break;
			case 8: beep("-..."); break;
			case 9: beep(".--"); break;
			default: beep("----------");
		}
		if(i > 1)
			delay(300);
	}
}


#endif