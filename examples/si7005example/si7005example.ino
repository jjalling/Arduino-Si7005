/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
/*
 Si7005
 
 Library for communicationg with the Silabs Si7005 temperature/
 humidity sensor over I2C.

 created 9 Aug 2013
 by Jonas B. Jalling
*/

#include <Wire.h>
#include <Si7005.h>

// Pin connected to the Si7005 CS pin
// The other I2C pins is controlled by the Wire library
const int Si7005_CS_PIN = 8;

Si7005 Si(Si7005_CS_PIN);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  if (Si.detectSensor())
  {
    // The sensor is detected. Lets carry on!
    Serial.println("Device detected"); 
  }
  else
  {
    Serial.println("Device NOT detected");
    while (1)
    {;;} // The sensor is not detected. Let's just stay here forever.
  }
}

void loop()
{
  float temperature, humidity;

  // Uncomment line below to enable faster measurements (lower resolution)
  // Si.enableFastMeasurements();

  // Uncomment line below to enable the internal heater (affects the temperature measurement)
  // Si.enableHeater();

  // Do some measurements!
  temperature = Si.getTemperature();
  humidity = Si.getHumidity();

  // And output the data
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %RH");
  delay(1000);
}

