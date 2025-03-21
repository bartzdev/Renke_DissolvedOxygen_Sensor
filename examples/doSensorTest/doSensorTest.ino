#include <SoftwareSerial.h>
#include <RenkeDissolvedOxygenSensor.h>

#define DI_PIN 17  //DRIVER INPUT (MODULE RX PIN)
#define DE_RE_PIN 4  //DRIVER/RECEIVER ENABLE INPUT/OUTPUT
#define RO_PIN 16  //RECEIVER OUTPUT (MODULE TX PIN)

SoftwareSerial modbusSerial(RO_PIN, DI_PIN);  //rx, tx
RDOSensor doSensor(&modbusSerial, DE_RE_PIN);

void setup()
{
  Serial.begin(9600);
  modbusSerial.begin(4800);
}

void loop()
{
  if (doSensor.readSensorData())
  {
    Serial.println("--------------------------------------");
    float oxCons = doSensor.getOxygenConcentration();
    Serial.println("Oxygen concentration: " + String(oxCons));

    float oxSat = doSensor.getOxygenSaturation();
    Serial.println("Oxygen saturation: " + String(oxSat));

    float temp = doSensor.getTemperature();
    Serial.println("Temperature: " + String(temp));    
  }else{
    Serial.println("An error occurred while reading from the sensor.");
  }

  delay(1000);
}