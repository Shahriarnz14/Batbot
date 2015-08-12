void tapeFollow1()
{
	LCD.clear(); LCD.home(); LCD.print("TAPE-FOLLOW");
	int16_t lerr = 0;
	//tapeFollowTime(speed0, 8800);
	tapeFollowTime(speed0, 8450, kp, kd, lerr);
	//motor.stop_all();
	//delay(500);
	tapeFollowAnalog(speedIR, 1, kp_B, kd_B, lerr); // Continue until only "1" cross section remains
	motor.stop_all();
	delay(100);
}

void tapeFollow2(bool track)
{
	int16_t lerr = 0;
	tapeFollowTime(speedBack, 4000, kp_B, kd_B, lerr);
	//motor.stop_all();
	//delay(500);
	for (int i = 0; i < 4; i++)
	{
		tapeFollowAnalog(speedBack, 1, kp_B, kd_B, lerr);
		motor.stop_all();
		delay(100);
		
		if (i == 0)
		{
			//long goBackTime = track ? 700 : 600;
			long goBackTime = track ? 850 : 850; // was 650 1228am
			goBack(goBackTime, 200);
			delay(500);
			dropOffEdge();
			pickUpNum3();
			delay(200);
			goForward(1200); //change from 500 to 1200 at 2:22am

			motor.stop_all();

			delay(200);

			// Turn
			int16_t speedLeft = 0;
			int16_t speedRight = speedIR;

			long startTime = millis();
			while (millis() - startTime < 300)
			{
				motor.speed(LEFT_MOTOR, speedLeft);
				motor.speed(RIGHT_MOTOR, speedRight);
			}
			motor.stop_all();
			lerr = -1;

			delay(100);
			
		}


		if (i == 1)
		{
			pickUpNum2();
			lerr = 1;
			tapeFollowTime(speedBack, 500, kp_B, kd_B, lerr);
			//store2(); in pickup 2 now
		}

		if (i == 2) 
		{
			lerr = 1;
			pickUpNum1();
		}
	}

	tapeFollowTime(speedBack, 2147483647, kp_B, kd_B, lerr);
}