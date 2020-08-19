#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H


class PIR {
public:

    PIR();
    ~PIR();

    void sense_surrounding(); 

private:  

    int MOT_PIN;

};

#endif //TEMP_MONITOR_H