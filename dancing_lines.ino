// scrolltext demo for Adafruit RGBmatrixPanel library.  // Demonstrates double-buffered animation on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// Double-buffered mode consumes nearly all the RAM available on the
// Arduino Uno -- only a handful of free bytes remain.

#define NUM_DOTS 6
#define LINES_PER_SHAPE 3
const uint8_t NUM_SHAPES = NUM_DOTS / LINES_PER_SHAPE;

// Initial x, y, velocityX, velocityY
float dot[NUM_DOTS][8] = {
    { 3,  0,  1,  1},
    { 8,  2,  1, -1},
    {17, 15,  1, -1},
    { 3,  6, -1,  1},
    {27,  4, -1,  1},
    {17, 14, -1, -1}
};
long shapeHue[NUM_DOTS / LINES_PER_SHAPE] = {
    1535,
    535
};

void setup() {
    matrix.begin();
}

void loop() {
    // Clear background
    matrix.fillScreen(0);

    for (uint8_t i = 0; i < NUM_DOTS; i++) {
        // Get the color for the shape that this line is part of
        uint8_t shapeIndex = i / LINES_PER_SHAPE;
        uint16_t color = matrix.ColorHSV(shapeHue[shapeIndex], 255, 175, false);

        // Find index of the next dot
        byte nextIndex = i + 1;
        if (nextIndex == i - (i % LINES_PER_SHAPE) + LINES_PER_SHAPE) {
            nextIndex = i - (i % LINES_PER_SHAPE);
        }

        // Draw a line to the next dot
        matrix.drawLine(
            dot[i][0], dot[i][1],
            dot[nextIndex][0], dot[nextIndex][1],
            color);

        // Cycle through different hues
        shapeHue[shapeIndex] += 1;

        // Apply velocities
        dot[i][0] += dot[i][2];
        dot[i][1] += dot[i][3];

        // Bounce X off edges
        float newSpeed = random(4, 10) / 10.0;
        if (dot[i][0] <= 0) {
            dot[i][0] = 0;
            dot[i][2] = newSpeed;
        }
        if (dot[i][0] >= (matrix.width() - 1)) {
            dot[i][0] = (matrix.width() - 1);
            dot[i][2] = -newSpeed;
        }

        // Bounce Y off edges
        newSpeed = random(4, 10) / 10.0;
        if (dot[i][1] <= 0) {
            dot[i][1] = 0;
            dot[i][3] = newSpeed;
        }
        if (dot[i][1] >= (matrix.height() - 1)) {
            dot[i][1] = (matrix.height() - 1);
            dot[i][3] = -newSpeed;
        }
    }

    // Update display
    //matrix.swapBuffers(false);

    delay(100);
}
