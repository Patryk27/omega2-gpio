#include <iostream>
#include <unistd.h>

#include "gpio.h"

const uint8_t DATA_PIN = 0;
const uint8_t LATCH_PIN = 1;
const uint8_t CLOCK_PIN = 2;
const uint8_t RESET_PIN = 3;

int main() {
    Gpio::initialize();

    // set the pin mode to output, so that we can digitalWrite() it
    Gpio::pinMode(DATA_PIN, GPD_OUTPUT);
    Gpio::pinMode(LATCH_PIN, GPD_OUTPUT);
    Gpio::pinMode(CLOCK_PIN, GPD_OUTPUT);
    Gpio::pinMode(RESET_PIN, GPD_OUTPUT);
	Gpio::digitalWrite(RESET_PIN, GPV_HIGH);
	
    while (true)
    {
		for (int byteWrite = 0; byteWrite < 256; byteWrite++)
		{
			Gpio::digitalWrite(LATCH_PIN, GPV_LOW);
			Gpio::shiftOut(DATA_PIN, CLOCK_PIN, GBO_LSB_FIRST, byteWrite);
			Gpio::digitalWrite(LATCH_PIN, GPV_HIGH);
			usleep(500 * 1000);
		}
		Gpio::digitalWrite(RESET_PIN, GPV_LOW);
		Gpio::digitalWrite(RESET_PIN, GPV_HIGH);
    }
}
