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

uint8_t speed = 0;
unsigned long run_start_time = 0;
unsigned long run_time_ms = 0;

void motor_test_sequence(void); // No usado, NO USA PID

void setup()
{
	Serial.begin(115200);

	motors.begin();
	motors.set_logic(REVERSE, FORWARD, false);
	motors.set_compensation(-0.03, 0);	// Ajuste del desbalance de los motores

	// speed = 100;
	// run_time_ms = 15000;

	pid.set_out_limits(-100, 100);	// Saturación de la salida
	pid.set_integral_limit(100);	// Saturación de la integral
	pid.set_freq(40);				// Hz

	pid.set_K(11.7f, 0.0f, 0.25f);		// ANDA FLAMA
	// pid.kP = 16.0; // Oscila

	GPIO_mode(LED_G1, OUTPUT);
	GPIO_mode(LED_G2, OUTPUT);
	GPIO_mode(LED_B, OUTPUT);

	LineSensor_initFromEEPROM(LINE_BLACK);	// Cargo la calibración guardada en la EEPROM

	// STOP all, print Calibration and Line values
	btn1.attachClick([](){
		motors.disable();
		run = false;
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
		motors.disable();
		GPIO_write(LED_G1, LOW);
		GPIO_write(LED_G2, LOW);
	});

	// STATIC TEST
	btn2.attachDoubleClick([](){
		GPIO_write(LED_B, HIGH);
		delay(1000);
		run = true;
		motors.enable(true, true);
		speed = 0;
		run_time_ms = 0;
		run_start_time = millis();
		GPIO_write(LED_B, LOW);
	});

	// RUN
	btn2.attachLongPressStart([](){
		GPIO_write(LED_B, HIGH);
	});
	btn2.attachLongPressStop([](){
		delay(500);
		GPIO_write(LED_B, LOW);
		LineSensor_read();
		run = true;
		speed = 100;
		run_time_ms = 15000;
		motors.enable(true, true);
		run_start_time = millis();
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
		motors.drive_high_level(100, speed, turn);
		// Serial.print("\tLine: ");
		// Serial.print(line);
		// Serial.print("\tTurn: ");
		// Serial.print(turn);
		// Serial.print("\tInt: ");
		// Serial.println(pid.integral_value(), 0);

		GPIO_toggle(LED_G2);

		if (LineSensor_lineOutOfRange())
		{
			motors.disable();
			GPIO_write(LED_G1, LOW);
		}
		else
		{
			GPIO_write(LED_G1, HIGH);
			motors.enable(true, true);
		}

		// Si run time = 0, corre indefinidamente
		if (run_time_ms != 0 && (millis() - run_start_time > run_time_ms))
		{
			run = false;
			motors.disable();
			GPIO_write(LED_G1, LOW);
			GPIO_write(LED_G2, LOW);
		}
	}
}





void motor_test_sequence()
{
	GPIO_write(LED_B, HIGH);
	delay(1000);

	/* uint8_t power = 100;
	int8_t line_start, line_end, line_delta;
	for (int i = 0; i < 6; i++)
	{
		Serial.print("Power: ");
		Serial.println(power);
		motors.drive_high_level(power, 0, 100);
		line_start = LineSensor_read();
		motors.enable(true, true);
		delay(120);
		motors.disable();
		
		delay(500);
		line_end = LineSensor_read();
		line_delta = line_end - line_start;
		Serial.print("Line start: ");
		Serial.println(line_start);
		Serial.print("Line end: ");
		Serial.println(line_end);
		Serial.print("Line delta: ");
		Serial.println(line_delta);

		motors.drive_high_level(power, 35, -100);
		motors.enable(true, true);
		delay(120);
		motors.disable();

		delay(1000);

		power -= 5;
	} */

	GPIO_write(LED_B, LOW);
}