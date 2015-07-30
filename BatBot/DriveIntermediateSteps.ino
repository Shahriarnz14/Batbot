void fixation(long timeMS_1, long timeMS_2, long timeMS_3)
{
	// Forward Short
	int16_t speedLeft = speedIR;
	int16_t speedRight = speedIR;

	long startTime = millis();
	while (millis() - startTime < timeMS_1)
	{
		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);
	}


	// Turn
	speedLeft = 0;
	speedRight = speedIR;

	startTime = millis();
	while (millis() - startTime < timeMS_2)
	{
		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);
	}
	motor.stop_all();
	delay(500);

	// Forward Long
	speedLeft = speedIR;

	startTime = millis();
	while (millis() - startTime < timeMS_3)
	{
		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);
	}

	motor.stop_all();
	delay(500);
}

void goBack()
{
	uint16_t startTime = millis();
	while (millis() - startTime < 1000)
	{
		motor.speed(LEFT_MOTOR, -speedIR);
		motor.speed(RIGHT_MOTOR, -speedIR);
	}

	motor.stop_all();
	delay(2000);
}

void turn()
{
	uint16_t startTime = millis();
	while (millis() - startTime < 1400)
	{
		motor.speed(LEFT_MOTOR, speedIR);
		motor.speed(RIGHT_MOTOR, -speedIR);
	}

	motor.stop_all();
	delay(2000);
}
