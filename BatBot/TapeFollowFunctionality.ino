void tapeFollowTime(int16_t speed, uint16_t timeMS)
{
	// PID Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;

	uint16_t startTime = millis();
	//LCD.clear(); LCD.home(); LCD.print(startTime);
	//delay(2000);

	//while ((leftCount < MAX_COUNT) || (rightCount < MAX_COUNT))
	while (millis() - startTime < timeMS)
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
			if (millis() - startTime < 6000)
			{
				if (lerr > 0) { error = 3; }
				else error = -3;
			}
			else
			{
				if (lerr <= 0) { error = -3; }
				else error = +3;
			}

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
		integ += kI.Value * error;
		last_integral_update_ms = millis();
		}

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int16_t p = kp*error;
		int16_t d = (int16_t)((float)kd*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;// +integ;

		if (c == 100)
		{
			//kp = knob(6) / 1023.0 * 200;
			//kd = knob(7) / 1023.0 * 200;

			LCD.clear();

			LCD.setCursor(0, 0);
			LCD.print(left); LCD.print("  "); LCD.print(right); LCD.print(" "); LCD.print(side);

			LCD.setCursor(0, 1);
			LCD.print(leftCount); LCD.print("  "); LCD.print(rightCount);

			c = 0;
		}

		c++;
		m++;


		int16_t speedRight = speed + con;
		int16_t speedLeft = speed - con;

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;
	}


	/* Measure Ending Part (To Be Removed) */
	/*motor.stop_all();
	LCD.clear(); LCD.home();
	while (!reset)
	{
	LCD.home(); LCD.print("End of Counting");
	LCD.setCursor(0, 1);

	LCD.print(leftCount); LCD.print("  "); LCD.print(rightCount);
	}*/


}

void tapeFollowAnalog(uint16_t motorSpeed, uint16_t totalCount)
{
	// PID Following Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	
	uint16_t count = 0;

	while (count < totalCount)
	{
		// variables
		uint16_t left = analogRead(LEFT_QRD);
		uint16_t right = analogRead(RIGHT_QRD);
		uint16_t side = analogRead(SIDE_QRD);


		if (side > THRESH_S)
		{
			delay(80);
			if (side > THRESH_S) { count++; }
		}

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
		integ += kI.Value * error;
		last_integral_update_ms = millis();
		}

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int16_t p = kp*error;
		int16_t d = (int16_t)((float)kd*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;// +integ;

		if (c == 100)
		{
			//kp = knob(6) / 1023.0 * 200;
			//kd = knob(7) / 1023.0 * 200;

			LCD.clear();
			LCD.setCursor(0, 0);
			LCD.print(left); LCD.print("  "); LCD.print(right); LCD.print(" "); LCD.print(side);

			LCD.setCursor(0, 1);
			LCD.print("KP:"); LCD.print(kp); LCD.print(" KD:"); LCD.print(kd); LCD.print("  "); LCD.print(count);

			c = 0;
		}

		c++;
		m++;

		int16_t speedRight = motorSpeed + con;
		int16_t speedLeft = motorSpeed - con;

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;
	}

	/* Measuring Stopping at the right place : To Be Removed */
	/*motor.stop_all();
	LCD.clear(); LCD.home();
	while (true)
	{
		LCD.home();
		LCD.print("GoodBye! ("); LCD.print(count); LCD.print(")");
	}
	*/
}

