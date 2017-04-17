#include <iostream>
#include <unistd.h>

#include "../gpio.h"

const uint8_t LED_PIN = 0;

int main() {
    Gpio::initialize();

    // set the pin mode to output, so that we can digitalWrite() it
    Gpio::pinMode(LED_PIN, GPD_OUTPUT);

    // by the default our led is set to be on
    bool ledStatus = true;

    while (true) {
        // toggle the pin
        Gpio::digitalWrite(LED_PIN, ledStatus);

        // and wait 500 ms
        usleep(500 * 1000);

        ledStatus = !ledStatus;
    }
}