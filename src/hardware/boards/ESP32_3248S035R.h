#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

// ESP32_3248S035R
#define TFT_WIDTH 320
#define TFT_HEIGHT 480
#define ST7796
#define ST7796_SPI_SCLK 14
#define ST7796_SPI_MOSI 13
#define ST7796_SPI_MISO 12
#define ST7796_PIN_CS 15
#define ST7796_PIN_DC 2
#define ST7796_SPI_FREQ 80000000
#define ST7796_PIN_BL 27
#define ST7796_PWM_CHANNEL_BL 12
#define ST7796_PWM_FREQ_BL 5000
#define ST7796_PWM_BITS_BL 8
#define ST7796_PWM_MAX_BL ((1 << ST7796_PWM_BITS_BL) - 1)
#define XPT2046
#define XPT2046_SPI_SCLK 14
#define XPT2046_SPI_MOSI 13
#define XPT2046_SPI_MISO 12
#define XPT2046_SPI_FREQ 2000000
#define XPT2046_PIN_INT 36
#define XPT2046_PIN_CS 33

// Calibration 320x480
#define XPT2046_MIN_X 256
#define XPT2046_MAX_X 3860
#define XPT2046_MIN_Y 180
#define XPT2046_MAX_Y 3900


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


static SPIClass spi_st7796;
#define spi_xpt2046 spi_st7796

static void setupHardwareInterfaces(){
  spi_st7796.begin(ST7796_SPI_SCLK, ST7796_SPI_MISO, ST7796_SPI_MOSI);
  // xpy2046 uses same SPI bus
}