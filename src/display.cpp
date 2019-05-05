#include <display.h>
Display::Display(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7735(cs, dc, mosi, sclk, rst){
    
}
void Display::writeText(const char* text){
    fillScreen(ST77XX_BLACK);
    setCursor(0, 0);
    setTextColor(ST77XX_WHITE);
    setTextWrap(true);
    print(text);
}
void Display::writeText(const char* text, uint32_t y){
    setCursor(0, y);
    setTextColor(ST77XX_WHITE);
    setTextWrap(true);
    print(text);
}