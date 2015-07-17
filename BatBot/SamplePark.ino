void park()
{
	uint16_t speed0 = menuItems[0].Value;
	int start = millis();
	int stop;
	do
	{
		stop = millis();
		LCD.clear(); LCD.home();
		LCD.print("moving forward");
		motor.speed(0, speed0);
		motor.speed(2, speed0);
	} while ((stop - start) < 7000);

	motor.stop_all();
	/* int start1 = millis(), stop1;
	do
	{
		LCD.clear(); LCD.home();
		LCD.print("Rotating");
		stop1 = millis();
		motor.speed(0, 500);
	} while ((stop1 - start1) < 7000); */

	int c = 0;

	while (true)
	{
		LCD.clear(); LCD.home();
		LCD.print("moving forward");
		motor.speed(0, speed0);
		motor.speed(2, speed0);
		c++;
		if (c == 1000) { speed0 = -1 * speed0; c = 0; }
	}

}
