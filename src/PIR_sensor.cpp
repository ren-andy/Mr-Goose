#include <iostream> 
#include <wiringPi.h>
#include <softTone.h>
#include "GPIO_pins.h"
#include "PIR_sensor.h"

PIR::PIR(): 
MOT_PIN{PIR_PIN}
{} 

PIR::~PIR() {

}


void PIR::sense_surrounding() {
    // Initialize the GPIO pins for the PIR sensor and the piezo buzzer 
	pinMode(PIR_PIN, INPUT);
	softToneCreate(PB_PIN);

	// std::cout << (int)digitalRead(PIR_PIN) << std::endl; 

    while(digitalRead(PIR_PIN) == HIGH) { 
		std::cout << "Motion detected: " << (int)digitalRead(PIR_PIN) << std::endl;
        softToneWrite(PB_PIN, 440); // Outputs a frequency of 440 Hz to the active buzzer 
		delay(500);
    }
}