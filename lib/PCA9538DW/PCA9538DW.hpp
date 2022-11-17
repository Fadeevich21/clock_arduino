#pragma once

#include <inttypes.h>

class PCA9538
{
private:
  enum Commands
  {
    INPUT_PORT_COMMAND = 0x00,
    OUTPUT_PORT_COMMAND = 0x01,
    POLARITY_INVERSION_COMMAND = 0x02,
    CONFIGURATION_COMMAND = 0x03
  };

private:
  void begin(uint8_t address) const;
  void end() const;

public:
  PCA9538();

  uint8_t inputPort(uint8_t address) const;
  void outputPort(uint8_t address, uint8_t data) const;

  void polarityInversion(uint8_t address, uint8_t mask) const;
  void configuration(uint8_t address, uint8_t ioPort) const;
};
