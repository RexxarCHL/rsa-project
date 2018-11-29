#ifndef _TASC_H_
#define _TASC_H_

#include <Arduino.h>  // Arduino library
#include <math.h>     // Additional math for RTC script
#include <Wire.h>     // I2C library for the light sensor
#include <Adafruit_Sensor.h>  // For light sensor
#include "Adafruit_TSL2591.h" // For light sensor
#include "RTClib.h"           // For RTC

// RTC circuit
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
RTC_PCF8523 rtc;

void setup_rtc();
DateTime get_current_time();
void adjust_rtc_time(int year, int month, int day, int hour, int minute, int second);
bool is_night(DateTime now);
float degrees_to_radians(float degrees);

// Servo control
const int MOTOR_PIN1 = 6;
const int MOTOR_PIN2 = 5;
const int MOTOR_ENABLE = 11;
const int MOTOR_ENCODER_A = 2;
const int MOTOR_ENCODER_B = 3;
volatile int servo_position = 0;

// 4 counts per cycle, 12 cycles per revolution
// 34 revolutions per output shaft revolution
const int EVENT_COUNT_PER_REV = 4*12*4.4;
const int DELTA_T = 10;
const float K_prop = 3.0;
const float K_D = 0.5;

void setup_servo();
void move_servo(int blind_pos, bool relative=false);

// Occupancy sensor
const int PIR_ENABLE = 3;
const int PIR_PIN = 2;

void setup_pir_sensor();
bool check_pir_sensor();

// Light sensor
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
const int LIGHT_SENSOR_ENABLE = 8; // connect the pin to the 5v in for the sensor?

void setup_light_sensor();
void configure_sensor();
int get_light_reading();

// Various control functions and variables
enum SystemState {
    INIT,
    MANUAL,
    AUTO
} state;

void adjust_blind_angle();
void move_blind_angle(int offset);
void check_button(); // TODO
void check_battery_level(); // TODO

// It takes about 4 turns from open to fully closed
// current_blind_pos = [0, 64] and there are 16 positions in each turn 
const int TURNS_TO_CLOSE = 4;
const int MAX_ANGLE_VALUE = 64;
const int ONE_TURN = MAX_ANGLE_VALUE / TURNS_TO_CLOSE;
const int HALF_TURN = ONE_TURN / 2;
const int MAX_DECREASING_COUNT = 2;
int current_blind_pos = 0;

#endif