#include "DS3231.hpp"
#include <Arduino.h>
#include <Wire.h>
// #include "I2C.hpp"


// I2C i2c;


void write8(uint8_t addr, uint8_t reg, uint8_t data);

DS3231_::DS3231_()
{
    // uint8_t control = _BV(INTCN);
    // write8(DS3231_ADDR, CONTROL_ADDR, control);
}

uint8_t dec_to_bcd(uint8_t val)
{
	return ((val / 10 * 16) + (val % 10));
}

uint8_t bcd_to_dec(uint8_t val)
{
	return ((val / 16 * 10) + (val % 16));
}


void read(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t buffer_len)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, buffer_len);
    for (uint8_t i = 0; i < buffer_len; i++)
    {
        buffer[i] = Wire.read();
    }
}

uint8_t read8(uint8_t addr, uint8_t reg)
{
    uint8_t buffer[1];
    read(addr, reg, buffer, 1);

    return buffer[0];
}


Time_t DS3231_::get_time()
{
    Time_t time;
    time.hour = get_hour();
    time.minute = get_minute();
    time.second = get_second();

    return time;
}

uint8_t DS3231_::get_hour()
{
    uint8_t hour = read8(DS3231_ADDR, HOUR_ADDR);
    return bcd_to_dec(hour);
}

uint8_t DS3231_::get_minute()
{
    uint8_t minute = read8(DS3231_ADDR, MINUTE_ADDR);
    return bcd_to_dec(minute);
}

uint8_t DS3231_::get_second()
{
    uint8_t second = read8(DS3231_ADDR, SECOND_ADDR);
    return bcd_to_dec(second);
}


Time_t DS3231_::get_alarm1()
{
    Time_t time;
    time.hour = bcd_to_dec(read8(DS3231_ADDR, ALARM1_HOUR_ADDR));
    time.minute = bcd_to_dec(read8(DS3231_ADDR, ALARM1_MINUTE_ADDR));
    time.second = bcd_to_dec(read8(DS3231_ADDR, ALARM1_SECOND_ADDR));

    return time;
}


void write(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t buffer_len)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(buffer, buffer_len);
    Wire.endTransmission();
}

void write8(uint8_t addr, uint8_t reg, uint8_t data)
{
    uint8_t buffer[1];
    buffer[0] = data;
    write(addr, reg, buffer, 1);
}


void DS3231_::set_time(Time_t time)
{
    set_time(time.hour, time.minute, time.second);
}

void DS3231_::set_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    set_hour(hour);
    set_minute(minute);
    set_second(second);
}

void DS3231_::set_hour(uint8_t hour)
{
    write8(DS3231_ADDR, HOUR_ADDR, dec_to_bcd(hour));
}

void DS3231_::set_minute(uint8_t minute)
{
    write8(DS3231_ADDR, MINUTE_ADDR, dec_to_bcd(minute));
}

void DS3231_::set_second(uint8_t second)
{
    write8(DS3231_ADDR, SECOND_ADDR, dec_to_bcd(second));

    uint8_t control = read8(DS3231_ADDR, CONTROL_ADDR) & (~_BV(EOSC));
    write8(DS3231_ADDR, CONTROL_ADDR, control);
}


void DS3231_::set_alarm1(Time_t time, uint8_t mode)
{
    set_alarm1(time.hour, time.minute, time.second, mode);
}

void DS3231_::set_alarm1(uint8_t hour, uint8_t minute, uint8_t second, uint8_t mode)
{
    uint8_t day = dec_to_bcd(read8(DS3231_ADDR, ALARM1_DAY_ADDR));
    if (0x08 & mode)
    {
        day |= _BV(OFF);
    }
    else
    {
        day &= ~_BV(OFF);
    }
    write8(DS3231_ADDR, ALARM1_DAY_ADDR, day);

    hour = dec_to_bcd(hour);
    if (0x04 & mode)
    {
        hour |= _BV(OFF);
    }
    else
    {
        hour &= ~_BV(OFF);
    }
    write8(DS3231_ADDR, ALARM1_HOUR_ADDR, hour);

    minute = dec_to_bcd(minute);
    if (0x02 & mode)
    {
        minute |= _BV(OFF);
    }
    else
    {
        minute &= ~_BV(OFF);
    }
    write8(DS3231_ADDR, ALARM1_MINUTE_ADDR, minute);

    second = dec_to_bcd(second);
    if (0x01 & mode)
    {
        second |= _BV(OFF);
    }
    else
    {
        second &= ~_BV(OFF);
    }
    write8(DS3231_ADDR, ALARM1_SECOND_ADDR, second);
}


void DS3231_::turn_off_alarm1()
{
    uint8_t control = read8(DS3231_ADDR, CONTROL_ADDR) & (~_BV(A1IE));
    write8(DS3231_ADDR, CONTROL_ADDR, control);
}

void DS3231_::turn_off_alarm2()
{
    uint8_t control = read8(DS3231_ADDR, CONTROL_ADDR) & (~_BV(A2IE));
    write8(DS3231_ADDR, CONTROL_ADDR, control);
}

void DS3231_::turn_on_alarm1()
{
    uint8_t control = read8(DS3231_ADDR, CONTROL_ADDR) | _BV(A1IE);
    write8(DS3231_ADDR, CONTROL_ADDR, control);
}

void DS3231_::turn_on_alarm2()
{
    uint8_t control = read8(DS3231_ADDR, CONTROL_ADDR) | _BV(A2IE);
    write8(DS3231_ADDR, CONTROL_ADDR, control);
}