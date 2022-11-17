#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdint.h>
#include <Servo.h>
#include <Adafruit_BMP280.h>
#include "DS3231.hpp"
#include "PCA9538DW.hpp"
#include "time.hpp"


#define LCD_I2C_ADR 0x20
// #define LCD_I2C_ADR 0x3F

#define POTENCIOMETERS_MIDDLE_VALUE 512
#define JOY_LEFT 600
#define JOY_RIGHT 400
#define JOY_UP 600
#define JOY_DOWN 400
#define JOY_CLICK 0

#define POTENCIOMETERS_PIN A0
#define JOY_Y_PIN A1
#define JOY_X_PIN A2
#define JOY_SEL_PIN A3
#define SERVO_PIN 5
#define DS3231_PIN_INTERRUPT 2
#define SPEAKER_PIN 10


LiquidCrystal_I2C lcd(LCD_I2C_ADR, 16, 2);
Servo servo;
PCA9538 pca9538;
DS3231_ ds3231;

volatile bool alarm = false;


void wait_setting(const char* str_command)
{
    lcd.clear();
    lcd.home();
    lcd.print(str_command);
    lcd.print(" setup time");
    lcd.setCursor(1, 1);
    lcd.print("Finger off btn");
    while (digitalRead(JOY_SEL_PIN) == JOY_CLICK)
    {
        delay(300);
    }
    lcd.clear();
}

void wait_start_setting()
{
    wait_setting("Start");
}

void wait_stop_setting()
{
    wait_setting("Stop");
}


void print_time(Time_t time)
{
    char* time_str = time.get_time_str();
    lcd.setCursor(4, 0);
    lcd.print(time_str);
    delete time_str;    
}


#define SELECT_SECOND 0
#define SELECT_MINUTE 1
#define SELECT_HOUR 2

void change_time(Time_t &time, int select)
{
    if (analogRead(JOY_Y_PIN) > JOY_UP)
        {
            switch (select)
            {
                case SELECT_SECOND:
                    time.inc_second();
                    break;

                case SELECT_MINUTE:
                    time.inc_minute();
                    break;

                case SELECT_HOUR:
                    time.inc_hour();
                    break;
            }
        }
        else if (analogRead(JOY_Y_PIN) < JOY_DOWN)
        {
            switch (select)
            {
                case SELECT_SECOND:
                    time.dec_second();
                    break;

                case SELECT_MINUTE:
                    time.dec_minute();
                    break;

                case SELECT_HOUR:
                    time.dec_hour();
                    break;
            }
        }
}

void change_select(uint8_t &select)
{
    if (analogRead(JOY_X_PIN) > JOY_LEFT)
    {
        select = select < 2 ? select + 1 : select;
    }
    else if (analogRead(JOY_X_PIN) < JOY_RIGHT)
    {
        select = select > 0 ? select - 1 : select;
    }
}


void print_unit_time(uint8_t select, const char* str)
{
    lcd.setCursor(1, 1);
    lcd.print(str);

    char ch;
    switch (select)
    {
        case SELECT_SECOND:
            ch = 's';
            break;

        case SELECT_MINUTE:
            ch = 'm';
            break;

        case SELECT_HOUR:
            ch = 'h';
            break;
    }
    lcd.write(ch);
}

void print_unit_time_set_time(uint8_t select)
{
    print_unit_time(select, "Setting time ");
}

void print_unit_time_set_alarm(uint8_t select)
{
    print_unit_time(select, "Setting alarm ");
}


void setting_time()
{
    wait_start_setting();

    uint8_t select = 0;
    Time_t time = ds3231.get_time();
    do
    {
        change_time(time, select);
        change_select(select);

        print_time(time);
        print_unit_time_set_time(select);

        delay(300);
    }
    while (digitalRead(JOY_SEL_PIN) != JOY_CLICK);

    ds3231.set_time(time);
    wait_stop_setting();
}

void setting_alarm()
{
    wait_start_setting();

    uint8_t select = 0;
    Time_t time = ds3231.get_time();
    do
    {
        change_time(time, select);
        change_select(select);

        print_time(time);
        print_unit_time_set_alarm(select);

        delay(300);
    }
    while (digitalRead(JOY_SEL_PIN) != JOY_CLICK);

    ds3231.set_alarm1(time, 0x08);

    wait_stop_setting();
}


void isr_TickTock();

void setup()
{
    ds3231.turn_off_alarm1();
    ds3231.turn_off_alarm2();

    // ---TEST---
    ds3231.set_alarm1(0, 10, 0, 0x08);
    // ----------

    pca9538.configuration(0x70, 0x00);
    pca9538.configuration(0x73, 0x00);
    pca9538.outputPort(0x70, 0x03);
    pca9538.outputPort(0x73, 0xFF);

    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(POTENCIOMETERS_PIN, INPUT);
    pinMode(JOY_X_PIN, INPUT);
    pinMode(JOY_Y_PIN, INPUT);
    pinMode(JOY_SEL_PIN, INPUT_PULLUP);
    servo.attach(SERVO_PIN);

    lcd.init();
    lcd.backlight();

    ds3231.turn_on_alarm1();
}

void loop()
{
    static bool first_tick_alarm = true;
    if (alarm)
    {
        if (first_tick_alarm)
        {
            first_tick_alarm = false;
            digitalWrite(SPEAKER_PIN, HIGH);
            pca9538.outputPort(0x70, 0b01000111);
        }
        pca9538.polarityInversion(0x70, 0b01101100);
    }
    lcd.clear();

    int potenciometers_value = analogRead(POTENCIOMETERS_PIN);

    int servo_angle = potenciometers_value <= POTENCIOMETERS_MIDDLE_VALUE ? 0 : 90;
    servo.write(servo_angle);
    if (digitalRead(JOY_SEL_PIN) == JOY_CLICK)
    {
        if (potenciometers_value <= POTENCIOMETERS_MIDDLE_VALUE)
        {
            setting_time();
        }
        else
        {
            setting_alarm();
        }
    }

    Time_t time = ds3231.get_time();
    print_time(time);

    delay(100);
    // time.inc_second();
}


void isr_TickTock()
{
    alarm = true;
}

// 
// Пункт отправки 
// Зависает в Wire
// экноледж
// i2c
// Выводить в терминал