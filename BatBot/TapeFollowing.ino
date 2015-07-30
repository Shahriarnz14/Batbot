void tapeFollow1()
{
	LCD.clear(); LCD.home(); LCD.print("TAPE-FOLLOW");
	//tapeFollowTime(speed0, 8800);
	tapeFollowTime(speed0, 7000, kp, kd);
	motor.stop_all();
	delay(500);
	tapeFollowAnalog(speedIR, 1, kp_B, kd_B); // Continue until only "1" cross section remains
	motor.stop_all();
}

void tapeFollow2()
{
	tapeFollowTime(speedBack, 2147483647, kp_B, kd_B);
	//motor.stop_all();
	//delay(500);
	//for (int i = 0; i < 3; i++)
	//{
	//	tapeFollowAnalog(speedBack, 1, kp_B, kd_B);
	//	motor.stop_all();
	//	delay(2000);
	//	// Pick Up Pet
	//}
	//	tapeFollowTime(speedBack, 2147483647, kp_B, kd_B);
}