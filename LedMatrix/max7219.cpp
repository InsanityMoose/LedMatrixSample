// 
// 
// 

#include "max7219.h"
#include "config.h"

// send one byte to max7219
void putByte(byte data)
{
	byte i = 8;
	byte mask;
	while (i > 0) {
		mask = 0x01 << (i - 1);      // get bitmask
		digitalWrite(MAX7219_CLK, LOW);   // tick
		if (data & mask) {            // choose bit
			digitalWrite(MAX7219_DIN, HIGH);// send 1
		}
		else {
			digitalWrite(MAX7219_DIN, LOW); // send 0
		}
		digitalWrite(MAX7219_CLK, HIGH);   // tock
		--i;                         // move to lesser bit
	}
}

// addressing different MAX7219's in a cascade
void toOne(byte maxNr, byte reg, byte col) {
	int c = 0;
	digitalWrite(MAX7219_CS, LOW);  // begin    

	for (c = DIS_SEGMENTS; c > maxNr; c--) {
		putByte(0);    // means no operation
		putByte(0);    // means no operation
	}

	putByte(reg);  // specify register
	putByte(col);//((data & 0x01) * 256) + data >> 1); // put data

	for (c = maxNr - 1; c >= 1; c--) {
		putByte(0);    // means no operation
		putByte(0);    // means no operation
	}

	digitalWrite(MAX7219_CS, LOW); // and load da stuff
	digitalWrite(MAX7219_CS, HIGH);
}

// send same data to all max7219
void toAll(byte reg, byte col)
{
	int c = 0;
	digitalWrite(MAX7219_CS, LOW);  // begin    
	for (c = 1; c <= DIS_SEGMENTS; c++) {
		putByte(reg);  // specify register
		putByte(col);  //((data & 0x01) * 256) + data >> 1); // put data
	}
	digitalWrite(MAX7219_CS, LOW);
	digitalWrite(MAX7219_CS, HIGH);
}

