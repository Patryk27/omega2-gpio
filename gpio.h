#ifndef GPIO_H
#define GPIO_H

#include <cstdint>
#include <stdexcept>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

enum GpioPinDirection {
    GPD_INPUT = 0,
    GPD_OUTPUT = 1,
};

enum GpioPinValue {
    GPV_LOW = 0,
    GPV_HIGH = 1,
};

enum GpioBitOrder {
    GBO_LSB_FIRST,
    GBO_MSB_FIRST,
};

class Gpio {

private:
    static const int REG_BLOCK_ADDR = 0x10000000;

    static const int REGISTER_CTRL0_OFFSET = 384;
    static const int REGISTER_CTRL1_OFFSET = 385;
    static const int REGISTER_CTRL2_OFFSET = 386;

    static const int REGISTER_DATA0_OFFSET = 392;
    static const int REGISTER_DATA1_OFFSET = 393;
    static const int REGISTER_DATA2_OFFSET = 394;

    static const int REGISTER_DSET0_OFFSET = 396;
    static const int REGISTER_DSET1_OFFSET = 397;
    static const int REGISTER_DSET2_OFFSET = 398;

    static const int REGISTER_DCLR0_OFFSET = 400;
    static const int REGISTER_DCLR1_OFFSET = 401;
    static const int REGISTER_DCLR2_OFFSET = 402;

protected:
    static uint16_t controlOffset;
    static uint16_t dataOffset;
    static uint16_t dataSetOffset;
    static uint16_t dataClearOffset;

    static uint32_t *baseAddress;

    static void prepareOffsets(int pin);

    static void writeToRegister(uint16_t registerOffset, uint32_t value);

    static uint32_t readFromRegister(uint64_t registerOffset);

public:
    /**
     * Initializes the GPIO subsystem.
     */
    static void initialize();

    /**
     * Changes pin direction; available directions: GPV_INPUT, GPV_OUTPUT.
     * `direction` == 0 is considered to be GPV_INPUT, anything else is GPV_OUTPUT.
     */
    static void pinMode(uint8_t pin, uint8_t direction);

    /**
     * Changes pin value; available values: GPV_LOW, GPV_HIGH.
     * `value` == 0 is considered to be GPV_LOW, anything else is GPV_HIGH.
     */
    static void digitalWrite(uint8_t pin, uint8_t value);

    /**
     * Reads and returns pin value.
     * This function always returns 0 (meaning GPV_LOW) or non-zero (meaning GPV_HIGH).
     */
    static uint8_t digitalRead(uint8_t pin);

    /**
     * Shifts out byte using given data and clock pin.
     * See Arduino documentation for more specific description.
     */
    static void shiftOut(uint8_t dataPin, uint8_t clockPin, GpioBitOrder bitOrder, uint8_t value);

};

#endif //GPIO_H
