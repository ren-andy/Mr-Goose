all: goosemonitor

goosemonitor: ./src/main.cpp ./src/piezo.cpp ./src/piezo.h ./src/PIR_sensor.cpp ./src/PIR_sensor.h ./src/temp_monitor.cpp ./src/temp_monitor.h
	g++ -std=c++11 -o goosemonitor ./src/main.cpp ./src/piezo.cpp ./src/piezo.h ./src/PIR_sensor.cpp ./src/PIR_sensor.h ./src/temp_monitor.cpp ./src/temp_monitor.h -lwiringPi -lpthread

debug: ./src/main.cpp ./src/piezo.cpp ./src/piezo.h ./src/PIR_sensor.cpp ./src/PIR_sensor.h ./src/temp_monitor.cpp ./src/temp_monitor.h
	g++ -std=c++11 -g -o goosemonitor ./src/main.cpp ./src/piezo.cpp ./src/piezo.h ./src/PIR_sensor.cpp ./src/PIR_sensor.h ./src/temp_monitor.cpp ./src/temp_monitor.h -lwiringPi -lpthread

clean:
	rm *.o goosemonitor goosemonitor.exe