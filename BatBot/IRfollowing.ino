int dif;
void IRfollow()
{
	//int thresh = 400;
	float leftQrd = analogRead(0);
	float rightQrd = analogRead(1);

	while (true)
	{
		// variables
		float left = analogRead(2);
		float right = analogRead(3);

		leftQrd = analogRead(0);
		rightQrd = analogRead(1);

		// Knobs

		int error = 0;
		dif = left - right;

		//if ((left>THRESH) && (right>THRESH)) error = 0;
		if ((dif <= 1) || (dif >= -1)) error = 0;
		//if ((left>THRESH) && (right<THRESH)) error = -1;
		if (dif > 1) error = -2;
		//if ((left<THRESH) && (right>THRESH)) error = +1;
		if (dif < -1) error = +2;

		/*
		// History for both tapes off the tape
		if ((left<THRESH) && (right<THRESH))
		{
			if (lerr>0) error = 5;
			else error = -5;
		}
		*/

		// Derivative Approximation
		if (error != lerr)
		{
			recerr = lerr;
			q = m;
			m = 1;
		}

		/*
		// Integral Approximation
		integ += menuItems[4].Value * error;

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int p = kp*error;
		int d = (int)((float)kd*(float)(error - recerr) / (float)(q + m));
		int con = p + d;

		if (c == 100)
		{
			kp = knob(6);
			kd = knob(7);

			LCD.clear();
			LCD.setCursor(0, 0);
			//LCD.print("R: "); LCD.print((int)right); LCD.print(" L: "); LCD.print((int)left);
			LCD.print((int16_t)left); LCD.print("  "); LCD.print((int16_t)right);
			//LCD.print("P:"); LCD.print(p); LCD.print(" D:"); LCD.print(d); LCD.print(" C:"); LCD.print(con);

			LCD.setCursor(0, 1);
			LCD.print("KP:"); LCD.print((int16_t)kp); LCD.print(" KD:"); LCD.print((int16_t)kd);

			c = 0;
		}

		c++;
		m++;

		//hello! very nice!
		int speedRight = speedIR + con;
		int speedLeft = speedIR - con;
		con;

		/*  if (speed1 > 700) speed1 = 700;
		else if (speed1<-700) speed1 = -700;

		if (speed2 > 700) speed2 = 700;
		else if (speed2<-700) speed2 = -700; */

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;

		if ((leftQrd > THRESH) > (rightQrd > THRESH)) { break; }
	}

	motor.stop_all();
}

