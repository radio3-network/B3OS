#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

// ESP32_2432S028R
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define TFT_PANEL_ORDER_BGR

#define HAS_BOARD
#define HAS_DISPLAY
#define HAS_TOUCH

// screen model and pins
#define ILI9431
#define ILI9431_SPI_SCLK 14
#define ILI9431_SPI_MOSI 13
#define ILI9431_SPI_MISO 12
#define ILI9341_PIN_CS 15
#define ILI9341_PIN_DC 2
#define ILI9341_SPI_FREQ 80000000
#define ILI9341_PIN_BL 21
#define ILI9341_PWM_CHANNEL_BL 12
#define ILI9341_PWM_FREQ_BL 5000
#define ILI9341_PWM_BITS_BL 8
#define ILI9341_PWM_MAX_BL ((1 << ILI9341_PWM_BITS_BL) - 1)

// touch screen model and pins
#define XPT2046
#define XPT2046_SPI_SCLK 25
#define XPT2046_SPI_MOSI 32
#define XPT2046_SPI_MISO 39
#define XPT2046_SPI_FREQ 2000000
#define XPT2046_PIN_INT 36
#define XPT2046_PIN_CS 33
// Calibration 240x320
#define XPT2046_MIN_X 349
#define XPT2046_MAX_X 3859
#define XPT2046_MIN_Y 247
#define XPT2046_MAX_Y 3871

// Build in RGB LED
#define LED_PIN_R 4
#define LED_PIN_G 16
#define LED_PIN_B 17
// PWM channels for RGB
#define LED_PWM_FREQ 5000
#define LED_PWM_CHANNEL_R 13
#define LED_PWM_CHANNEL_G 14
#define LED_PWM_CHANNEL_B 15
#define LED_PWM_BITS 8
#define LED_PWM_MAX ((1 << LED_PWM_BITS) - 1)

// Photo resistor
#define CDS_PIN 34 // ANALOG_PIN_0

// Audio out
#define AUDIO_PIN 26

// TF Card
#define TF_PIN_CS 5
#define TS_PIN_MOSI 23
#define TF_PIN_SCLK 18
#define TF_PIN_MISC 19


extern SPIClass spi_ili9431;
extern SPIClass spi_xpt2046;

static void setupHardwareInterfaces(){
    spi_ili9431.begin(ILI9431_SPI_SCLK, ILI9431_SPI_MISO, ILI9431_SPI_MOSI);
    spi_xpt2046.begin(XPT2046_SPI_SCLK, XPT2046_SPI_MISO, XPT2046_SPI_MOSI);
}

