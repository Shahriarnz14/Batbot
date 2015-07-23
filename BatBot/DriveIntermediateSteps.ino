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
