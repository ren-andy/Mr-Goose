/*
 * temp_monitor.h
 * 
 * Description: dht22 temperature sensor header file 
 * Author: Andy Ren (ren-andy)
 * Date: 7/9/2020
 *
 */

#ifndef TEMP_MONITOR_H 
#define TEMP_MONITOR_H

class dht22 {
public:
  dht22();
  ~dht22();

  void read_dht_data(); 

private: 
    int TEMP_PIN;
    int MAX_TIMINGS; 
    int *data;
};

#endif //TEMP_MONITOR_H