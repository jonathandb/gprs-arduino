
#include "Arduino.h"

#define MAX_NUMBER_OF_MODBUS_SLAVES 60
#define MAX_NUMBER_OF_I2C_SLAVES 10
#define MAX_NUMBER_OF_TYPES 10
#define MAX_LENGTH_SERVER_ADDRESS 100
 

// Tell it where to store config data in EEPROM
#define CONFIG_START 32
#define CONFIG_VERSION "bla" //must be 3 characters!!!!!!!!!!


typedef struct ConfigurationParameters {
  char modbusSlaveAddresses[MAX_NUMBER_OF_MODBUS_SLAVES]; //1 char = 1 slave (slave address = 8 bit)
  char modbusSlaveTypes[MAX_NUMBER_OF_MODBUS_SLAVES]; //parallel to modbusSlaveAddresses
  char i2cSlaveAddresses[MAX_NUMBER_OF_I2C_SLAVES];
  char i2cSlaveTypes[MAX_NUMBER_OF_I2C_SLAVES]; //parallel to i2cSlaveAddresses
  char types[MAX_NUMBER_OF_TYPES];
  unsigned int timeToStartToCheckEachType[MAX_NUMBER_OF_TYPES]; //parallel to type  invalid value = max unsigned int
  unsigned int timeIntervalToCheckEachType[MAX_NUMBER_OF_TYPES]; //parallel to type  invalid value = max unsigned int
  char serverAddress[MAX_LENGTH_SERVER_ADDRESS];
  char programVersion[4];
};

class Configuration {
  private:
    void saveDefaultConfig();
  public:
    bool loadSuccess;
    ConfigurationParameters parameters;
    Configuration();
    void loadConfig();
    void saveConfig();
};
