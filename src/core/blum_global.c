
#ifndef BLUM_GLOBAL_C
#define BLUM_GLOBAL_C

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"



static void SerialSetup(){


    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);

     
}

static void SerialWrite(const char *src){
    uart_write_bytes(UART_NUM_0, src, strlen(src));
}

static void SerialWriteLn(const char* str) {
    size_t len = strlen(str);
    char buf[len + 2];
    strcpy(buf, str);
    buf[len] = '\n';
    buf[len + 1] = '\0';
    uart_write_bytes(UART_NUM_0, buf, len + 1);
}


#endif