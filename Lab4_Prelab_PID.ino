#include "Arduino.h"
#include "Encoder.h"

int kP = 0.0;
int kI = 0.0;
int kD = 0.0;
int kPIDPeriod = 50;

volatile int input;
volatile int output;

volatile int m_p;
volatile int m_i;
volatile int m_d;
volatile int m_setpoint;

void setup() 
{
	Serial.begin(115200);
	m_setpoint = 100;
}

void loop() 
{
	if (Serial.available() > 0)
	{
		String gain = Serial.readStringUntil('=');
		float value = Serial.parseInt();
		if (gain == "P" || gain == "p")
		{
			kP = value;
			Serial.print("kP set to ");
			Serial.print(value);
			Serial.println();
		} else if (gain == "I" || gain == "i") {
			kI = value;
			Serial.print("kI set to ");
			Serial.print(value);
			Serial.println();
		} else if (gain == "D" || gain == "d") {
			kD = value;
			Serial.print("kD set to ");
			Serial.print(value);
			Serial.println();
		} else {
			Serial.println("Unrecognized value.");
		}
	}
}

int calculatePID(int input)
{
	int err = m_setpoint - input;
	m_i += err;
	output = kP * err + kI * (m_i) + kD * (input - m_d);
	m_d = input;
	return output;
}
