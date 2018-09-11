#ifndef HARDWARE_TIMER_H
#define HARDWARE_TIMER_H
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"
#include "softwareTimer.h"

TIME update_time_now();
void start_hardware_timer(TIME unTime);

#endif
