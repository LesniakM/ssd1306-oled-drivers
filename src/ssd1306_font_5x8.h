#ifndef SSD1306_FONT_5x8_H
#define SSD1306_FONT_5x8_H


#include <stdint.h>
typedef uint8_t fontData;

#define fontWidth 5                  // Font width, padding excluded.
#define fontHeight 8                 // Font height, padding included.
extern fontData CharData[128][5];

#endif  // SSD1306_FONT_5x8_H