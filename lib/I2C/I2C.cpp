#include "I2C.hpp"
#include <avr/io.h>
#include <Wire.h>


I2C::I2C()
{
    this->_buff_index = 0;
}


uint8_t I2C::get_status()
{
    uint8_t status = TWSR & 0xF8;
    return status;
}

void I2C::error()
{

}


void I2C::begin_transmission(uint8_t addr)
{
    this->_addr_receiver = addr;
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);
    while (!(TWCR & _BV(TWINT)));
}

void I2C::end_transmission()
{
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}


void I2C::send_pack(uint8_t pack)
{
    TWDR = pack;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)));
}


void I2C::write(uint8_t data)
{
    Wire.write(data);
    // uint8_t sla_w = (this->_addr_receiver << 1) | W;
    // send_pack(sla_w);
    // if (get_status() != 0x18)
    // {
    //     error();
    // }

    // send_pack(data);
    // if (get_status() != 0x28)
    // {
    //     error();
    // }
}

void I2C::write(uint8_t* buff, uint8_t buff_size)
{
    for (uint8_t i = 0; i < buff_size; i++)
    {
        write(buff[i]);
    } 
}


uint8_t I2C::read()
{
    if (this->_buff_index == 0)
    {
        return 0;
    }

    return this->_buff[this->_buff_index--];
}

void I2C::request_from(uint8_t addr, uint8_t quantity)
{
    begin_transmission(addr);
    write(this->_addr_receiver);
    for (uint8_t i = 0; i < quantity; i++)
    {
        begin_transmission(addr);
        uint8_t sla_r = (this->_addr_receiver << 1) | R;
        send_pack(sla_r);
        if (get_status() != 0x18)
        {
            error();
            break;
        }

        this->_buff[this->_buff_index++] = TWCR;
    }
    end_transmission();
}