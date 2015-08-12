void fixation(long timeMS_1, long timeMS_2, long timeMS_3, long timeMS_4, bool track)
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
	delay(300);


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
	pickUpNum5(track);
	
	delay(700);

	// Forward Long
	speedLeft = speedIR;

	startTime = millis();

	//timeMS_3 = trackNum ? 500 : 700;

	while (millis() - startTime < timeMS_3)
	{
		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);
	}

	motor.stop_all();
	delay(500);

	// Turn
	speedLeft = 0;
	speedRight = speedIR;

	startTime = millis();
	while (millis() - startTime < timeMS_4)
	{
		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);
	}
	motor.stop_all();
	delay(200);

}

void goBack(long timeMS, int16_t speed)
{
	long startTime = millis();
	while (millis() - startTime < timeMS)
	{
		motor.speed(LEFT_MOTOR, -speed);
		motor.speed(RIGHT_MOTOR, -speed);
	}

	motor.stop_all();
	delay(100);
}

void goForward(long time)
{
	long startTime = millis();
	while (millis() - startTime < time)
	{
		motor.speed(LEFT_MOTOR, speedIR);
		motor.speed(RIGHT_MOTOR, speedIR);
	}

	motor.stop_all();
	delay(100);
}

void turn(long timeMS)
{
	long startTime = millis();
	while (millis() - startTime < timeMS)
	{
		motor.speed(LEFT_MOTOR, speedIR);
		motor.speed(RIGHT_MOTOR, -speedIR);
	}

	motor.stop_all();
	delay(100);
}
