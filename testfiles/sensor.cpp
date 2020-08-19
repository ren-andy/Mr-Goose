#include <wiringPi.h>
#include <softTone.h>
#include <iostream> 
#include <cstdint>

#define MAX_TIMINGS 85 // number of bits sent by sensor that will be read and stored 
//#define LED_PIN 0
#define DHT_PIN 1
#define PIR_PIN 2
#define PB_PIN 3

int data[5] = {0, 0, 0, 0, 0};

void read_dht_data() {

		uint8_t laststate = HIGH;
		uint8_t counter = 0; 
		uint8_t j = 0, i; 
		
		// 0 8bit humidity integer data (overflow)
		// 1 8bit humidity decimal data
		// 2 8bit temperature integer data (overflow) 
		// 3 8bit temperature decimal data
		// 4 8 bit parity bit 
		data[0] = data[1] = data[2] = data[3] = data[4] = 0;
		
		// Pull pin down for 18 milliseconds
		pinMode( DHT_PIN, OUTPUT ); 
		digitalWrite( DHT_PIN, LOW );
		delay( 18 ); 
		
		// Pull pin back up for 40 microseconds
		digitalWrite( DHT_PIN, HIGH );
		delayMicroseconds(40);
		
		// Prepare to read pin
		pinMode( DHT_PIN, INPUT);
		
		// Setup LED output pin 
		pinMode(0, OUTPUT); 
		digitalWrite(0, HIGH); 
		
		// Detect change and read data 
		for( i = 0; i < MAX_TIMINGS; ++i ) {
			counter = 0; // Counts the number of loops where the signal did not change 
			// Detects how long a high/low signal persisted for in microseconds.
			// Time a on/off portion of a continuous signal 
			// 26-28 us for low (0), 70 us for a high (1)  
			
			while( digitalRead(DHT_PIN) == laststate) { 
				counter++; 
				//forloop iteration time * maxtimings = 3*85 = 255 
				delayMicroseconds(1); // Delay counter by 1 microsecond
				if( counter == 255 ) { 
					break;
				}
			}
			laststate = digitalRead(DHT_PIN); // last state set as the loop breaking state 
			
			if( counter == 255 ) { // largest number storable in 8 bits (a byte) 
				break;
			}
			
			// Shove incoming data bits into storage bytes 
			// Top three transitions are ignored 
			if( (i >= 4) && (i % 2 == 0) ) {
				// j/8 means it will point to same byte for 8 values of j. 
				data[j / 8] <<= 1; // Left shift by 1 bit a low(0) bit passed serially
				if( counter > 30 ) { // High signal longer than 28 us is a high(1) bit passed serially
					data[j / 8] |=1;
					
				}
				++j;
			}
		} 
			
		for(size_t k = 0; k < 5; ++k) {
				std::cout << data[k] << " "; 
		}
		
		// DHT22 sends 40 bits of data at once 
		if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) { 
			float h = (float)((data[0] << 8) + data[1]) / 10; 
			float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10; 
			if( data[2] & 0x80 ) {
				c = -c; 
			}
			
			float f = c*1.8f + 32; 
			
			/*for(size_t k = 0; k < 5; ++k) {
				std::cout << data[k] << " "; 
			}*/
			
			std::cout << "Humidity = " << h << " Temperature = " << c << "*C (" << f << "*F)" << std::endl; 
		} else { 
			std::cout << "Data not good, skip" << std::endl;
		}
		
		delay(500);
		digitalWrite(0, LOW); 
}

void sense_surrounding() {
	
	// Initialize the GPIO pins for the PIR sensor and the Piezo Buzzer 
	pinMode(PIR_PIN, INPUT);
	softToneCreate(PB_PIN);

	std::cout << (int)digitalRead(PIR_PIN) << std::endl; 
	
	while(digitalRead(PIR_PIN) == HIGH) { 
		std::cout << "Motion detected: " << (int)digitalRead(PIR_PIN) << std::endl;
		softToneWrite(PB_PIN, 196); // g
		delay(500);
		softToneWrite(PB_PIN, 220); // a
		delay(500);
		softToneWrite(PB_PIN, 246); // b
		delay(500);
		softToneWrite(PB_PIN, 262); // c
		delay(500);
		softToneWrite(PB_PIN, 294); // d
		delay(500);
		softToneWrite(PB_PIN, 330); // e
		delay(500);
		softToneWrite(PB_PIN, 370); // f#
		delay(500);
		softToneWrite(PB_PIN, 392); // g
		delay(500);
		softToneWrite(PB_PIN, 0);
	}
}

int main() {
	
	std::cout << "Raspberry Pi DHT22 test " << std::endl;
	
	if( wiringPiSetup() == -1 ) {
		exit(1);
	}
	
	while(true) {
		read_dht_data();
		delay(2000);
		
		if(digitalRead(PIR_PIN) == HIGH) {
			sense_surrounding();
		}
	}
	
	return 0; 
}
