#include <Arduino.h>


#include <OneButton.h>

#include "drivers/board.h"	// ADC, PWM, GPIO

#include "drivers/Motors.h"

#include "drivers/LineSensor.h"

uint8_t ir_sen_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
uint8_t ir_sen_vals[8];

OneButton btn1(BTN1, true, false);
OneButton btn2(BTN2, true, false);

Motors motors;

bool stream = false;
bool calib_mode = false;
bool stream_raw = false;
unsigned long last_time = 0;

void setup()
{
	Serial.begin(115200);

	motors.begin();
	motors.set_logic(REVERSE, FORWARD, false);

	GPIO_mode(LED_G1, OUTPUT);
	GPIO_mode(LED_G2, OUTPUT);
	GPIO_mode(LED_B, OUTPUT);

	LineSensor_initFromEEPROM(LINE_BLACK);

	// Print calibration values
	btn1.attachClick([](){
		LineSensor_printCalibration();
	});

	// Calibrate
	btn1.attachDoubleClick([](){
		calib_mode = !calib_mode;
		GPIO_write(LED_B, calib_mode);
		if (calib_mode)
		{
			LineSensor_resetCalibration();
		}
	});

	// Save calibration
	btn1.attachLongPressStart([](){
		GPIO_write(LED_B, HIGH);
		LineSensor_saveCalibrationToEEPROM();
		delay(10);
		GPIO_write(LED_B, LOW);
	});

	// Print readings
	btn2.attachClick([](){
		LineSensor_printReadings();
		int line = LineSensor_read(70);
		Serial.print("Line: ");
		Serial.println(line);
	});

	btn2.attachDoubleClick([](){
		stream_raw = !stream_raw;
		GPIO_write(LED_G1, stream_raw);
	});

	btn2.attachLongPressStart([](){
		stream = !stream;
		GPIO_write(LED_G2, LOW);
		GPIO_write(LED_G1, stream);
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

	if (stream_raw && millis() - last_time > 20)
	{
		last_time = millis();
		Serial.write((uint8_t)0xFF);
		Serial.write((uint8_t)0x00);
		LineSensor_printReadingsBinary();
		// LineSensor_printReadings();
		int line = (unsigned int) LineSensor_read(70);
		Serial.write((uint8_t)(line & 0x00FF));
		Serial.write((uint8_t)((line >> 8) & 0x00FF));
		// Serial.print("\tLine: ");
		// Serial.println(line);
	}

	if (stream && millis() - last_time > 20)
	{
		last_time = millis();
		int line = LineSensor_read(70);
		Serial.write((uint8_t)0xFF);
		Serial.write((uint8_t)0x00);
		Serial.write((uint8_t)(line & 0x00FF));
		Serial.write((uint8_t)((line >> 8) & 0x00FF));
		// Serial.println(line);
		GPIO_write(LED_G2, LineSensor_lineDetected());
	}
}