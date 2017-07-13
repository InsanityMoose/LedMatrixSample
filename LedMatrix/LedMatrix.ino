/*
 Name:		LedMatrix.ino
 Created:	7/13/2017 2:55:37 PM
 Author:	Martin
*/

// the setup function runs once when you press reset or power the board
#include "config.h"
#include "max7219.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

char disbuffer[DIS_SEGMENTS][DIS_HEIGHT];

void initBuffer(char _buffer[DIS_SEGMENTS][DIS_HEIGHT], int sizex, int sizey)
{
	for (int x = 0; x < sizex; x++)
	{
		for (int y = 0; y < sizey; y++)
		{
			_buffer[x][y] = 0;
		}
	}
}

char ReverseBits(char input)
{
	char output = input;
	for (int i = sizeof(input) * 8 - 1; i; --i)
	{
		output <<= 1;
		input >>= 1;
		output |= input & 1;
	}
	return output;
}

void printChar(char c)
{
	byte col = 0;
	for (int y = 0; y < fontH; y++)
	{
		col = font[c][y];
		toOne(2, fontH - y, col);
	}
}

// Print single character to buffer at absolute position x, y
void printChar(char _buffer[DIS_SEGMENTS][DIS_WIDTH], char c, int x, int y)
{
	char bitmap;

	for (int _y = 0; _y < fontH; _y++)
	{
		if (REVERSEFONT)
			bitmap = ReverseBits(font[c][_y]) >> 8 - fontW;
		else
			bitmap = font[c][_y];

		int ypos = _y - y;

		if (ypos >= 0 && ypos < DIS_HEIGHT)
		{
			// First segment
			if (x / DIS_WIDTH >= 0 && x / DIS_WIDTH < DIS_SEGMENTS)
			{
				// Wipe buffer area
				_buffer[x / DIS_WIDTH][ypos] = _buffer[x / DIS_WIDTH][ypos] & ~(0b11111111 << (x % DIS_WIDTH));
				// Print char to buffer
				_buffer[x / DIS_WIDTH][ypos] = _buffer[x / DIS_WIDTH][ypos] | (bitmap << (x % DIS_WIDTH));
			}
			// Second segment
			if ((x + DIS_WIDTH) / DIS_WIDTH >= 0 && (x + DIS_WIDTH) / DIS_WIDTH < DIS_SEGMENTS)
			{
				// Wipe buffer area
				_buffer[(x + DIS_WIDTH) / DIS_WIDTH][ypos] = _buffer[(x + DIS_WIDTH) / DIS_WIDTH][ypos] & ~(0b11111111 >> (DIS_WIDTH - ((x + DIS_WIDTH) % DIS_WIDTH)));
				// Print char to buffer
				_buffer[(x + DIS_WIDTH) / DIS_WIDTH][ypos] = _buffer[(x + DIS_WIDTH) / DIS_WIDTH][ypos] | (bitmap >> (DIS_WIDTH - ((x + DIS_WIDTH) % DIS_WIDTH)));
			}
		}
	}
}

// Print string to buffer
void printString(char _buffer[DIS_SEGMENTS][DIS_WIDTH], const char *s, int x, int y)
{
	int len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		printChar(_buffer, s[i], x + i*(fontW + 1), y);
	}
}


void updateDisplay(char _buffer[DIS_SEGMENTS][DIS_HEIGHT])
{
	for (int x = 0; x < DIS_SEGMENTS; x++)
	{
		for (int y = 0; y < DIS_HEIGHT; y++)
		{
			toOne(x + 1, fontH - y + 1, _buffer[x][y]);
		}
	}

}

void pushUp(char _buffer[DIS_SEGMENTS][DIS_HEIGHT])
{
	for (int segment = 0; segment < DIS_SEGMENTS; segment++)
	{
		for (int y = 0; y < DIS_HEIGHT; y++)
		{
			_buffer[segment][y] = _buffer[segment][y + 1];
		}
		_buffer[segment][DIS_HEIGHT - 1] = 0;
	}
}

void blankLine(char _buffer[DIS_SEGMENTS][DIS_HEIGHT], int y)
{
	for (int segment = 0; segment < DIS_SEGMENTS; segment++)
	{
		_buffer[segment][y] = 0;
	}
}

void scrollHorizontalLeft(const char *text)
{
	for (int i = 0; i < 8 * 4 + (fontW + 1) * strlen(text); i++) {
		printString(disbuffer, text, DIS_SEGMENTS*DIS_WIDTH - i, 0);
		updateDisplay(disbuffer);
		delay(SCROLLDELAY);
	}
}

void scrollVerticalUp(const char *text)
{
	for (int i = -DIS_HEIGHT; i <= 0; i++)
	{
		pushUp(disbuffer);
		printString(disbuffer, text, 0, i);
		updateDisplay(disbuffer);
		delay(SCROLLDELAY);
	}
}

void setup() {
	Serial.begin(115200);

	pinMode(MAX7219_CS, OUTPUT);
	pinMode(MAX7219_CLK, OUTPUT);
	pinMode(MAX7219_DIN, OUTPUT);

	toAll(max7219_reg_scanLimit, 0x07);
	toAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	toAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	toAll(max7219_reg_displayTest, 0x00); // no display test
	for (int e = 1; e <= 8; e++) {    // empty registers, turn all LEDs off
		toAll(e, 0);
	}
	//toAll(max7219_reg_intensity, 0x0f & 0x0f);
	toAll(max7219_reg_intensity, 0x01 & 0x0f);
	initBuffer(disbuffer, DIS_SEGMENTS, DIS_HEIGHT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//scrollHorizontalLeft("HELLO WORLD!");
	scrollVerticalUp("HELLO!");
	delay(SCROLLDELAY * 5);
	scrollVerticalUp("EVERYONE");
	delay(SCROLLDELAY * 5);
	scrollVerticalUp("AROUND");
	delay(SCROLLDELAY * 5);
	scrollVerticalUp("THE");
	delay(SCROLLDELAY * 5);
	scrollVerticalUp("WORLD!");
	delay(SCROLLDELAY * 5);
	scrollVerticalUp(" ");
	delay(SCROLLDELAY * 5);
}
