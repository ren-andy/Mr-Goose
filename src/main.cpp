#include <iostream>
#include <wiringPi.h>
#include <string>

#include "GPIO_pins.h"
#include "temp_monitor.h"
#include "PIR_sensor.h"
#include "piezo.h"

int main() {

    std::cout << "Mr. Goose v1.0" << std::endl;
    std::cout << "Initializing GPIO pins..." << std::endl;

    if( wiringPiSetup() == -1 ) {
        std::cout << "ERROR, GPIO pins not setup." << std::endl;
		exit(1);
	}

    std::cout << "Initializing sensors..." << std::endl; 

    std::string command; 
    dht22 temp;
    PIR pir;  
    piezo buzz;
    // variable to save the location of song

    while(command != "exit") { 
        if(digitalRead(TEMP_ONLY_PIN) == HIGH) {
            temp.read_dht_data();
        } else if(digitalRead(ALL_PIN == HIGH)) {
            temp.read_dht_data();
            // run in parallel here? 
            pir.sense_surrounding(); 
        }

        if(digitalRead(SONG_BUTTON_PIN) == HIGH) {
            buzz.star_wars_opening(); 
        }
    }


    // sliding switch implementation 
    // 1 PIR sensor only  
    // 2 Both temp and PIR 
    // 3 temp 
    return 0; 
}
