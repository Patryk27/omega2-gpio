# Omega2-GPIO

A library for simple Omega 2 GPIO management.

## What is this?

You can switch Omega 2 registers with the `fast-gpio` or `gpiomux` tool - but what if you wanted to do this not inside some `bash` script, but inside your own application?

Normally you would be condemned to use horrible things like `system("fast-gpio blah blah");` - but it would be slow as duck *(if we talk about something more than just switching led on and off)*.

It bothered me too - and so I've created my own small GPIO management library, basing on the official `fast-gpio` project from the Omega team
(https://github.com/OnionIoT/fast-gpio).

## How do I use this?

It's quite simple - just download the `gpio.cpp` and `gpio.h`, and include them in your code.

They export the `Gpio` class with following methods:

```cpp
static void Gpio::initialize();

static void Gpio::pinMode(uint8_t pin, uint8_t direction);

static void Gpio::digitalWrite(uint8_t pin, uint8_t value);

static uint8_t Gpio::digitalRead(uint8_t pin);

static void Gpio::shiftOut(uint8_t dataPin, uint8_t clockPin, GpioBitOrder bitOrder, uint8_t value);
```

(you can find their descriptions inside the `gpio.h`: https://github.com/Patryk27/Omega2-GPIO/blob/master/gpio.h)

## Can you show me any example, please?

Here ya go, pal: https://github.com/Patryk27/Omega2-GPIO/tree/master/examples

## Why such a weird nomenclature?

I've based it on the Arduino standard library because most people familiar with electronics programming *(including me)* probably know that platform and there are many, many source codes on the web which use functions named like these - so it, theoretically, should be easier for us *(the Omega programmers)* to translate them from eg. Arduino/Raspberry to Omega, not having to think about "what and why the how is this command called here".

Although I **have** slightly modified it:

1.`GPD_INPUT`/`GPD_OUTPUT` instead of pure Arudino's `INPUT`/`OUTPUT`.

2.`GPV_LOW`/`GPV_HIGH` instead of just `LOW`/`HIGH`.

3.I've wrapped the functions in a class.

I did this because I don't feel like polluting the namespace is a convention worth following - and it does not hurt that much. You can always do `#define INPUT GPD_INPUT` or `#define digitalWrite Gpio::digitalWrite` if you feel like it.

## Why your library lacks `analogRead`/`analogWrite`?

Omega 2 does not have any ADC (`analog/digital converter`) on the chip and so it is not possible to create any generic `analogRead`/`analogWrite` function.

If you want to use this, you'd have to buy some ADC and have fun on your own.

## How do I compile this?

I am lazy and thus I didn't want to mess with any virtual machines just to compile a few small files - so I use my own Omega 2 to do the compiling.

And if you want too, you just have to:

1.Uncomment the LEDE repositories in `/etc/opkg/distfeeds.conf` (that is: `reboot_core`, `reboot_base`, `reboot_luci` and `reboot_packages`).

2.Do the `opkg update && opkg install gcc` (it will take a few minutes).

3.If everything went correctly, you should now be able to run `gcc`/`g++` from the console (you can check it right now).

4.Assuming your `gcc` works - you can now without any problems compile applications on your Omega 2. Example command line can be:
```
g++ -std=c++11 gpio.cpp led-blink.cpp
```
It should take about 20 seconds and result in an `a.out` file (in the directory where you ran the command), which can be executed.

Disclaimer: I have tested it on my `pivot-root`ed Omega 2+ - I do not know if `gcc` works on the standard version (with the 64 MB of RAM) nor do I know if your Omega 2(+) without `pivot-*` will have sufficient storage space to install `gcc`.

## Is your code tested?

I have succesfully written and operated a PCD8544 and PS2 mouse drivers *(so far)* using my methods so they should be pretty good. Although everything is eventually up to you - if you set two GPIOs to output and connect them, don't blame me if your Omega never wakes up.
