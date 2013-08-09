/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
 
#ifndef SI7005_H
#define SI7005_H

//#include <inttypes.h>

#include "Arduino.h"


/* Si7005 Registers */
#define REG_STATUS         0x00
#define REG_DATA           0x01
#define REG_CONFIG         0x03
#define REG_ID             0x11

/* Status Register */
#define STATUS_NOT_READY   0x01

/* Config Register */
#define CONFIG_START       0x01
#define CONFIG_HEAT        0x02
#define CONFIG_HUMIDITY    0x00
#define CONFIG_TEMPERATURE 0x10
#define CONFIG_FAST        0x20

/* ID Register */
#define ID_SAMPLE          0xF0
#define ID_SI7005          0x50

/* Coefficients */
#define TEMPERATURE_OFFSET   50
#define TEMPERATURE_SLOPE    32
#define HUMIDITY_OFFSET      24
#define HUMIDITY_SLOPE       16
#define a0 (-4.7844)
#define a1  0.4008
#define a2 (-0.00393)
#define q0  0.1973
#define q1  0.00237

#define WAKE_UP_TIME 15
#define SI7005_ADR 0x40

class Si7005
{
 public:
  Si7005( int pin );
  bool  detectSensor( );
  float getTemperature( );
  float getHumidity( );
  void  enableHeater( );
  void  disableHeater( );
  void  enableFastMeasurements( );
  void  disableFastMeasurements( );
  
 private:
  int _cs_pin;
  int _config_reg;
  float _last_temperature;
  float _last_humidity;
  int _doMeasurement( byte );

};
#endif  // SI7005_H