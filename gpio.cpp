#include "gpio.h"

uint16_t Gpio::controlOffset;
uint16_t Gpio::dataOffset;
uint16_t Gpio::dataSetOffset;
uint16_t Gpio::dataClearOffset;

uint32_t *Gpio::baseAddress;

void Gpio::writeToRegister(uint16_t registerOffset, uint32_t value) {
    *(Gpio::baseAddress + registerOffset) = value;
}

uint32_t Gpio::readFromRegister(uint64_t registerOffset) {
    return *(Gpio::baseAddress + registerOffset);
}

void Gpio::prepareOffsets(int pin) {
    switch (pin / 32) {
        case 0:
            Gpio::controlOffset = Gpio::REGISTER_CTRL0_OFFSET;
            Gpio::dataOffset = Gpio::REGISTER_DATA0_OFFSET;
            Gpio::dataSetOffset = Gpio::REGISTER_DSET0_OFFSET;
            Gpio::dataClearOffset = Gpio::REGISTER_DCLR0_OFFSET;
            break;

        case 1:
            Gpio::controlOffset = Gpio::REGISTER_CTRL1_OFFSET;
            Gpio::dataOffset = Gpio::REGISTER_DATA1_OFFSET;
            Gpio::dataSetOffset = Gpio::REGISTER_DSET1_OFFSET;
            Gpio::dataClearOffset = Gpio::REGISTER_DCLR1_OFFSET;
            break;

        default:
            Gpio::controlOffset = Gpio::REGISTER_CTRL2_OFFSET;
            Gpio::dataOffset = Gpio::REGISTER_DATA2_OFFSET;
            Gpio::dataSetOffset = Gpio::REGISTER_DSET2_OFFSET;
            Gpio::dataClearOffset = Gpio::REGISTER_DCLR2_OFFSET;
    }
}

void Gpio::initialize() {
    int memHandle;

    if ((memHandle = open("/dev/mem", O_RDWR)) < 0) {
        throw std::runtime_error("(GPIO) Opening /dev/mem failed.");
    }

    Gpio::baseAddress = (uint32_t *) mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, memHandle,
                                          Gpio::REG_BLOCK_ADDR);

    close(memHandle);

    if (Gpio::baseAddress == MAP_FAILED) {
        throw std::runtime_error("(GPIO) mmap() failed.");
    }
}

void Gpio::pinMode(uint8_t pin, uint8_t direction) {
    Gpio::prepareOffsets(pin);
    pin %= 32;

    uint32_t controlRegister = Gpio::readFromRegister(Gpio::controlOffset);

    switch (direction) {
        case GPD_INPUT:
            controlRegister &= ~(1 << (uint64_t) pin);
            break;

        default:
            controlRegister |= 1 << (uint64_t) pin;
            break;
    }

    Gpio::writeToRegister(Gpio::controlOffset, controlRegister);
}

void Gpio::digitalWrite(uint8_t pin, uint8_t value) {
    Gpio::prepareOffsets(pin);
    pin %= 32;

    uint16_t registerOffset;

    if (value) {
        registerOffset = Gpio::dataSetOffset;
    } else {
        registerOffset = Gpio::dataClearOffset;
    }

    uint32_t registerValue = ((uint32_t) 1 << pin);

    Gpio::writeToRegister(registerOffset, registerValue);
}

uint8_t Gpio::digitalRead(uint8_t pin) {
    Gpio::prepareOffsets(pin);
    pin %= 32;

    uint32_t registerValue = Gpio::readFromRegister(Gpio::dataOffset);

    return (uint8_t) ((registerValue & ((uint32_t) 1 << pin)) > 0);
}

void Gpio::shiftOut(uint8_t dataPin, uint8_t clockPin, GpioBitOrder bitOrder, uint8_t value) {
    for (uint8_t i = 0; i < 8; i++) {
        switch (bitOrder) {
            case GBO_LSB_FIRST:
                Gpio::digitalWrite(dataPin, (value & (1 << i)) != 0);
                break;

            case GBO_MSB_FIRST:
                Gpio::digitalWrite(dataPin, (value & (1 << (7 - i))) != 0);
                break;
        }

        Gpio::digitalWrite(clockPin, GPV_HIGH);
        Gpio::digitalWrite(clockPin, GPV_LOW);
    }
}
