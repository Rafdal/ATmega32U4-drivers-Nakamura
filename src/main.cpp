#include <Arduino.h>


#include <OneButton.h>

#include "drivers/board.h"	// ADC, PWM, GPIO

#include "drivers/Motors.h"

uint8_t ir_sen_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
uint8_t ir_sen_vals[8];

OneButton btn1(BTN1, true, false);
OneButton btn2(BTN2, true, false);

Motors motors;

void read_analog(void);

void setup()
{
	Serial.begin(115200);

	motors.begin();
	motors.set_logic(REVERSE, FORWARD, false);

	GPIO_mode(LED_G1, OUTPUT);
	GPIO_mode(LED_G2, OUTPUT);

	btn1.attachClick([](){
		delay(1000);
		GPIO_write(LED_G1, HIGH);
		motors.test_both(100, FORWARD);
		GPIO_write(LED_G1, LOW);
	});

}

void loop()
{
	btn1.tick();
	btn2.tick();
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