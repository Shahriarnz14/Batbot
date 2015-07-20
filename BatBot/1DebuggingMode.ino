/*
	Testing Wheels Movement. And Wheel Encoder Values
 */
void speedTest()
{
	int16_t c;

	while (true)
	{
		int16_t speed = knob(6);
		int16_t positive = knob(7);

		if (positive == 0) speed = -speed;

		motor.speed(LEFT_MOTOR, speed);
		motor.speed(RIGHT_MOTOR, speed);

		
		if (c == 100)
		{
			LCD.clear(); LCD.home();
			LCD.print(leftCount); LCD.print("  "); LCD.print(rightCount);
			
			LCD.setCursor(0, 1);
			LCD.print("L:"); LCD.print(speed); LCD.print(" R:"); LCD.print(speed);
			c = 0;
		}

		c++;
	}
}

void tapeFollowTest()
{
	// PID Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	int16_t c;

	uint16_t kP = knob(6) / 1023.0 * 200;
	uint16_t kD = knob(7) / 1023.0 * 200;


	while (true)
	{
		// variables
		uint16_t left = analogRead(LEFT_QRD);
		uint16_t right = analogRead(RIGHT_QRD);
		uint16_t side = analogRead(SIDE_QRD);


		int16_t error = 0;

		if ((left > THRESH_L) && (right > THRESH_R)) error = 0;
		if ((left > THRESH_L) && (right < THRESH_R)) error = -1;
		if ((left < THRESH_L) && (right > THRESH_R)) error = +1;

		// History for both tapes off the tape
		if ((left < THRESH_L) && (right < THRESH_R))
		{
			if (lerr > 0) { error = 3; }
			else error = -3;
		}


		// Derivative Approximation
		if (error != lerr)
		{
			recerr = lerr;
			q = m;
			m = 1;
		}

		/*
		// Integral Approximation
		if (millis() > INTEGRAL_UPDATE_DELAY_MS + last_integral_update_ms)
		{
		integ += kI * error;
		last_integral_update_ms = millis();
		}

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int16_t p = kP*error;
		int16_t d = (int16_t)((float)kD*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;// +integ;

		if (c == 100)
		{
			kP = knob(6) / 1023.0 * 200;
			kD = knob(7) / 1023.0 * 200;

			LCD.clear();

			LCD.setCursor(0, 0);
			LCD.print(left); LCD.print("  "); LCD.print(right); LCD.print(" "); LCD.print(side);

			LCD.setCursor(0, 1);
			LCD.print(kP); LCD.print("  "); LCD.print(kD);

			c = 0;
		}

		c++;
		m++;


		int16_t speedRight = speed0 + con;
		int16_t speedLeft = speed0 - con;

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;
	}


	/* Measure Ending Part*/
	motor.stop_all();
	LCD.clear(); LCD.home();
	while (true)
	{
		LCD.home(); LCD.print("End of Counting");
		LCD.print(leftCount); LCD.print("  "); LCD.print(rightCount);
	}


}
