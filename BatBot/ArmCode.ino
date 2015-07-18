#define PI 3.14159
#define SERVO_RANGE 229 //do not touch, calibration for new servo

//origin is determined to be between the two servos
#define BASE_HEIGHT 9.5 //base of the arm (Will's mulitple layers)
#define SIDE_LENGTH 13.2 //length from middle to side of the robot
#define FRONT_LENGTH 0 //length from base of arm to front of robot
#define SIDE_HEIGHT 5 //height of the side walls of the robot
#define DOLL_HEIGHT 14.242 
#define CHASSIS_HEIGHT 10.5 //height from ground to chassis

#define RADIUS_START 20
#define HEIGHT_START (20)// + BASE_HEIGHT + CHASSIS_HEIGHT)
#define BASE_START 90

#define ELBOW_OFFSET 50 //angle between elbow physical and calculation 0
#define SHOULDER_OFFSET 10 //angle between shoulder physical and calculation 0

#define ELBOW_MAX_ANGLE 180
#define SHOULDER_MAX_ANGLE 180

double a1;
double a2;
double a3;

void coorMap(float r, float z, int baseAngle, int &shoulderAngle, int &elbowAngle)
{
  double barLength = 8.0 * 2.54;
  z -= (BASE_HEIGHT + CHASSIS_HEIGHT);

  double R = sqrt(pow(r, 2) + pow(z, 2));
  double beta = atan(z / r);
  double alpha = acos(R / (2 * barLength));
  double gamma = acos(1 - (pow(R, 2) / (2 * pow(barLength, 2)))) * (180.0 / PI);

  shoulderAngle = (int) (beta + alpha) * 180.0 / PI;
  elbowAngle = (int) (180.0 - gamma - shoulderAngle);
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

  if (baseAngle != a1) {
    RCServo0.write(baseAngle); //no conversion needed as 180 degree servo is used at base
    a1 = baseAngle;
  }
  if (shoulderAngle != a2) {
    RCServo1.write(angleConv(shoulderAngle - SHOULDER_OFFSET));
    a2 = shoulderAngle;
  }
  if (elbowAngle != a3) {
    RCServo2.write(angleConv(elbowAngle + ELBOW_OFFSET));
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

void goSmoothTo(float r, float z, int b, float t)
{

  int a1final = b;
  int a2final, a3final;
  coorMap(r, z, b, a2final, a3final); //sets a2final, a3final

  int a1step = a1;
  int a2step = a2;
  int a3step = a3;

  int rDiff = a1final - a1;
  int zDiff = a2final - a2;
  int bDiff = a3final - a3;

  //finds the servo that needs to turn the most, the number of degrees will be the number of steps
  int steps = abs(rDiff);
  (steps < abs(zDiff)) && (steps = abs(zDiff)); //abuse && statements
  (steps < abs(bDiff)) && (steps = abs(bDiff));

  bool rDone, zDone, bDone;

  do {
    rDiff = a1final - a1step;
    zDiff = a2final - a2step;
    bDiff = a3final - a3step;

    rDone = abs(rDiff) <= 0.5;  //less than half the maximum change
    zDone = abs(zDiff) <= 0.5;
    bDone = abs(bDiff) <= 0.5;

    if (!rDone) {
      a1step += rDiff / abs(rDiff);
    }
    if (!zDone) {
      a2step += zDiff / abs(zDiff);
    }
    if (!bDone) {
      a3step += bDiff / abs(bDiff);
    }

    goFastToAngles(a1step, a2step, a3step);
    delay(t / steps);

  } while (!rDone || !zDone || !bDone);
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

void reset() {
  LCD.print("Set both arms");
  LCD.setCursor(0,1);
  LCD.print("to lowest position");
  goFastToAngles(90,0,0);
}

int angleConv(int baseAngle){
  int newAngle;
  newAngle = baseAngle*180.0/SERVO_RANGE;
  return newAngle;
}

