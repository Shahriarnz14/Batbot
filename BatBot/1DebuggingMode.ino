/*
	Testing Wheels Movement.
 */
void speedTest()
{
	while (true)
	{
		int16_t speed = knob(6);
		int16_t positive = knob(7);

		if (positive == 0) speed = -speed;

		motor.speed(LEFT_MOTOR, speed);
		motor.speed(RIGHT_MOTOR, speed);

		/*
		if (c == 100)
		{
			LCD.clear();
			LCD.home(); LCD.print("Debugging Speed");
			LCD.setCursor(0, 1);
			LCD.print("L:"); LCD.print(speed); LCD.print(" R:"); LCD.print(speed);
			c = 0;
		}*/

		c++;
	}
}
