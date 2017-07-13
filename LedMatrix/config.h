#pragma once

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "font7x5.h"

#define REVERSEFONT true
#define SCROLLDELAY 50 // ms

// Hardware config
#define MAX7219_CLK D2
#define MAX7219_CS D3
#define MAX7219_DIN D4
#define DIS_HEIGHT 8
#define DIS_WIDTH 8
#define DIS_SEGMENTS 4

#endif