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
    //piezo buzz;
    // variable to save the location of song

    // switch infinite while loop 
    while(1) { 
        //std::cin >> command; 
        //for temperature reading
        temp.read_dht_data();
        if(digitalRead(PIR_ON_PIN) == HIGH) { // detects if sliding switch has PIR sensor ON  
            pir.sense_surrounding(); 
        }

        if(digitalRead(SONG_BUTTON_PIN) == HIGH) {
            //buzz.star_wars_opening(); 
        }

    }

    return 0; 
}
