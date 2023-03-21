#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

/*
  NodeMCU V3 is basically an ESP2866 standalone board without screen.

  Keep in attention:

  + Change platformio.ini file to include the following:

[env:nodemcu3]
platform = espressif8266
board = nodemcu3
framework = arduino

  + Adjust the settings for your external screen.
  On this config board we used a 2.8 TFT SPI 240x320 V1.2 display.
  
  The display is an ILI9341 LCD model with XPT2046 touch screen.
  Purchase link: https://www.aliexpress.com/item/32795636902.html


TFT Screen CS (Chip Select) pin to NodeMCU D2 (GPIO4)
TFT Screen DC (Data/Command) pin to NodeMCU D1 (GPIO5)
TFT Screen MOSI (Master Out Slave In) pin to NodeMCU D7 (GPIO13)
TFT Screen SCK (Serial Clock) pin to NodeMCU D5 (GPIO14)
TFT Screen MISO (Master In Slave Out) pin to NodeMCU D6 (GPIO12)
TFT Screen RESET pin to NodeMCU D0 (GPIO16)

Touch Screen CS (Chip Select) pin to NodeMCU D3 (GPIO0)
Touch Screen IRQ (Interrupt) pin to NodeMCU D4 (GPIO2)
Touch Screen MOSI (Master Out Slave In) pin to NodeMCU D7 (GPIO13)
Touch Screen SCK (Serial Clock) pin to NodeMCU D5 (GPIO14)
Touch Screen MISO (Master In Slave Out) pin to NodeMCU D6 (GPIO12)
Touch Screen GND to NodeMCU GND
Touch Screen VCC to NodeMCU 3.3V (or an external power supply)


*/


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
#define ILI9341_PIN_CS 04
#define ILI9341_PIN_DC 05
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
#define LED_PIN_R 18
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

#define TF_CARD
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

