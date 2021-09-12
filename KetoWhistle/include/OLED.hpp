#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define led_pin 12

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern const unsigned char PROGMEM ANU_LOGO[];
extern const unsigned char PROGMEM DEVICE_LOGO[];

extern const unsigned char PROGMEM happy_face [128];
extern const unsigned char PROGMEM sad_face [128];
extern const unsigned char PROGMEM loading [28][128];


void Devicetext(Adafruit_SSD1306 *display, String text, int x, int y,int size, boolean d);
void load(Adafruit_SSD1306 *display, int T);

void ready_sig(Adafruit_SSD1306 *display, int T1);

void draw_anu(Adafruit_SSD1306 *display, int time);

void draw_logo(Adafruit_SSD1306 *display, int time);

void disp_setup(Adafruit_SSD1306 *display);

void disp_loop(Adafruit_SSD1306 *display);

void disp_setup(Adafruit_SSD1306 *display);