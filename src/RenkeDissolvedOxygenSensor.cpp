#include "Arduino.h"
#include <SoftwareSerial.h>
#include "RenkeDissolvedOxygenSensor.h"

bool driverHasEnablePin = true;
const byte oxygenRequestFrame[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x06, 0xc5, 0xc8};
const byte sensorZeroCalibrationFrame[] = {0x01, 0x06, 0x10, 0x10, 0x00, 0x01, 0x4d, 0x0f};
Stream *renkeModbusSerial;

void RDOSensor::init(Stream *serialPort, int driverEnablePin, int receiverEnablePin)
{
    driverEnable = driverEnablePin;
    receiverEnable = receiverEnablePin;

    pinMode(driverEnable, OUTPUT);
    pinMode(receiverEnable, OUTPUT);
    renkeModbusSerial = serialPort;
}

RDOSensor::RDOSensor(Stream *serialPort)
{
    driverHasEnablePin = false;
    init(serialPort, 0, 0);
}

RDOSensor::RDOSensor(Stream *serialPort, int driverReceiverEnablePin)
{
    driverHasEnablePin = true;
    init(serialPort, driverReceiverEnablePin, driverReceiverEnablePin);
}

RDOSensor::RDOSensor(Stream *serialPort, int driverEnablePin, int receiverEnablePin)
{
    driverHasEnablePin = true;
    init(serialPort, driverEnablePin, receiverEnablePin);
}

bool RDOSensor::readSensorData()
{
    // Enabling Modbus write on the RS485 module to send an oxygen request frame.
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, HIGH);
        digitalWrite(receiverEnable, HIGH);
    }

    // Send request bytes
    for (int i = 0; i < sizeof(oxygenRequestFrame); i++)
    {
        renkeModbusSerial->write(oxygenRequestFrame[i]); // Send each byte of the frame
    }

    // Enabling Modbus read on the RS485 module to get oxygen data
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, LOW);
        digitalWrite(receiverEnable, LOW);
    }

    byte responseBytes[17];
    int responseByteCount = 0;
    while (renkeModbusSerial->available())
    {
        responseBytes[responseByteCount] = renkeModbusSerial->read();
        responseByteCount++;
    }

    if (responseByteCount == 17)
    {
        ByteFloat fb;

        fb.bytes[3] = responseBytes[3];
        fb.bytes[2] = responseBytes[4];
        fb.bytes[1] = responseBytes[5];
        fb.bytes[0] = responseBytes[6];
        oxygenSaturation = fb.value;

        fb.bytes[3] = responseBytes[7];
        fb.bytes[2] = responseBytes[8];
        fb.bytes[1] = responseBytes[9];
        fb.bytes[0] = responseBytes[10];
        dissolvedOxygen = fb.value;

        fb.bytes[3] = responseBytes[11];
        fb.bytes[2] = responseBytes[12];
        fb.bytes[1] = responseBytes[13];
        fb.bytes[0] = responseBytes[14];
        temperature = fb.value;

        return true;
    }
    else
    {
        oxygenSaturation = 0;
        dissolvedOxygen = 0;
        temperature = 0;
    }

    return false;
}

float RDOSensor::getOxygenSaturation()
{
    return oxygenSaturation * 100.0;
}

float RDOSensor::getOxygenConcentration()
{
    return dissolvedOxygen;
}

float RDOSensor::getTemperature()
{
    return temperature;
}

bool RDOSensor::oxygenZeroCalibration()
{
    // Enabling Modbus write on the RS485 module to send an oxygen request frame.
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, HIGH);
        digitalWrite(receiverEnable, HIGH);
    }

    // Send request bytes
    for (int i = 0; i < sizeof(sensorZeroCalibrationFrame); i++)
    {
        renkeModbusSerial->write(sensorZeroCalibrationFrame[i]); // Send each byte of the frame
    }

    // Enabling Modbus read on the RS485 module to get oxygen data
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, LOW);
        digitalWrite(receiverEnable, LOW);
    }

    byte responseBytes[17];
    int responseByteCount = 0;
    while (renkeModbusSerial->available())
    {
        responseBytes[responseByteCount] = renkeModbusSerial->read();
        responseByteCount++;
    }

    return responseByteCount == 17;
}

bool RDOSensor::atmosphericPressureCalibration(float pressure)
{
    String sPressure = String(pressure);
    sPressure.replace(".", "");
    int iPressure = sPressure.toInt();

    byte byte1 = highByte(iPressure);
    byte byte2 = lowByte(iPressure);

    // Enabling Modbus write on the RS485 module to send an oxygen request frame.
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, HIGH);
        digitalWrite(receiverEnable, HIGH);
    }

    renkeModbusSerial->write(0x01);
    renkeModbusSerial->write(0x06);
    renkeModbusSerial->write(0x10);
    renkeModbusSerial->write(0x22);
    renkeModbusSerial->write(byte1);
    renkeModbusSerial->write(byte2);
    renkeModbusSerial->write(0x77);
    renkeModbusSerial->write(0x5e);

    // Enabling Modbus read on the RS485 module to get oxygen data
    if (driverHasEnablePin)
    {
        digitalWrite(driverEnable, LOW);
        digitalWrite(receiverEnable, LOW);
    }

    byte responseBytes[17];
    int responseByteCount = 0;
    while (renkeModbusSerial->available())
    {
        responseBytes[responseByteCount] = renkeModbusSerial->read();
        responseByteCount++;
    }

    return responseByteCount == 17;
}
