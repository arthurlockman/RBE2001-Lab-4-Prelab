#include "Arduino.h"
#include "Servo.h"
#include "TimerOne.h"

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

int kPotMax = 500;
int kPotMin = 170;

static const int kMotorForward = 22;
static const int kMotorReverse = 23;
static const int kPotPin = A1;

void setup() 
{
	Serial.begin(115200);
	m_setpoint = 100;
	pinMode(kPotPin, INPUT);
	Timer1.initialize(kPIDPeriod);
	Timer1.attachInterrupt(calculatePID);
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
		} else if (gain == "S" || gain == "s") {
			m_setpoint = value;
			Serial.print("Setpoint set to ");
			Serial.print(value);
			Serial.println();
		} else {
			Serial.println("Unrecognized value.");
		}
	}
	Serial.println(input);
	delay(100);
}

void calculatePID()
{
	input = analogRead(kPotPin);
	int err = m_setpoint - input;
	m_i += err;
	output = kP * err + kI * (m_i) + kD * (input - m_d);
	m_d = input;
	output = (output >= 255)? 255 : (output <= -255)? -255 : output;
	writeToMotor(output);
}

void writeToMotor(int input)
{
	if (input > 0)
	{
		digitalWrite(kMotorReverse, LOW);
		analogWrite(kMotorForward, input);
	} else if (input == 0) {
		digitalWrite(kMotorReverse, LOW);
		digitalWrite(kMotorForward, LOW);
	} else {
		digitalWrite(kMotorForward, LOW);
		analogWrite(kMotorReverse, -input);
	}
}
