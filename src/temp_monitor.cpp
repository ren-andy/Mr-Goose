/*
 * temp_monitor.cpp
 * 
 * Description: dht22 temperature sensor implementation file 
 * Author: Andy Ren (ren-andy)
 * Date: 7/9/2020
 *
 */

#include <cstdint>
#include <iostream> 
#include <wiringPi.h>
#include "GPIO_pins.h"
#include "temp_monitor.h"

// Constructor
dht22::dht22() : 
TEMP_PIN{DHT_PIN},
MAX_TIMINGS{85}, 
data{new int[5]}
{}

// Destructor
dht22::~dht22() {

    delete[] data;
    data = nullptr; 
}

// reads temp sensor and prints data.  
void dht22::read_dht_data() {
        uint8_t laststate = HIGH;
		uint8_t counter = 0; 
		uint8_t j = 0, i; 
		
		// 0 8bit humidity integer data (overflow)
		// 1 8bit humidity decimal data
		// 2 8bit temperature integer data (overflow) 
		// 3 8bit temperature decimal data
		// 4 8 bit parity bit 
		this->data[0] = this->data[1] = this->data[2] = this->data[3] = this->data[4] = 0;
		
		// Pull pin down for 18 milliseconds
		pinMode( DHT_PIN, OUTPUT ); 
		digitalWrite( DHT_PIN, LOW );
		delay( 18 ); 
		
		// Pull pin back up for 40 microseconds
		digitalWrite( DHT_PIN, HIGH );
		delayMicroseconds(40);
		
		// Prepare to read temperature pin
		pinMode(DHT_PIN, INPUT);
		
		// Setup LED output pin 
		pinMode(LED_PIN, OUTPUT); 
		digitalWrite(LED_PIN, HIGH); // Turn on LED
		
		// Detect change and read data 
		for( i = 0; i < MAX_TIMINGS; ++i ) {
			counter = 0; // Counts the number of loops where the output did not change 
			// Reads the binary bits of a temperature value. 
			// Time on/off portion of a binary value, which consists of a low reset portion and high length
			// high length of 26-28 us represents a 0 bit in value, 70 us for a 1 bit 
			
			while( digitalRead(DHT_PIN) == laststate) {  
				counter++; 
				//forloop iteration time * maxtimings = 3*85 = 255 
				delayMicroseconds(1); // increase counter by 1 microsecond to match output signals
				if( counter == 255 ) { // largest number storable in 8 bits.
					break;
				}
			}
			laststate = digitalRead(DHT_PIN); // last state set as the loop breaking state 
			
			if( counter == 255 ) { // largest number storable in 8 bits (a byte), usually doesn't occur
				break;
			}
			
			// Shove incoming data bits into storage bytes 
			// first three signal transitions are ignored, as they are intialization between controller and sensor
			if( (i >= 4) && (i % 2 == 0) ) { // only adds read bit if it is from a signal high 
				// j/8 means it will round to same index for 8 values of j. 
				this->data[j / 8] <<= 1; // Left shift by 1 bit a 0 bit passed serially
				if( counter > 30 ) { // High signal longer than 28 us is a 1 bit passed serially
					this->data[j / 8] |=1; //bit OR'ed to ensure a 1 is passed in
				}
				++j;
			}
		} 
			
		// for(size_t k = 0; k < 5; ++k) {
		// 		std::cout << data[k] << " "; 
		// }
		
		// DHT22 sends 40 bits of useful data from 85 databits 
		if ((j >= 40) && (this->data[4] == ((this->data[0] + this->data[1] + this->data[2] + this->data[3]) & 0xFF))) { // Verify that 40 bits were sent and checksum matches. 
			float h = (float)((this->data[0] << 8) + this->data[1]) / 10; // humidity = (overflow *64 + decimal data) / 10
			float c = (float)(((this->data[2] & 0x7F) << 8) + this->data[3]) / 10; // temperature = (overflow & 0x7f * 64 + decimal data) / 10
			if(this->data[2] & 0x80) { // Checks if the largest overflow bit is a 1 or not, determines if temp is negative 
				c = -c; 
			}
			
			float f = c*1.8f + 32; // Fahrenheit  
			
			/*for(size_t k = 0; k < 5; ++k) {
				std::cout << data[k] << " "; 
			}*/
			
			std::cout << "Humidity = " << h << " Temperature = " << c << "*C (" << f << "*F)" << std::endl; 
		} // } else { 
		// 	std::cout << "Data not good, skip" << std::endl;
		// }
		
		delay(500);
		digitalWrite(LED_PIN, LOW); // Turn off LED 

        delay(2000); // dht pin requires 2 second delay between each temperature read 
}

