#ifndef RENKEDISSOLVEDOXYGENSENSOR_H
#define RENKEDISSOLVEDOXYGENSENSOR_H

#include "Arduino.h"

class RDOSensor
{
public:
    RDOSensor(Stream *serialPort);
    RDOSensor(Stream *serialPort, int driverReceiverEnablePin);
    RDOSensor(Stream *serialPort, int driverEnablePin, int receiverEnablePi);
    bool readSensorData();
    float getOxygenSaturation();
    float getOxygenConcentration();
    float getTemperature();
    bool oxygenZeroCalibration();
    bool atmosphericPressureCalibration(float pressure);

private:
    int driverEnable;
    int receiverEnable;
    float oxygenSaturation;
    float dissolvedOxygen;
    float temperature;
    void init(Stream *serialPort, int driverEnablePin, int receiverEnablePin);
};

typedef union
{
    float value;
    byte bytes[4];
} ByteFloat;

#endif