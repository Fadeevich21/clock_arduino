#include "time.hpp"

void Time_t::inc_second()
{
    this->second++;
    if (this->second == 60)
    {
        this->second = 0;
        inc_minute();
    }
}

void Time_t::inc_minute()
{
    this->minute++;
    if (this->minute == 60)
    {
        this->minute = 0;
        inc_hour();
    }
}

void Time_t::inc_hour()
{
    this->hour++;
    if (this->hour == 24)
    {
        this->hour = 0;
    }
}


void Time_t::dec_second()
{
    if (this->second > 0)
    {
        this->second--;
        return;
    }

    this->second = 59;
    dec_minute();
}

void Time_t::dec_minute()
{
    if (this->minute> 0)
    {
        this->minute--;
        return;
    }

    this->minute = 59;
    dec_hour();
}

void Time_t::dec_hour()
{
    if (this->hour > 0)
    {
        this->hour--;
        return;
    }

    this->hour = 23;
}


void Time_t::set_time(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
    this->hour = _hour;
    this->minute = _minute;
    this->second = _second;
}

char* Time_t::get_time_str()
{
    char* time_str = new char[9];

    time_str[0] = this->hour / 10 + '0';
    time_str[1] = this->hour % 10 + '0';

    time_str[2] = ':';
    
    time_str[3] = this->minute / 10 + '0';
    time_str[4] = this->minute % 10 + '0';

    time_str[5] = ':';

    time_str[6] = this->second / 10 + '0';
    time_str[7] = this->second % 10 + '0';

    time_str[8] = '\0';

    return time_str;
}