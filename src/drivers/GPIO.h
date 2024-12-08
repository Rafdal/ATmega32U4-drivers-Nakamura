#ifndef GPIO_H
#define GPIO_H
/**
 * @file GPIO.h
 * @brief GPIO library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */

#include <Arduino.h>

#undef PC6
#undef PD3
#undef PD2
#undef PD1
#undef PD0
#undef PB7
#undef PE6
#undef PB3
#undef PB2
#undef PB1
#undef PB0
#undef PD5

typedef uint8_t GPIO_PIN_t;

#define PB0 (GPIO_PIN_t)(LED_BUILTIN_RX)        // RX_LED, SS
#define PB1 (GPIO_PIN_t)(PIN_SPI_SCK)        	// SCK
#define PB2 (GPIO_PIN_t)(PIN_SPI_MOSI)       	// MOSI
#define PB3 (GPIO_PIN_t)(PIN_SPI_MISO)       	// MISO
#define PB7 (GPIO_PIN_t)(11)         	        // OC0A, OC1C
#define PC6 (GPIO_PIN_t)(5)     	            // OC4A
#define PD0 (GPIO_PIN_t)(PIN_WIRE_SCL)      	// SCL
#define PD1 (GPIO_PIN_t)(PIN_WIRE_SDA)       	// SDA
#define PD2 (GPIO_PIN_t)(0)     
#define PD3 (GPIO_PIN_t)(1)     
#define PD5 (GPIO_PIN_t)(LED_BUILTIN_TX)        // CTS, TX_LED
#define PE6 (GPIO_PIN_t)(7)     

#define GPIO_PB0 PB0    // RX_LED, SS
#define GPIO_PB1 PB1    // SCK
#define GPIO_PB2 PB2    // MOSI
#define GPIO_PB3 PB3    // MISO
#define GPIO_PB7 PB7    // OC0A, OC1C
#define GPIO_PC6 PC6    // OC4A
#define GPIO_PD0 PD0    // SCL
#define GPIO_PD1 PD1    // SDA
#define GPIO_PD2 PD2
#define GPIO_PD3 PD3
#define GPIO_PD5 PD5    // TX_LED
#define GPIO_PE6 PE6

void GPIO_mode(GPIO_PIN_t pin, uint8_t mode);   // INPUT, INPUT_PULLUP, OUTPUT
uint8_t GPIO_read(GPIO_PIN_t pin);
void GPIO_write(GPIO_PIN_t pin, uint8_t val);

void GPIO_toggle(GPIO_PIN_t pin);


#endif // GPIO_H