//   DS2438.h
/*
     DS2438T emperature And Voltage

     This example demonstrates the use of the DS2438 Library to read temperature and
     voltage from a Dallas Semiconductor DS2438 battery monitor using the Arduino
     OneWire library.

     DS2438 has 2 ADC channels - One general purpose
     second used for current accumulation

     Original library written by Joe Bechter
*/

#ifndef DS2438_h
#define DS2438_h

#include <Arduino.h>
#include <OneWire.h>

#define DS2438_TEMPERATURE_CONVERSION_COMMAND 0x44
#define DS2438_VOLTAGE_CONVERSION_COMMAND 0xb4
#define DS2438_WRITE_SCRATCHPAD_COMMAND 0x4e
#define DS2438_COPY_SCRATCHPAD_COMMAND 0x48
#define DS2438_READ_SCRATCHPAD_COMMAND 0xbe
#define DS2438_RECALL_MEMORY_COMMAND 0xb8
#define DS2438_PAGE_0 0x00

#define DS2438_CHA 0
#define DS2438_CHB 1

#define DS2438_MODE_CHA 0x01
#define DS2438_MODE_CHB 0x02
#define DS2438_MODE_TEMPERATURE 0x04

#define DS2438_TEMPERATURE_DELAY 10
#define DS2438_VOLTAGE_CONVERSION_DELAY 8

class DS2438 
{
    public:
        DS2438(OneWire *ow, uint8_t *address);
		void begin();
        void update();
        double getTemperature();
		float getVoltage(bool channel);
        boolean isError();
    private:
        OneWire *_ow;
        uint8_t *_address;
        double _temperature;
        boolean _error;
        float _dataA;
		float _dataB;
        void startConversion();
        void writePageZero(uint8_t *data);
        boolean readPageZero(uint8_t *data);
};

#endif
