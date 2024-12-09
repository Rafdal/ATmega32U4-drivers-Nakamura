#include <Arduino.h>
#include <OneButton.h>

#include "drivers/board.h"	// ADC, PWM, GPIO

#include "drivers/Motors.h"
#include "drivers/LineSensor.h"
#include "PID.h"

OneButton btn1(BTN1, true, false);
OneButton btn2(BTN2, true, false);

Motors motors;
PID pid;

bool calib_mode = false;
bool run = false;
unsigned long last_time = 0;

void motor_test_sequence(void); // No usado, NO USA PID

void setup()
{
	Serial.begin(115200);

	motors.begin();
	motors.set_logic(REVERSE, FORWARD, false);
	motors.set_compensation(-0.03, 0);	// Ajuste del desbalance de los motores

	pid.set_out_limits(-100, 100);
	pid.set_integral_limit(100);
	pid.set_freq(20);
	pid.kP = 3.5;
	pid.kI = 2.5;

	GPIO_mode(LED_G1, OUTPUT);
	GPIO_mode(LED_G2, OUTPUT);
	GPIO_mode(LED_B, OUTPUT);

	LineSensor_initFromEEPROM(LINE_BLACK);

	// Print calibration values and line reading
	btn1.attachClick([](){
		motors.enable(false, false);
		GPIO_write(LED_B, HIGH);
		LineSensor_printCalibration();
		int line = LineSensor_read();
		Serial.print("Line: ");
		Serial.println(line);
		delay(10);
		GPIO_write(LED_B, LOW);
		GPIO_write(LED_G1, LOW);
		GPIO_write(LED_G2, LOW);
	});

	// CALIBRATE
	btn1.attachDoubleClick([](){
		calib_mode = !calib_mode;
		GPIO_write(LED_B, calib_mode);
		if (calib_mode)
			LineSensor_resetCalibration();
	});

	// Save calibration to EEPROM
	btn1.attachLongPressStart([](){
		GPIO_write(LED_B, HIGH);
		LineSensor_saveCalibrationToEEPROM();
		delay(10);
		GPIO_write(LED_B, LOW);
	});

	// STOP EVERYTHING
	btn2.attachClick([](){
		run = false;
		motors.enable(false, false);
		GPIO_write(LED_G1, LOW);
		GPIO_write(LED_G2, LOW);
	});

	// TEST MOTORS
	btn2.attachDoubleClick(motor_test_sequence);

	// RUN
	btn2.attachLongPressStart([](){
		GPIO_write(LED_B, HIGH);
	});
	btn2.attachLongPressStop([](){
		GPIO_write(LED_B, LOW);
		LineSensor_read();
		run = true;
		motors.enable(true, true);
	});
}


void loop()
{
	btn1.tick();
	btn2.tick();

	if (calib_mode && millis() - last_time > 100)
	{
		last_time = millis();
		LineSensor_calibrateSensors();
	}

	if (run && millis() - last_time > pid.deltaT_ms())
	{
		last_time = millis();
		int line = LineSensor_read();
		int turn = -(int)pid.run(line, 0);
		motors.drive_high_level(100, 0, turn);
		Serial.print("Line: ");
		Serial.print(line);
		Serial.print("\tTurn: ");
		Serial.print(turn);
		Serial.print("\tIntegral: ");
		Serial.println(pid.integral_value());

		GPIO_toggle(LED_G2);

		if (LineSensor_lineOutOfRange())
		{
			motors.enable(false, false);
			GPIO_write(LED_G1, LOW);
		}
		else
		{
			GPIO_write(LED_G1, HIGH);
			motors.enable(true, true);
		}
	}
}





void motor_test_sequence()
{
	GPIO_write(LED_B, HIGH);
	delay(1000);

	uint8_t power = 100;
	int8_t line_start, line_end, line_delta;
	for (int i = 0; i < 6; i++)
	{
		Serial.print("Power: ");
		Serial.println(power);
		motors.drive_high_level(power, 0, 100);
		line_start = LineSensor_read();
		motors.enable(true, true);
		delay(120);
		motors.enable(false, false);
		
		delay(500);
		line_end = LineSensor_read();
		line_delta = line_end - line_start;
		Serial.print("Line start: ");
		Serial.println(line_start);
		Serial.print("Line end: ");
		Serial.println(line_end);
		Serial.print("Line delta: ");
		Serial.println(line_delta);

		motors.drive_high_level(power, 0, -100);
		motors.enable(true, true);
		delay(120);
		motors.enable(false, false);

		delay(1000);

		power -= 5;
	}

	GPIO_write(LED_B, LOW);
}