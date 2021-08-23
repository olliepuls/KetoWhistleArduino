#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define led_pin 12

double count_T=0;
double count=1.21;
int16_t h;
int16_t i;
int16_t k;
int16_t j;
int16_t m;

// ANU Logo Bitmap
static const unsigned char PROGMEM ANU_LOGO[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0xC0,0x1F,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0xFF,0xFE,0x00,0x00,0xFE,0x3F,0xE0,0x1F,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x22,0x00,0x00,0x3F,0x07,0xE0,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x8A,0x00,0x00,0x3F,0x03,0xF0,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x8A,0x00,0x00,0x3F,0x03,0xF0,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x22,0x00,0x00,0x3F,0x83,0xF8,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x62,0x00,0x00,0x77,0x83,0xF8,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x82,0x02,0x00,0x00,0x77,0x83,0xBC,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0x8F,0xFA,0x00,0x00,0x77,0xC3,0xBE,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0xB9,0x4A,0x00,0x00,0xE3,0xC3,0x9E,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0xAF,0xEE,0x00,0x00,0xE3,0xE3,0x9F,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x00,0xFC,0x64,0x00,0x00,0xC3,0xE3,0x8F,0x1C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x08,0xF7,0xDC,0x20,0x01,0xC1,0xE3,0x87,0x9C,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x08,0x5C,0x74,0x60,0x01,0xFF,0xF3,0x87,0xDC,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x0A,0x63,0x9C,0x60,0x03,0xFF,0xF3,0x83,0xDC,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x0E,0x3C,0x79,0xC0,0x03,0xFF,0xF3,0x83,0xFC,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x1F,0x0B,0xA3,0xE0,0x03,0x80,0x7B,0x81,0xFC,0xF8,0x07,0x00,0x00,0x00,
    0x00,0x00,0x3F,0xC6,0xCF,0xF0,0x07,0x00,0x7B,0x81,0xFC,0xF8,0x0F,0x00,0x00,0x00,
    0x00,0x00,0x3F,0xF8,0x3F,0xF0,0x07,0x00,0x7B,0x80,0xFC,0x78,0x0E,0x00,0x00,0x00,
    0x00,0x00,0x13,0xFF,0xFF,0x90,0x07,0x00,0x3F,0x80,0x7C,0x7C,0x1E,0x00,0x00,0x00,
    0x00,0x00,0x02,0x3F,0xF8,0x00,0x0E,0x00,0x3F,0x80,0x7C,0x3F,0xFC,0x00,0x00,0x00,
    0x00,0x00,0x00,0x0F,0xE0,0x00,0x0E,0x00,0x3F,0x80,0x3C,0x1F,0xF8,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// Device Logo Bitmap
static const unsigned char PROGMEM DEVICE_LOGO[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0xFE,0x00,0x00,0x00,0x0E,0x00,0x79,0x80,0x0C,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0xEF,0x00,0x00,0x01,0xCE,0x00,0x7B,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0xEF,0x00,0x00,0x03,0xEE,0x00,0x7A,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0xEE,0x7E,0xF9,0xFF,0xEF,0xE0,0x7E,0x0F,0xBE,0xFC,0xFE,0x7C,0x00,0x00,
    0x00,0x01,0xFC,0x7F,0xCD,0x9D,0xCF,0xE0,0x7F,0x1D,0x9D,0xCC,0xEE,0xEE,0x00,0x00,
    0x00,0x01,0xEE,0x71,0xDC,0x7D,0xCE,0xE0,0x7F,0x1D,0xDD,0xCE,0xEE,0xEE,0x00,0x00,
    0x00,0x01,0xE7,0x71,0xFD,0xDD,0xCE,0xE0,0x77,0x9F,0x9D,0xCE,0xEE,0xFE,0x00,0x00,
    0x00,0x01,0xE7,0x71,0xC1,0xDD,0xCE,0xE0,0x73,0x9C,0x1D,0xCE,0xEE,0xE0,0x00,0x00,
    0x00,0x01,0xEF,0x70,0xED,0xDD,0xCE,0xE0,0x7B,0xDC,0x9C,0xCC,0xEE,0x76,0x00,0x00,
    0x00,0x01,0xFE,0x70,0x79,0xFE,0xEE,0xE0,0x79,0xCF,0x8E,0x78,0xEE,0x3C,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x8C,0xC3,0xCF,0xE3,0xF8,0xC0,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x8F,0xEC,0xEF,0xE6,0x79,0xF5,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x09,0xCE,0xEC,0xEE,0xE4,0x73,0xB7,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x09,0xCE,0xE3,0xEE,0x7C,0x73,0xB7,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x1F,0xCE,0xEE,0xEE,0x78,0xE3,0xE7,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x1F,0xEE,0xEE,0xEE,0x79,0xE3,0x87,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x10,0xEE,0xEF,0xEE,0x39,0xC3,0x97,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x10,0xEC,0xE7,0x7E,0x31,0xF8,0xE7,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00

};

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Emoticons
const unsigned char PROGMEM happy_face [128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x7C, 0x1E, 0x00, 0x01, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x01, 0xC0, 0x07, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x04, 0x20, 0x70, 0x0C, 0x04, 0x60, 0x78, 0x1E, 0x06, 0x60, 0x78, 0x1E, 0x06, 0x60, 0x30, 0x0E, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x21, 0x80, 0x01, 0x84, 0x30, 0xC0, 0x03, 0x0C, 0x30, 0xE0, 0x07, 0x0C, 0x18, 0x38, 0x1C, 0x18, 0x18, 0x1F, 0xF8, 0x18, 0x0C, 0x03, 0xE0, 0x30, 0x06, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0xC0, 0x01, 0xC0, 0x03, 0x80, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char PROGMEM sad_face [128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x7C, 0x1E, 0x00, 0x01, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x01, 0xC0, 0x07, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x04, 0x20, 0x70, 0x0C, 0x04, 0x60, 0x78, 0x1E, 0x06, 0x60, 0x78, 0x1E, 0x06, 0x60, 0x30, 0x0E, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x20, 0x02, 0x80, 0x04, 0x30, 0x3F, 0xFC, 0x0C, 0x30, 0x74, 0x1E, 0x0C, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x0C, 0x00, 0x00, 0x30, 0x06, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0xC0, 0x01, 0xC0, 0x03, 0x80, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00};

// Loading animation
const unsigned char PROGMEM loading [28][128] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x03, 0xC0, 0xF0, 0x1F, 0x87, 0xE1, 0xF8, 0x31, 0x8C, 0x31, 0x0C, 0x20, 0xCC, 0x33, 0x04, 0x30, 0xCC, 0x33, 0x0C, 0x39, 0x86, 0x61, 0x9C, 0x1F, 0x07, 0xE0, 0xF8, 0x04, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x0E, 0x0E, 0x60, 0xF0, 0x1F, 0x8C, 0x31, 0xF8, 0x31, 0x88, 0x31, 0x0C, 0x30, 0xCC, 0x33, 0x04, 0x30, 0xC6, 0xE3, 0x0C, 0x31, 0x83, 0xC1, 0x9C, 0x1F, 0x80, 0x00, 0xF8, 0x06, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x0C, 0x60, 0x00, 0x00, 0x08, 0x20, 0xF0, 0x07, 0x18, 0x21, 0xF8, 0x1F, 0x8C, 0x61, 0x0C, 0x18, 0xCE, 0xE3, 0x04, 0x30, 0x41, 0x03, 0x0C, 0x30, 0x40, 0x01, 0x9C, 0x19, 0xC0, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x20, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x18, 0x40, 0x00, 0x00, 0x10, 0x60, 0x00, 0x00, 0x18, 0x60, 0xF0, 0x00, 0x18, 0x61, 0xF8, 0x00, 0x0F, 0xC1, 0x0C, 0x07, 0x87, 0x03, 0x04, 0x18, 0x40, 0x03, 0x0C, 0x18, 0x60, 0x01, 0x9C, 0x18, 0x60, 0x00, 0xF8, 0x18, 0x60, 0x00, 0x20, 0x1C, 0xC0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0xF0, 0x00, 0x1F, 0x81, 0xF8, 0x00, 0x0F, 0x01, 0x8C, 0x00, 0x00, 0x03, 0x04, 0x03, 0xC0, 0x03, 0x0C, 0x0F, 0x60, 0x01, 0x9C, 0x0C, 0x20, 0x00, 0xF8, 0x08, 0x30, 0x00, 0x20, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x33, 0x00, 0xF0, 0x00, 0x3F, 0x01, 0xF8, 0x00, 0x04, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x01, 0xE0, 0x01, 0x9C, 0x07, 0x70, 0x00, 0xF8, 0x06, 0x10, 0x00, 0x20, 0x04, 0x18, 0x00, 0x00, 0x06, 0x18, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x66, 0x00, 0xF0, 0x00, 0x3C, 0x01, 0xF8, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x00, 0xE0, 0x01, 0x9C, 0x01, 0xF8, 0x00, 0xF8, 0x03, 0x18, 0x00, 0x20, 0x03, 0x0C, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x01, 0xB8, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x01, 0x84, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0xDC, 0x00, 0xF0, 0x00, 0x78, 0x01, 0xF8, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x70, 0x01, 0x9C, 0x00, 0xFC, 0x00, 0xF8, 0x01, 0x8C, 0x00, 0x20, 0x01, 0x86, 0x00, 0x00, 0x01, 0x06, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0xCC, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x01, 0xB8, 0x00, 0xF0, 0x01, 0xF0, 0x01, 0xF8, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x38, 0x01, 0x9C, 0x00, 0x7E, 0x00, 0xF8, 0x00, 0xC3, 0x00, 0x20, 0x00, 0xC3, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x07, 0x30, 0x00, 0x00, 0x06, 0x18, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x06, 0x18, 0x00, 0x00, 0x06, 0x30, 0x00, 0xF0, 0x03, 0xF0, 0x01, 0xF8, 0x00, 0xC0, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x1E, 0x01, 0x9C, 0x00, 0x37, 0x00, 0xF8, 0x00, 0x61, 0x80, 0x20, 0x00, 0x61, 0x80, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x0C, 0x60, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0xF0, 0x06, 0xE0, 0x01, 0xF8, 0x07, 0xC0, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x06, 0x03, 0x0C, 0x00, 0x1F, 0x81, 0x9C, 0x00, 0x31, 0x80, 0xF8, 0x00, 0x30, 0xC0, 0x20, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x19, 0x80, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0xF0, 0x18, 0x60, 0x01, 0xF8, 0x0F, 0xC0, 0x01, 0x8C, 0x07, 0x80, 0x03, 0x04, 0x00, 0x0F, 0xC3, 0x0C, 0x00, 0x18, 0xC1, 0x9C, 0x00, 0x18, 0x60, 0xF8, 0x00, 0x10, 0x60, 0x20, 0x00, 0x18, 0x60, 0x00, 0x00, 0x1D, 0xC0, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x18, 0xC0, 0x00, 0x00, 0x10, 0xC0, 0x00, 0xF0, 0x30, 0x40, 0x01, 0xF8, 0x30, 0xC3, 0x81, 0x0C, 0x18, 0xC7, 0xC3, 0x04, 0x07, 0x18, 0x63, 0x0C, 0x00, 0x08, 0x21, 0x9C, 0x00, 0x18, 0x20, 0xF8, 0x00, 0x0C, 0x60, 0x20, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0xF0, 0x30, 0xC3, 0xC1, 0xF8, 0x20, 0xCE, 0xE1, 0x0C, 0x30, 0xCC, 0x33, 0x04, 0x19, 0x88, 0x33, 0x0C, 0x1F, 0x8C, 0x31, 0x9C, 0x04, 0x06, 0xE0, 0xF8, 0x00, 0x07, 0xC0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x03, 0xC0, 0xF0, 0x1F, 0x87, 0xE1, 0xF8, 0x31, 0x8C, 0x31, 0x0C, 0x20, 0xCC, 0x33, 0x04, 0x30, 0xCC, 0x33, 0x0C, 0x39, 0x86, 0x61, 0x9C, 0x1F, 0x07, 0xE0, 0xF8, 0x04, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x0F, 0x07, 0xE0, 0x70, 0x1F, 0x86, 0x31, 0xF8, 0x31, 0x8C, 0x33, 0x0C, 0x20, 0xCC, 0x33, 0x0C, 0x30, 0xC6, 0x33, 0x0C, 0x39, 0x87, 0xE1, 0x8C, 0x1F, 0x01, 0xC1, 0xF8, 0x04, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0x06, 0x30, 0x00, 0x1F, 0x84, 0x18, 0xE0, 0x31, 0x84, 0x13, 0xF8, 0x20, 0xC6, 0x1B, 0x18, 0x30, 0xC3, 0xF2, 0x08, 0x39, 0x81, 0x82, 0x0C, 0x1F, 0x00, 0x03, 0x98, 0x04, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00, 0x06, 0x18, 0x00, 0x0F, 0x02, 0x08, 0x00, 0x1F, 0x86, 0x08, 0x00, 0x30, 0x83, 0x18, 0x00, 0x20, 0xC1, 0xF3, 0xE0, 0x30, 0xC0, 0x06, 0x10, 0x31, 0x80, 0x04, 0x18, 0x1F, 0x00, 0x06, 0x18, 0x04, 0x00, 0x06, 0x18, 0x00, 0x00, 0x03, 0x70, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x00, 0x01, 0x8C, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x0F, 0x03, 0x0C, 0x00, 0x1F, 0x81, 0x9C, 0x00, 0x31, 0x80, 0xF8, 0x00, 0x20, 0xC0, 0x20, 0x00, 0x30, 0xC0, 0x07, 0xC0, 0x39, 0x80, 0x06, 0x60, 0x1F, 0x00, 0x0C, 0x30, 0x04, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x70, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00, 0x01, 0x86, 0x00, 0x0E, 0x00, 0xC6, 0x00, 0x1F, 0x80, 0x7C, 0x00, 0x31, 0x80, 0x28, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x39, 0x80, 0x07, 0x80, 0x1F, 0x00, 0x1C, 0xC0, 0x04, 0x00, 0x18, 0x60, 0x00, 0x00, 0x10, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x40, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xC1, 0x80, 0x00, 0x00, 0x41, 0x00, 0x0F, 0x00, 0x77, 0x00, 0x1F, 0x80, 0x3E, 0x00, 0x31, 0x80, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x39, 0x80, 0x0E, 0x00, 0x1F, 0x00, 0x1F, 0x80, 0x04, 0x00, 0x31, 0x80, 0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x3B, 0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x31, 0x80, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0x80, 0x0E, 0x00, 0x3B, 0x80, 0x1F, 0x80, 0x0F, 0x00, 0x31, 0x80, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x31, 0x80, 0x18, 0x00, 0x1F, 0x00, 0x3E, 0x00, 0x04, 0x00, 0x63, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x80, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x18, 0xC0, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x60, 0x0F, 0x00, 0x0C, 0xC0, 0x1F, 0x80, 0x0F, 0x80, 0x31, 0x80, 0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x39, 0x80, 0x38, 0x00, 0x1F, 0x00, 0xFC, 0x00, 0x04, 0x00, 0xC6, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0xEE, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x06, 0xE0, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0C, 0x30, 0x0F, 0x00, 0x0C, 0x30, 0x1F, 0x80, 0x07, 0xE0, 0x31, 0x80, 0x03, 0xC0, 0x20, 0xC0, 0x00, 0x00, 0x30, 0xC0, 0x20, 0x00, 0x39, 0x80, 0xF8, 0x00, 0x1F, 0x01, 0x8C, 0x00, 0x04, 0x01, 0x0C, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x01, 0x8C, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x06, 0x18, 0x0F, 0x00, 0x04, 0x18, 0x1F, 0x80, 0x06, 0x18, 0x31, 0x80, 0x03, 0x70, 0x20, 0xC0, 0x03, 0xE0, 0x30, 0xC3, 0xF0, 0x00, 0x39, 0x83, 0x18, 0x00, 0x1F, 0x06, 0x08, 0x00, 0x04, 0x06, 0x08, 0x00, 0x00, 0x02, 0x18, 0x00, 0x00, 0x03, 0xB8, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x01, 0xF8, 0x0E, 0x00, 0x03, 0x18, 0x1F, 0x80, 0x02, 0x08, 0x31, 0x83, 0xE2, 0x0C, 0x20, 0xC7, 0x73, 0x08, 0x30, 0xC4, 0x19, 0xF0, 0x39, 0x84, 0x18, 0xE0, 0x1F, 0x06, 0x10, 0x00, 0x04, 0x07, 0x70, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x0F, 0x01, 0x81, 0xF8, 0x1F, 0x87, 0xE1, 0x98, 0x31, 0x86, 0x33, 0x0C, 0x20, 0xCC, 0x33, 0x0C, 0x30, 0xCC, 0x33, 0x0C, 0x39, 0x86, 0x31, 0xF8, 0x1F, 0x07, 0xE0, 0xF0, 0x04, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x03, 0xC0, 0xF0, 0x1F, 0x87, 0xE1, 0xF8, 0x31, 0x8C, 0x31, 0x0C, 0x20, 0xCC, 0x33, 0x04, 0x30, 0xCC, 0x33, 0x0C, 0x39, 0x86, 0x61, 0x9C, 0x1F, 0x07, 0xE1, 0xF8, 0x04, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

// Display text
void Devicetext(String text, int x, int y,int size, boolean d) {
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
}

// Loading animation function
void load(int T){
  int xx=48;
  int yy=0;
  int tt=0;
  for (j=0; j<T; j+=1){
    for (int i = 0; i < 28; i++) {
      display.clearDisplay();
      display.drawBitmap(xx, yy,loading[i],32,32, 1);
      display.display();
      delay(tt);
    }
    delay(1000);
  } 
}

// Resdy 
void ready_sig(int T1){
  display.clearDisplay();
  Devicetext("Ready!", 10, 10, 3, false);
  display.display();
  delay(T1); 
  
} 

void draw_anu(int time) {
  display.drawBitmap(0, 0, ANU_LOGO, 128, 32, 1);
  display.display();
  delay(time);
  display.clearDisplay();
}

void draw_logo(int time) {
  display.drawBitmap(0, 0, DEVICE_LOGO, 128, 32, 1);
  display.display();
  delay(time);
  display.clearDisplay();
}

// EXAMPLE USAGE!!

// void setup() {
//   Serial.begin(115200);
//   pinMode(led_pin,OUTPUT);
//   digitalWrite(led_pin,LOW);

//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//     Serial.println(F("SSD1306 allocation failed"));
//     for(;;);
//   }
//   delay(2000);
//   display.clearDisplay();
  
  
// }

// void loop() {
//   draw_anu(5000);
//   draw_logo(5000);

//   for(i=0; i<13; i+=1){
//     String TString =  String(count_T,0);
    
//     display.clearDisplay();
//     Devicetext("Heating!", 20, 0, 2, false);
//     Devicetext("Temp:   ", 30, 25, 1, false);
//     Devicetext(TString, 65, 25, 1, false);
//     Devicetext("C", 90, 25, 1, false);
//     display.display();
    
//     if(count_T < 349)
//       count_T +=32;
//     else 
//       count_T = count_T-1;  
    
//     delay(1000);
//   }

//   display.clearDisplay();
//   digitalWrite(led_pin,LOW);
//   ready_sig(3000);

//   load(6);

//   display.clearDisplay();
//   Devicetext("Invaild ! ", 10, 10, 2, false);
//   display.display();

//   ready_sig(6);
//   load(6);

//   for(h=0; h<7; h+=1){
      
//     String vString =  String(count, 2);
//     display.clearDisplay();
//     Devicetext("Acetone:        ", 20, 3, 1, false);
//     Devicetext(vString, 30, 15, 2, false);
//     Devicetext("ppm", 95, 20, 1, false);
//     display.display();
//     count +=0.11;
//     delay(2000);  
//   }
//   delay(5000);
//   display.clearDisplay();

//   for(m=0; m<10; m+=1){
    
//     display.clearDisplay();
//     count=1.87;
//     String vString =  String(count, 2);
//     Devicetext("BAD! ", 25, 1, 2, false);
//     Devicetext(vString, 25, 25, 1, false);
//     Devicetext("ppm", 55, 25, 1, false);
//     display.drawBitmap(90, 0, sad_face, 32,32, 1);
//     display.display();
//     delay(0);
//     delay(2000);  
//   }

//   display.clearDisplay();
//   display.display();
// }

