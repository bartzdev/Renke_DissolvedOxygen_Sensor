
# RenkeDissolvedOxygenSensor

This is an Arduino/ESP32 library for interfacing with the Renke Fluorescence Dissolved Oxygen Sensor and similar sensors that use the same communication protocol.

## Supported Models

- RS-LDO-N01-EX
- VMS-3001-LDO/LDOS-N01-2-20
- VMS-3002-LDO-N01-20

##
This library uses a serial interface to communicate with the sensor. Since these sensors use the RS485 protocol, an RS485 to TTL converter is required and can be wired as follows:




<img src="https://github.com/bartzdev/Renke_DissolvedOxygen_Sensor/blob/main/examples/doSensorTest/wiring.png">