/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
#include "Arduino.h"

#include "Si7005.h"
#include <Wire.h>

Si7005::Si7005( int pin )
{
  pinMode( pin, OUTPUT );
  digitalWrite( pin, HIGH );
  _cs_pin = pin;
  _last_temperature = 25.0;
  _config_reg = 0;
}

bool Si7005::detectSensor( )
{
  byte deviceID;

  digitalWrite( _cs_pin, LOW );								// Enable the sensor
  delay( WAKE_UP_TIME );									// Wait for it to wake up

  Wire.beginTransmission( SI7005_ADR );
  Wire.write( REG_ID );										// Select the ID register
  Wire.endTransmission( false );							// We don't want to release the bus - this is important!

  Wire.requestFrom( SI7005_ADR, 1 );
  deviceID  = Wire.read( );									// Read the ID from the sensor

  digitalWrite( _cs_pin, HIGH );							// Disable the sensor
  
  if ( ( deviceID & ID_SAMPLE ) == ID_SI7005 )
  {
	return true;
  }
  else
  {
    return false;
  }
}

int Si7005::_doMeasurement( byte configValue )
{
  int rawData;
  byte measurementStatus;
  
  digitalWrite( _cs_pin, LOW );								// Enable the sensor
  delay( WAKE_UP_TIME );									// Wait for it to wake up

  Wire.beginTransmission( SI7005_ADR );
  Wire.write(REG_CONFIG);									// Select the CONFIG register
  Wire.write( CONFIG_START | configValue | _config_reg );	// Start measurement of the selected type (Temperature / humidity)
  Wire.endTransmission( );

  measurementStatus = STATUS_NOT_READY;						// Wait for the measurement to finish
  while ( measurementStatus & STATUS_NOT_READY )
  {
    Wire.beginTransmission( SI7005_ADR );
    Wire.write( REG_STATUS );
    Wire.endTransmission( false );							// We don't want to release the bus - this is important!
    Wire.requestFrom( SI7005_ADR, 1 );
    measurementStatus = Wire.read( );
  }

  Wire.beginTransmission( SI7005_ADR );
  Wire.write( REG_DATA );									// Select the DATA register
  Wire.endTransmission( false );

  Wire.requestFrom( SI7005_ADR, 2 );						// Read 2 bytes from the sensor
  while ( Wire.available( ) < 2 )							// Wait for data
  {;;}
  
  rawData  = ( Wire.read() << 8 );							// MSB
  rawData |= Wire.read( );									// LSB
  
  digitalWrite( _cs_pin, HIGH );							// Disable the sensor

  return rawData;
}

float Si7005::getTemperature( )
{
  float rawTemperature;

  rawTemperature = _doMeasurement( CONFIG_TEMPERATURE ) >> 2;
  _last_temperature = ( rawTemperature / TEMPERATURE_SLOPE ) - TEMPERATURE_OFFSET;

  return _last_temperature;
}

float Si7005::getHumidity( )
{
  float rawHumidity, curve, linearHumidity;

  rawHumidity = _doMeasurement( CONFIG_HUMIDITY ) >> 4;
  curve = ( rawHumidity / HUMIDITY_SLOPE ) - HUMIDITY_OFFSET;
  linearHumidity = curve - ( (curve * curve) * a2 + curve * a1 + a0);
  linearHumidity = linearHumidity + ( _last_temperature - 30 ) * ( linearHumidity * q1 + q0 );

  return linearHumidity;
}

void Si7005::enableHeater( )
{
  _config_reg |= CONFIG_HEAT;
}

void Si7005::disableHeater( )
{
	_config_reg ^= CONFIG_HEAT;
}

void Si7005::enableFastMeasurements( )
{
  _config_reg |= CONFIG_FAST;
}

void Si7005::disableFastMeasurements( )
{
	_config_reg ^= CONFIG_FAST;
}