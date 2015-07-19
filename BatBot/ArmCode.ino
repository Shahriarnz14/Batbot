#define PI 3.14159
#define SERVO_RANGE 240

#define BASE_HEIGHT 9.5
#define SIDE_LENGTH 13.2
#define FRONT_LENGTH 0
#define SIDE_HEIGHT 5
#define DOLL_HEIGHT 14.242
#define CHASSIS_HEIGHT 10.5

#define RADIUS_START 20
#define HEIGHT_START (20)// + BASE_HEIGHT + CHASSIS_HEIGHT)
#define BASE_START 90

double a1;
double a2;
double a3;

void coorMap(float r, float z, float baseAngle, float &shoulderAngle, float &elbowAngle)
{
  double barLength = 8.0 * 2.54;
  //float heightOfBase = 0 + 95.7; // add in height of base
  //z -= (BASE_HEIGHT + CHASSIS_HEIGHT);

  double R = sqrt(pow(r, 2) + pow(z, 2));
  double beta = atan(z / r);
  double alpha = acos(R / (2 * barLength));
  double gamma = acos(1 - (pow(R, 2) / (2 * pow(barLength, 2)))) * (180.0 / PI);

  shoulderAngle = (beta + alpha) * 180.0 / PI;
  elbowAngle = (180.0 - gamma - shoulderAngle) + 18.0; //40 is the offset of elbow, defined 0 is parallel to ground
  //return true
}

void goFastTo(float r, float z, float baseAngle)
{
  float shoulderAngle, elbowAngle;
  coorMap(r, z, baseAngle, shoulderAngle, elbowAngle);
  goFastToAngles(baseAngle, shoulderAngle, elbowAngle);
}

void goFastToAngles(float baseAngle, float shoulderAngle, float elbowAngle)
{
  if (175. - (shoulderAngle + elbowAngle) < 0) {
    return;
  }

  if (baseAngle != a1) {
    RCServo0.write(angleConv(baseAngle));
    a1 = baseAngle;
  }
  if (shoulderAngle != a2) {
    RCServo1.write(angleConv(shoulderAngle));
    a2 = shoulderAngle;
  }
  if (elbowAngle != a3) {
    RCServo2.write(angleConv(elbowAngle));
    a3 = elbowAngle;
  }
}

float tween(int currentStep, int maxSteps)
{
  float t = currentStep / (maxSteps / 2.0);
  if (t < 1) return (t * t * t) / 2;
  else t -= 2;
  return (t * t * t + 2) / 2;
}

float goLinear(int currentStep, int maxSteps) {
  return ((double) currentStep / (double)maxSteps);
}

void goSmoothTo(int r, int z, int b, int t)
{	
  int angleIncrement = 5;
	
  float a1final = b;
  float a2final, a3final;
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
  steps = steps/angleIncrement;
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
	
  goFastToAngles(a1final,a2final,a3final); //in case it was stopped a little too early
}


void startPosition() {
  goFastTo(RADIUS_START, HEIGHT_START, BASE_START);
}

void pickUpRight() {
  goSmoothTo(SIDE_LENGTH + 15.0, DOLL_HEIGHT, 0, 2000);
  delay(500);
  goSmoothTo(SIDE_LENGTH + 15.0, CHASSIS_HEIGHT + DOLL_HEIGHT + SIDE_HEIGHT + 5, 0, 2000);
  delay(500);
  goSmoothTo(SIDE_LENGTH + 10.0, CHASSIS_HEIGHT + DOLL_HEIGHT, 90, 2000);
  delay(500);
  goFastTo(SIDE_LENGTH + 10.0, CHASSIS_HEIGHT + DOLL_HEIGHT, 0);
  delay(500);
  startPosition();
}

void dance ()
{
  int i = 0;
  while (true) {
    if (i % 2 == 0) {
      goFastTo(15, 40, 180 - 30 * abs(i - 6));
    } else {
      goFastTo(20, 45, 180 - 30 * abs(i - 6));
    }
    delay(950); //Pat and Mat

    i++;
    if (i = 12) {
      i = 0;
    }
  }
}

float angleConv(float baseAngle){
  float newAngle;
  newAngle = baseAngle*180.0/SERVO_RANGE;
  return newAngle;
}

