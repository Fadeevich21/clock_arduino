#include "PCA9538DW.hpp"
#include <Wire.h>

PCA9538::PCA9538()
{
  Wire.begin();
}

void PCA9538::begin(uint8_t address) const
{
  Wire.beginTransmission(address);
}

void PCA9538::end() const
{
  Wire.endTransmission();
}

uint8_t PCA9538::inputPort(uint8_t address) const
{
  Wire.requestFrom(address, (uint8_t)1);
  
  uint8_t portData;
  while (Wire.available())
  {
    portData = Wire.read();
  }

  return portData;
}

void PCA9538::outputPort(uint8_t address, uint8_t data) const
{
  begin(address);
  Wire.write(Commands::OUTPUT_PORT_COMMAND);
  Wire.write(data);
  end();
}

void PCA9538::configuration(uint8_t address, uint8_t ioPort) const
{
  begin(Commands::CONFIGURATION_COMMAND);
  Wire.write(address);
  Wire.write(ioPort);
  end();
}

void PCA9538::polarityInversion(uint8_t address, uint8_t mask) const
{
  begin(address);
  Wire.write(Commands::POLARITY_INVERSION_COMMAND);
  Wire.write(mask);
  end();
}