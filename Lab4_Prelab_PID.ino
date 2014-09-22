#include "Arduino.h"
#include "TimerOne.h"

float kP = 0.0;
float kI = 0.0;
float kD = 0.0;
int kPIDPeriod = 50;

volatile int input;
volatile int output;

void setup() 
{
	Serial.begin(115200);
	Timer1.initialize(kPIDPeriod);
	Timer1.attachInterrupt(updateOutput);
}

void loop() 
{
	if (Serial.available() > 0)
	{
		String gain = Serial.readStringUntil('=');
		float value = Serial.parseFloat();
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

void updateOutput()
{
	output = 0;
}
