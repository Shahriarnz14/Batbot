void tapeFollow1()
{
	LCD.clear(); LCD.home(); LCD.print("TAPE-FOLLOW");
	tapeFollowTime(speed0, 8900);
	tapeFollowAnalog(speed0, 1); // Continue until only "1" cross section remains
}

void tapeFollow2()
{
	tapeFollowTime(speedBack, 1000);
	for (int i = 0; i < 3; i++)
	{
		tapeFollowAnalog(speedBack, 1);
		motor.stop_all();
		delay(2000);
		// Pick Up Pet
	}
	tapeFollowTime(speedBack, 5000);
}