#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

public:
    void inc_second();
    void inc_minute();
    void inc_hour();

    void dec_second();
    void dec_minute();
    void dec_hour();

    void set_time(uint8_t _hour, uint8_t _minute, uint8_t _second);
    char* get_time_str();
}
Time_t;
