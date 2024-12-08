#include <Arduino.h>

#include "ADC.h" 	// ADC0, ADC1, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, ADC10, ADC11, ADC12, ADC13
#include "PWM.h"	// OC4A, OC4D
#include "GPIO.h" 	// PC6, PD3, PD2, PD1, PD0, PB7, PE6, PB3, PB2, PB1, PB0, PD5

#define M1_PWM OC4A
#define M2_PWM OC4D

#define M1_EN PD1
#define M2_EN PD2

#define M1_DIR PC6
#define M2_DIR PD3

#define SENSOR_1 ADC0
#define SENSOR_2 ADC1
#define SENSOR_3 ADC4
#define SENSOR_4 ADC5
#define SENSOR_5 ADC6
#define SENSOR_6 ADC7
#define SENSOR_7 ADC8
#define SENSOR_8 ADC9

#define SENSOR_IR_ENABLE PD0

#define SENS_OUT_B ADC12
#define SENS_OUT_A ADC13

#define LED_G1	PB3
#define LED_G2	PB2
#define LED_B1	PB1

uint8_t ir_sen_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
uint8_t ir_sen_vals[8];


void read_analog(void)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		ir_sen_vals[i] = ADC_read(ir_sen_pins[i]) >> 2;
	}

	Serial.write((uint8_t)0xFF);
	Serial.write((uint8_t)0x00);
	for (uint8_t i = 0; i < 8; i++)
	{
		Serial.write(ir_sen_vals[i]);
	}

	// uint8_t s1 = map(ADC_read(SENSOR_1), 0, 1023, 0, 160);
	// uint8_t s2 = map(ADC_read(SENSOR_2), 0, 1023, 0, 160);
	// Serial.print("S1:\t");
	// Serial.print(s1);
	// Serial.print("\tS2:\t");
	// Serial.println(s2);
	// PWM_write_both(s1, s2);
}

void setup()
{
	Serial.begin(115200);
	GPIO_mode(SENSOR_IR_ENABLE, OUTPUT);
	GPIO_write(SENSOR_IR_ENABLE, HIGH);

	GPIO_mode(LED_G1, OUTPUT);
	GPIO_mode(LED_G2, OUTPUT);
	GPIO_mode(LED_B1, OUTPUT);

	// PWM_setup(PWM_PRESCALER_4, 159, FAST_PWM);
	// PWM_setup();


	// GPIO_mode(M1_EN, OUTPUT);
	// GPIO_mode(M1_DIR, OUTPUT);

	// GPIO_write(M1_EN, HIGH);
	// GPIO_write(M1_DIR, LOW);

	// GPIO_mode(M2_EN, OUTPUT);
	// GPIO_mode(M2_DIR, OUTPUT);

	// GPIO_write(M2_EN, HIGH);
	// GPIO_write(M2_DIR, LOW);

	// PWM_write(M1_PWM, 255);
}

void loop()
{
	GPIO_write(LED_G1, HIGH);
	delay(1000);
	GPIO_write(LED_G1, LOW);
	GPIO_write(LED_G2, HIGH);
	delay(1000);
	GPIO_write(LED_G2, LOW);
	GPIO_write(LED_B1, HIGH);
	delay(1000);
	GPIO_write(LED_B1, LOW);

	// read_analog();
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

