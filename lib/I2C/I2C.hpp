#pragma once

#include <stdint.h>

#define BUFF_SIZE 128

#define R 1
#define W 0

class I2C
{   
private:
    bool _is_processing;

    uint8_t _addr_receiver;
    uint8_t _buff[BUFF_SIZE];
    uint8_t _buff_index;

    void send_pack(uint8_t pack);
    uint8_t get_status();
    void error();

public:
    I2C();

    void begin_transmission(uint8_t addr);
    void end_transmission();

    void write(uint8_t data);
    void write(uint8_t* buff, uint8_t buff_size);
    uint8_t read();
    void request_from(uint8_t addr, uint8_t quantity);
};