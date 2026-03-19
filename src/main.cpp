#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "setup/helpers.h"
#include "setup/I2C.h"
#include "setup/ssd1306.h"
#include "setup/macros.h"
#include "setup/spi.h"
#include "setup/uart.h"
#include <string.h>

int main(void) {

    I2C_Init();
    InitializeDisplay();
    clear_display();
    interrupt_init();
    uart_init();
    sei();
    int hh, mm, ss;

    uart_print("Skriv tid som hh:mm:ss\r\n");

    while (!receiveFlag);  // wait for ISR to complete the string
    receiveFlag = 0;
    sscanf((const char *)rxBuf, "%d:%d:%d", &hh, &mm, &ss);

    while (1) {
        char time_str[9];
        sprintf(time_str, "%02d:%02d:%02d", hh, mm, ss);
       // clear_display();
        print_centered(time_str, 3);

        char msg[40];
        sprintf(msg, "Aktuelt klokkeslaet er %s\r\n", time_str);
        uart_print(msg);

        _delay_ms(1000);

        ss++;
        if (ss == 60) { 
          ss = 0; mm++; 
        }
        if (mm == 60) { 
          mm = 0; hh++;
        }
        if (hh == 24) { 
          hh = 0;
        }
    }

}

