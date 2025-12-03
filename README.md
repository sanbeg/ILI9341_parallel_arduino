# ILI9341_parallel
ILI9341 parallel communication lib for Arduino.

This code provides basic functions to control ILI9341 TFT touch screen using parallel interface (8 data lines, 5 control lines).

# Usage
You can add files `ILI9341.h` and `ILI9341.cpp` to project directory.

.ino file contains a simple example of usage.

# TODO

## 1st milestone
* Implement sprites drawing and scaling
* Implement basic text writing

## 2nd milestone
* Implement RLE decoding for high resolution fonts or sprites
* Implement touch screen capabilities

# Timed tests

With some added instrumentation, we can time how long it takes to run the demo and fill the 
screen with stripes, in both the Arduino & Micropython ports on different boards using both
generic (arduino or micropython) methods to write to the 8 parallel data pins, or bypassing the
platform abstractions and using a platform-specific method which allows writing multiple pins at 
once.

| platform | board | emitter | seconds|
| ---|---|---|---|
| arduino | uno | avr | 1.91 |
| arduino | uno | arduino | 7.95 |
| micropython | g474re | stm | 6.32 |
| micropython | g474re | mpy | 14.77 |
| arduino | g474re | arduino | 0.43 |
