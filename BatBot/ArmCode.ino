#define SERVO_RANGE 229 //do not touch, calibration for new servo

//origin is determined to be between the two servos
#define BASE_HEIGHT 5 //base of the arm (Will's mulitple layers)
#define SIDE_LENGTH 12.5 //length from middle to side of the robot
#define FRONT_LENGTH 12.2 //length from base of arm to front of robot
#define SIDE_HEIGHT 5 //height of the side walls of the robot
#define DOLL_HEIGHT 15.24
#define CHASSIS_HEIGHT 5 //height from ground to chassis

#define RADIUS_START 15 //15 for hook under 7 for hook over
#define HEIGHT_START 20 // 25 for hook under 15 for hook over
#define BASE_START 90

#define ELBOW_OFFSET 28 //angle between elbow physical and calculation 0
#define SHOULDER_OFFSET 14 //angle between shoulder physical and calculation 0

#define ELBOW_MAX_ANGLE 180
#define SHOULDER_MAX_ANGLE 180

int a1;
int a2;
int a3;

void coorMap(float r, float z, int baseAngle, int &shoulderAngle, int &elbowAngle)
{
	double barLength = 8.0 * 2.54;
	z -= (BASE_HEIGHT + CHASSIS_HEIGHT);

	double R = sqrt(pow(r, 2) + pow(z, 2));
	double beta = atan(z / r);
	double alpha = acos(R / (2 * barLength));
	double gamma = acos(1 - (pow(R, 2) / (2 * pow(barLength, 2)))) * (180.0 / PI);

	shoulderAngle = (int)((beta + alpha) * 180.0 / PI);
	elbowAngle = (int)(180.0 - gamma - shoulderAngle);
}

void goFastTo(float r, float z, int baseAngle)
{
	int shoulderAngle, elbowAngle;
	coorMap(r, z, baseAngle, shoulderAngle, elbowAngle);
	goFastToAngles(baseAngle, shoulderAngle, elbowAngle);
}

void goFastToAngles(int baseAngle, int shoulderAngle, int elbowAngle)
{
	if (175. - (shoulderAngle + elbowAngle) < 0) {
		return;
	}
	if ((shoulderAngle) > SHOULDER_MAX_ANGLE || (elbowAngle + ELBOW_OFFSET) > ELBOW_MAX_ANGLE) {
		return;
	}
	if ((baseAngle) > 170){
		return;
	}

	//if (baseAngle != a1) {
	s0.write(baseAngle + 10); //no conversion needed as 180 degree servo is used at base
	a1 = baseAngle;
	//}
	//if (shoulderAngle != a2) {
	s1.write(angleConv(shoulderAngle - SHOULDER_OFFSET));
	a2 = shoulderAngle;
	//}
	//if (elbowAngle != a3) {
	s2.write(angleConv(elbowAngle + ELBOW_OFFSET));
	a3 = elbowAngle;
	//}
}

void goSmoothTo(float r, float z, int b, int t)
{
	int angleIncrement = 1;

	int a1final = b;
	int a2final, a3final;
	coorMap(r, z, b, a2final, a3final); //sets a2final, a3final

	int a1step = a1;
	int a2step = a2;
	int a3step = a3;

	int diff1 = a1final - a1;
	int diff2 = a2final - a2;
	int diff3 = a3final - a3;

	//finds the servo that needs to turn the most, the number of degrees will be the number of steps
	int steps = abs(diff1);
	(steps < abs(diff2)) && (steps = abs(diff2)); //abuse && operator
	(steps < abs(diff3)) && (steps = abs(diff3));
	steps = steps / angleIncrement;
	int done1, done2, done3;

	do {
		diff1 = a1final - a1step;
		diff2 = a2final - a2step;
		diff3 = a3final - a3step;

		done1 = abs(diff1) < angleIncrement;  //less than half the maximum change
		done2 = abs(diff2) < angleIncrement;
		done3 = abs(diff3) < angleIncrement;

		if (!done1) {
			a1step += angleIncrement*(diff1 / abs(diff1));
		}
		if (!done2) {
			a2step += angleIncrement*(diff2 / abs(diff2));
		}
		if (!done3) {
			a3step += angleIncrement*(diff3 / abs(diff3));
		}

		if (!done1 || !done2 || !done3){
			goFastToAngles(a1step, a2step, a3step);
			delay(t / steps);
		}

	} while (!done1 || !done2 || !done3);

	goFastToAngles(a1final, a2final, a3final); //in case it was stopped a little too early
}


void startPosition() {
	goSmoothTo(RADIUS_START, HEIGHT_START, BASE_START, 2000);
}

void reset() {
	LCD.print("Set both arms");
	LCD.setCursor(0, 1);
	LCD.print("to lowest position");
	goFastToAngles(90, 0, 0);
}

int angleConv(int baseAngle){
	int newAngle;
	newAngle = baseAngle*180.0 / SERVO_RANGE;
	return newAngle;
}

void pushWire(){
	goSmoothTo(22, 17, 90, 1000);
	goSmoothTo(15, 17, 90, 1000);
	goSmoothTo(18, (20 + BASE_HEIGHT + CHASSIS_HEIGHT - 0.5), 90, 700);
}

void pickUpNum5(bool track)
{
	float r = track ? 18 : 14;
	float z = track ? (20 + BASE_HEIGHT + CHASSIS_HEIGHT - 0.5) : (20 + BASE_HEIGHT + CHASSIS_HEIGHT + 1);
	//goSmoothTo(r, 15, 90, 1500); changed 1245am
	goSmoothTo(r, z, 90, 700);
	goSmoothTo(r, z, 0, 700);
}

void store5(){
	goSmoothTo(18, (20 + BASE_HEIGHT + CHASSIS_HEIGHT - 0.5), 65, 1000);
	goSmoothTo(8, (20 + BASE_HEIGHT + CHASSIS_HEIGHT - 0.5), 65, 1500);
	goSmoothTo(8, 18, 65, 1500);
	goFastTo(8, 18, 160);
	//goSmoothTo(10, 18, 160, 500);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 1000);
}

void pickUpNum6(bool track){
	float r = 33;
	float z = track ? 17 : 16; //17 for Giraffe 16 for Penguin

	goSmoothTo(20.7, 30, 90, 1000);

	for (int i = 0; i < 2; i++)
	{
		goSmoothTo(r, z, 90, 500);
		goSmoothTo(r, z, 60, 500);
		goSmoothTo(r, z, 100, 500);
		goSmoothTo(r, z, 60, 500);
		goSmoothTo(r, z + 5 /*+ i*/, 90, 1000); //problem line. Dont make the time 500
		//z = z - 1;
		//r = r + 0.5;
	}

	goSmoothTo(29.6, 26, 90, 500);
	goSmoothTo(17.4, 40, 90, 1000);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 500); //added 1246am
	//goSmoothTo(10, 25, 90, 500); changed 1245am
}

void dropOffEdge(){
	goSmoothTo(20, 30, 90, 1000);
	goSmoothTo(20, 30, 0, 1000);
	goSmoothTo(30, 7, 0, 1000);
	goFastTo(9, 7, 0);
	goSmoothTo(30, 7, 0, 700);
	goFastTo(12, 7, 0);
	delay(500);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 500);
}

void pickUpNum3(){
	goSmoothTo(28, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	goSmoothTo(28, 13, 120, 1000);
	goSmoothTo(28, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	dropOffEdge();
}

void pickUpNum2(){
	goSmoothTo(25, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	goSmoothTo(28, 13, 120, 1000);
	goSmoothTo(25, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	store2();
	//goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 1000);
	//goSmoothTo(10, 25, 90, 1000);
}

void store2(){
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	goSmoothTo(10, (15 + BASE_HEIGHT + CHASSIS_HEIGHT), 120, 1000);
	goSmoothTo(10, 18, 120, 1000);
	goSmoothTo(10, 18, 20, 500);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 700);
}

void pickUpNum1(){
	goSmoothTo(25, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 100, 1000);
	goSmoothTo(28, 13, 100, 1000);
	goSmoothTo(25, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 100, 1000);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 1000);
	goSmoothTo(10, 25, 90, 1000);
}

void throwPet(){
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 90, 1500);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 140, 1500);
	goSmoothTo(20, (20 + BASE_HEIGHT + CHASSIS_HEIGHT), 0, 100);
	delay(500);
}

void petPickUp(int i)
{
	if (i == 1) { pickUpNum1(); }
	else if (i == 2) { pickUpNum2(); }
	else if (i == 3) { pickUpNum3(); }
	else if (i == 4) { pushWire(); }
	else if (i == 5) { pickUpNum5(1); }
	else if (i == 6) { pickUpNum6(1); }
	else if (i == 7) { store5(); }
	else if (i == 8) { store2(); }
	else if (i == 9) { throwPet(); }
	else if (i == 10) { dropOffEdge(); }
}