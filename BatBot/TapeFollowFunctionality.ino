void tapeFollowTime(int16_t speed, long timeMS, int16_t kP, int16_t kD, int16_t &lerr)
{
	// PID Variables
	
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;

	int TICKS = 40;

	long startTime = millis();

	//LCD.clear(); LCD.home(); LCD.print(startTime);
	//delay(2000);

	//while ((leftCount < MAX_COUNT) || (rightCount < MAX_COUNT))
	while (millis() - startTime < timeMS)
	{

		//uint16_t left = analogRead(LEFT_QRD);
		//uint16_t right = analogRead(RIGHT_QRD);

		//AVERAGE NOISE REMOVAL *FEATURE 1*
		
		uint16_t sum_left = 0;
		uint16_t sum_right = 0;

		long s2 = micros(); //just a debugging feature
		for (int i=0; i< TICKS; i++) {
			sum_left += analogRead(LEFT_QRD);
			sum_right += analogRead(RIGHT_QRD);
		}
		long e2 = micros(); //just a debugging feature

		uint16_t left = sum_left/TICKS;
		uint16_t right = sum_right/TICKS;


		uint16_t side = analogRead(SIDE_QRD);

		int16_t error = 0;

		//THRESHOLD DEADZONE *FEATURE 2*

		/*
		//these definitions are stupid, but temporary
		int THRESH_U = THRESH_L;
		int THRESH_D = THRESH_R;

		if ((left > THRESH_U) && (right > THRESH_U)) error = 0;
		else if ((left > THRESH_U) && (right < THRESH_D)) error = -1;
		else if ((left < THRESH_D) && (right > THRESH_U)) error = +1;

		// History for both tapes off the tape
		else if ((left < THRESH_D) && (right < THRESH_D))
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

		else {continue;} //ignore the result if the values are in the threshold 'deadzone' between upper and lower thresholds
		*/

		if ((left > THRESH_L) && (right > THRESH_R)) error = 0;
		if ((left > THRESH_L) && (right < THRESH_R)) error = -1;
		if ((left < THRESH_L) && (right > THRESH_R)) error = +1;

		// History for both tapes off the tape
		if ((left < THRESH_L) && (right < THRESH_R))
		{
			if (lerr > 0) { error = 3; }
			else { error = -3; }

			/*if (millis() - startTime < 6000)
			{
				if (lerr > 0) { error = 3; }
				else error = -3;
			}
			else
			{
				if (lerr <= 0) { error = -3; }
				else error = +3;
			}*/

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

		int16_t p = kP*error;
		int16_t d = (int16_t)((float)kD*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;// +integ;

		if (c == 100)
		{
			//kP = knob(6) / 1023.0 * 200;
			//kD = knob(7) / 1023.0 * 200;

			LCD.clear();

			LCD.setCursor(0, 0);
			LCD.print(left); LCD.print("  "); LCD.print(right); LCD.print(" "); LCD.print(side);

			LCD.setCursor(0, 1);
			LCD.print(kP); LCD.print("  "); LCD.print(kD); LCD.print("  "); LCD.print(e2-s2);

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
	motor.stop_all();
	LCD.clear(); LCD.home();
	/*while (true)
	{
		LCD.home(); LCD.print("End of TIME");
	}
	*/


}

void tapeFollowAnalog(uint16_t motorSpeed, uint16_t totalCount, int16_t kP, int16_t kD, int16_t &lerr)
{
	// PID Following Variables
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	
	int TICKS = 40;


	uint16_t count = 0;

	while (count < totalCount)
	{
		// variables

		uint16_t sum_left = 0;
		uint16_t sum_right = 0;

		long s2 = micros(); //just a debugging feature
		for (int i = 0; i< TICKS; i++) {
			sum_left += analogRead(LEFT_QRD);
			sum_right += analogRead(RIGHT_QRD);
		}
		long e2 = micros(); //just a debugging feature

		uint16_t left = sum_left / TICKS;
		uint16_t right = sum_right / TICKS;

		uint16_t side = analogRead(SIDE_QRD);


		if (side > THRESH_S)
		{
			delay(100);
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

		int16_t p = kP*error;
		int16_t d = (int16_t)((float)kD*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;// +integ;

		if (c == 100)
		{
			//kp = knob(6) / 1023.0 * 200;
			//kd = knob(7) / 1023.0 * 200;

			LCD.clear();
			LCD.setCursor(0, 0);
			LCD.print(left); LCD.print("  "); LCD.print(right); LCD.print(" "); LCD.print(side);

			LCD.setCursor(0, 1);
			LCD.print("KP:"); LCD.print(kP); LCD.print(" KD:"); LCD.print(kD); LCD.print("  "); LCD.print(count);

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
	
	/*LCD.clear(); LCD.home();
	while (true)
	{
		LCD.home();
		LCD.print("GoodBye! ("); LCD.print(count); LCD.print(")");
	}
	*/
}

