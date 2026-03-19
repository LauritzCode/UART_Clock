#include "uart.h"
#include "macros.h"
#include "helpers.h"
#include <Arduino.h>
#include "setup/I2C.h" 
#include "setup/ssd1306.h" 

volatile uint8_t receiveFlag = 0;
volatile uint8_t receivedByte = 0;

#define BAUD 19200
#define MYUBRRF F_CPU/8/BAUD -1
#define MYUBRRH F_CPU/16/BAUD -1 // half speed


void uart_init() {

    // Enable TX & RX 

    SETBIT(UCSR0B, RXEN0);
    SETBIT(UCSR0B, TXEN0);
   // SETBIT(UCSR0B, RXCIE0); // Enable RX complete interrupt

    CLRBIT(UCSR0C, UMSEL01);
    CLRBIT(UCSR0C, UMSEL00);

    CLRBIT(UCSR0C, UPM01);
    CLRBIT(UCSR0C, UPM00);

    CLRBIT(UCSR0B, UCSZ02);
    SETBIT(UCSR0C, UCSZ01);
    SETBIT(UCSR0C, UCSZ00);

    SETBIT(UCSR0A, U2X0);
    UBRR0H = (MYUBRRF) >> 8;
    UBRR0L = (MYUBRRF);

}
unsigned char USART_Receive(void) {
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */
return UDR0;
}

void uart_transmit(unsigned char data) {

    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void uart_print(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

int uart_receive_string(char *p, int max) {
    int i = 0;
    char c;
    while (i < max - 1) {
        c = USART_Receive();
        if (c == '\n' || c == '\r') break;
        p[i++] = c;
    }
    p[i] = '\0';
    return i;
}

ISR(USART0_RX_vect) {
    receivedByte = UDR0;
    receiveFlag = 1;
}


