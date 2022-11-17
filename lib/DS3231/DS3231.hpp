#pragma once

#include <stdint.h>
#include "time.hpp"

#define DS3231_ADDR 0x68
#define SECOND_ADDR 0x00
#define MINUTE_ADDR 0x01
#define HOUR_ADDR 0x02

#define ALARM1_SECOND_ADDR 0x07
#define ALARM1_MINUTE_ADDR 0x08
#define ALARM1_HOUR_ADDR 0x09
#define ALARM1_DAY_ADDR 0x0A

#define CONTROL_ADDR 0x0E

#define EOSC 7
#define BBSQW 6
#define CONV 5
#define RS1 3
#define RS2 4
#define INTCN 2
#define A1IE 0
#define A2IE 1
#define OSF 7
#define EN32 3
#define BSY 2
#define A1F 0
#define A2F 1
#define OFF 7


class DS3231_
{
public:
    DS3231_();

    Time_t get_time();
    uint8_t get_hour();    
    uint8_t get_minute();
    uint8_t get_second();

    void set_time(Time_t time);
    void set_time(uint8_t hour, uint8_t minute, uint8_t second);
    void set_hour(uint8_t hour);
    void set_minute(uint8_t minute);
    void set_second(uint8_t second);

    void set_alarm1(Time_t time, uint8_t mode);
    void set_alarm1(uint8_t hour, uint8_t minute, uint8_t second, uint8_t mode);

    void turn_off_alarm1();
    void turn_off_alarm2();
    void turn_on_alarm1();
    void turn_on_alarm2();
};