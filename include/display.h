#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
class Display: public Adafruit_ST7735{
    private:
    public:
        int Buffer[15][128];
        Display(int8_t, int8_t, int8_t, int8_t, int8_t);
        void displayTime();
        void writeText(const char*);
        void writeText(const char*, uint32_t );
};