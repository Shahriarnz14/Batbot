int16_t diff;
void IRfollow()
{
	// PID Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	int16_t c;

	//int thresh = 400;
	uint16_t leftQrd = analogRead(LEFT_QRD);
	uint16_t rightQrd = analogRead(RIGHT_QRD);

	while (true)
	{
		// variables
		uint16_t leftIR = analogRead(LEFT_IR);
		uint16_t rightIR = analogRead(RIGHT_IR);

		leftQrd = analogRead(LEFT_QRD);
		rightQrd = analogRead(RIGHT_QRD);

		if ((leftQrd > THRESH_L) || (rightQrd > THRESH_R)) { break; }

		// Knobs
		int16_t error = 0;
		diff = leftIR - rightIR;

		//if ((left>THRESH) && (right>THRESH)) error = 0;
		if ((diff <= diffMaxIR) || (diff >= -diffMaxIR)) error = 0;
		//if ((left>THRESH) && (right<THRESH)) error = -1;
		if (diff > diffMaxIR) error = -2;
		//if ((left<THRESH) && (right>THRESH)) error = +1;
		if (diff < -diffMaxIR) error = +2;

		// Derivative Approximation
		if (error != lerr)
		{
			recerr = lerr;
			q = m;
			m = 1;
		}

		int16_t p = kp*error;
		int16_t d = (int16_t)((float)kd*(float)(error - recerr) / (float)(q + m));
		int16_t con = p + d;

		if (c == 100)
		{
			kp = knob(6);
			kd = knob(7);

			LCD.clear(); LCD.home();
			LCD.print(leftIR); LCD.print("  "); LCD.print(rightIR);

			LCD.setCursor(0, 1);
			LCD.print(leftQrd); LCD.print(" "); LCD.print(rightQrd);
			//LCD.print("KP:"); LCD.print(kp); LCD.print(" KD:"); LCD.print(kd);

			c = 0;
		}

		c++;
		m++;

		//hello! very nice!
		int speedRight = speedIR + con;
		int speedLeft = speedIR - con;

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;

		
	}

	motor.stop_all();

	/* Testing Tape Finding */
	while (true)
	{
		LCD.home(); LCD.print("End of IR");
		LCD.print(leftQrd); LCD.print("  "); LCD.print(rightQrd);
	}
}

