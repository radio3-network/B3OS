#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include <Wire.h>


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
#define GT911
#define GT911_IIC_SDA 33
#define GT911_IIC_SCL 32
#define GT911_IIC_RST 25

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
static TwoWire i2c_gt911 = TwoWire(1); // Bus number 1


static void setupHardwareInterfaces(){
  spi_st7796.begin(ST7796_SPI_SCLK, ST7796_SPI_MISO, ST7796_SPI_MOSI);
  i2c_gt911.begin(GT911_IIC_SDA, GT911_IIC_SCL);
}