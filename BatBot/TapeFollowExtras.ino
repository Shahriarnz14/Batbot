/*
This function performs the tape following task.
If center is on the line there a major different in our error compared to when it's not.
L	C	R	|	Error
=====================
1	1	1	|	0
0	1	0	|	0
0	1	1	|	-1
1	1	0	|	+1
_____________________
0	0	1	|	-3
1	0	0	|	+3
0	0	0*	|	-5
0*	0	0	|	+5
*/
void tapeFollow3QRD()
{
	// Tape Following Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	int16_t c;


	while (true)
	{
		// variables
		float left = analogRead(0);
		float center = analogRead(2);
		float right = analogRead(1);

		//uint16_t speed0 = menuItems[0].Value;
		//uint16_t kp = menuItems[1].Value;
		//uint16_t kd = menuItems[2].Value;

		//uint16_t speed0 = 100;
		uint16_t kp = knob(6);
		uint16_t kd = knob(7);

		int16_t error = 0;
		if (center > THRESH_L)
		{
			error = 0;
			if ((right > THRESH_R) && (left < THRESH_L)) error--;
			if ((right < THRESH_R) && (left > THRESH_L)) error++;
		}
		else
		{
			if ((right > THRESH_R) && (left < THRESH_L)) error = -2;
			if ((right < THRESH_R) && (left > THRESH_L)) error = 2;

			// History for both tapes off the tape
			if ((left<THRESH_L) && (right<THRESH_R))
			{
				if (lerr>0) error = 5;
				else error = -5;
			}
		}


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


		// Screen printing
		if (c == 100)
		{
			LCD.clear();
			LCD.home(); LCD.print(speed0 - con); LCD.print(" "); LCD.print(speed0 + con);
			LCD.setCursor(0, 1);
			LCD.print("KP:");  LCD.print(kp);
			LCD.print(" |KD:"); LCD.print(kd);

			c = 0;
		}

		c++;
		m++;


		int16_t speed1 = speed0 + con;
		int16_t speed2 = speed0 - con;

		/*  if (speed1 > 700) speed1 = 700;
		else if (speed1<-700) speed1 = -700;

		if (speed2 > 700) speed2 = 700;
		else if (speed2<-700) speed2 = -700; */

		motor.speed(LEFT_MOTOR, speed2);
		motor.speed(RIGHT_MOTOR, speed1);

		lerr = error;
	}
}



int16_t integ = 0;
uint32_t last_integral_update_ms = 0;
const int INTEGRAL_UPDATE_DELAY_MS = 5;

/*
Tape Following with 2 QRDs.
*/
void tapeFollow2QRD(volatile int16_t *flagOn, uint16_t motorSpeed)
{
	// Tape Following Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	int16_t c;


	//int thresh = 400;
	while (*flagOn)
		//while (true)
	{
		// variables
		float left = analogRead(LEFT_QRD);
		float right = analogRead(RIGHT_QRD);

		// Knobs

		int error = 0;

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
		integ += menuItems[4].Value * error;
		last_integral_update_ms = millis();
		}

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int p = kp*error;
		int d = (int)((float)kd*(float)(error - recerr) / (float)(q + m));
		int con = p + d;// +integ;

		if (c == 100)
		{
			//kp = knob(6) / 1023.0 * 200;
			//kd = knob(7) / 1023.0 * 200;

			LCD.clear();
			LCD.setCursor(0, 0);
			//LCD.print("R: "); LCD.print((int)right); LCD.print(" L: "); LCD.print((int)left);
			LCD.print((int16_t)left); LCD.print("  "); LCD.print((int16_t)right); LCD.print(" "); LCD.print(digitalRead(1));
			//LCD.print("P:"); LCD.print(p); LCD.print(" D:"); LCD.print(d); LCD.print(" C:"); LCD.print(con);

			LCD.setCursor(0, 1);
			LCD.print("KP:"); LCD.print((int16_t)kp); LCD.print(" KD:"); LCD.print((int16_t)kd); LCD.print("  "); LCD.print(tapeFlag1);

			c = 0;
		}

		c++;
		m++;


		int speedRight = motorSpeed + con;
		int speedLeft = motorSpeed - con;
		con;

		/*  if (speed1 > 700) speed1 = 700;
		else if (speed1<-700) speed1 = -700;

		if (speed2 > 700) speed2 = 700;
		else if (speed2<-700) speed2 = -700; */

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;
	}

	motor.stop_all();
	LCD.clear(); LCD.home();
	//delay(5000);
	while (true)
	{
		LCD.print("GoodBye!");
	}
}

void tapeFollowAnalogTrial(uint16_t motorSpeed)
{
	// Tape Following Variables
	int16_t lerr = 0;
	int16_t recerr = 0;
	int16_t q = 0;
	int16_t m = 0;
	int16_t c;

	int count = 0;

	while (count < 4)
	{
		// variables
		float left = analogRead(LEFT_QRD);
		float right = analogRead(RIGHT_QRD);

		uint16_t sideRight = analogRead(2);
		uint16_t sideLeft = analogRead(3);

		//if (side > 300) { pressed = 1; }
		//else { pressed = 0; }
		//if (pressed) { motor.stop_all();  delay(1000); //flag = 1; }

		if ((sideRight > THRESH_S) && (sideLeft > THRESH_S))
		{
			delay(100);
			count++;
		}

		//side > 300 ? pressed = 1 : pressed = 0;
		/*for (int i = 0; i < 5; i++)
		{
		pressed &= side > 300 ? 1 : 0;
		}*/

		int error = 0;

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
		integ += menuItems[4].Value * error;
		last_integral_update_ms = millis();
		}

		if (integ > I_MAX) integ = I_MAX;
		else if (-integ > I_MAX) integ = -I_MAX;
		*/

		int p = kp*error;
		int d = (int)((float)kd*(float)(error - recerr) / (float)(q + m));
		int con = p + d;// +integ;

		if (c == 100)
		{
			//kp = knob(6) / 1023.0 * 200;
			//kd = knob(7) / 1023.0 * 200;

			LCD.clear();
			LCD.setCursor(0, 0);
			//LCD.print("R: "); LCD.print((int)right); LCD.print(" L: "); LCD.print((int)left);
			LCD.print((int16_t)left); LCD.print("  "); LCD.print((int16_t)right); LCD.print(" "); //LCD.print(side);
			//LCD.print("P:"); LCD.print(p); LCD.print(" D:"); LCD.print(d); LCD.print(" C:"); LCD.print(con);

			LCD.setCursor(0, 1);
			LCD.print("KP:"); LCD.print(kp); LCD.print(" KD:"); LCD.print(kd); LCD.print("  "); LCD.print(count);

			c = 0;
		}

		c++;
		m++;


		int speedRight = motorSpeed + con;
		int speedLeft = motorSpeed - con;
		con;

		/*  if (speed1 > 700) speed1 = 700;
		else if (speed1<-700) speed1 = -700;

		if (speed2 > 700) speed2 = 700;
		else if (speed2<-700) speed2 = -700; */

		motor.speed(LEFT_MOTOR, speedLeft);
		motor.speed(RIGHT_MOTOR, speedRight);

		lerr = error;
	}

	motor.stop_all();
	LCD.clear(); LCD.home();
	//delay(5000);
	while (true)
	{
		LCD.print("GoodBye!");
	}
}


