#include "eepromDrv.h"



uint8_t readEEPROM( unsigned int CV ) {
    return EEPROM.read(CV) ;
}

void writeEEPROM( unsigned int CV, uint8_t Value ) {
    EEPROM.write(CV, Value) ;
  #if defined(ESP8266)
    EEPROM.commit();
  #endif
}

uint8_t wCV( unsigned int CV, uint8_t Value)
{
    writeEEPROM( CV, Value ) ;
}

uint8_t readCV( unsigned int CV )
{
  uint8_t Value ;

  Value = readEEPROM(CV);
  return Value ;
}
