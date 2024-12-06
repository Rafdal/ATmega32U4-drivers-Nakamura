#include <Arduino.h>

#include "ADC.h" 	// ADC0, ADC1, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, ADC10, ADC11, ADC12, ADC13
#include "PWM.h"	// OC4A, OC4D
#include "GPIO.h" 	// PC6, PD3, PD2, PD1, PD0, PB7, PE6, PB3, PB2, PB1, PB0, PD5

void read_analog(void)
{
	uint8_t val = ADC_read(ADC0) >> 2;
	Serial.print("ADC0: ");
	Serial.println(val);
	PWM_write(OC4A, val);
}

void setup()
{
	Serial.begin(115200);

	PWM_setup();
	PWM_write(OC4D, 30);
}

void loop()
{
	// put your main code here, to run repeatedly:
	// Serial.println("Hello World");
	read_analog();
	delay(100);
	// state = !state;
	// GPIO_write(pins[selected], state);
	// Serial.print("Selected: ");
	// Serial.println(pin_names[selected]);

	// Serial.println("GPIO Values:");
	// for (uint8_t i = 0; i < 12; i++)
	// {
	// 	Serial.print(pin_names[i]);
	// 	Serial.print("\t");
	// }
	// Serial.println();
	// for (uint8_t i = 0; i < 12; i++)
	// {
	// 	Serial.print(GPIO_read(pins[i]));
	// 	Serial.print("\t");
	// }
	// Serial.println();
}

