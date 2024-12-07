#include <Arduino.h>

#include "ADC.h" 	// ADC0, ADC1, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, ADC10, ADC11, ADC12, ADC13
#include "PWM.h"	// OC4A, OC4D
#include "GPIO.h" 	// PC6, PD3, PD2, PD1, PD0, PB7, PE6, PB3, PB2, PB1, PB0, PD5

#define M1_PWM OC4A
#define M2_PWM OC4D

#define SENSOR_1 ADC0
#define SENSOR_2 ADC1
#define SENSOR_3 ADC4
#define SENSOR_4 ADC5
#define SENSOR_5 ADC6
#define SENSOR_6 ADC7
#define SENSOR_7 ADC8
#define SENSOR_8 ADC9

#define SENS_OUT_B ADC12
#define SENS_OUT_A ADC13

int sens_pins[10] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
const char* sens_names[10] = {"SENSOR_1", "SENSOR_2", "SENSOR_3", "SENSOR_4", "SENSOR_5", "SENSOR_6", "SENSOR_7", "SENSOR_8"};

void read_analog(void)
{
	for (uint8_t i = 0; i < 10; i++)
	{
		int val = ADC_read(sens_pins[i]);
		if (val == 0)
		{
			Serial.println(sens_names[i]);
		}
	}
	// Serial.print("ADC0: ");
	// Serial.println(val);
	// PWM_write(OC4A, val);
}

void setup()
{
	Serial.begin(115200);

	// PWM_setup();
	// PWM_write(M1_PWM, (75*160)/100);
	// PWM_write(M2_PWM, (40*160)/100);

}

void loop()
{
	read_analog();
	delay(1000);
	// put your main code here, to run repeatedly:
	// Serial.println("Hello World");
	// read_analog();
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

