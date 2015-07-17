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

#define ELBOW_OFFSET 40 //angle between elbow physical and calculation 0
#define SHOULDER_OFFSET 0 //angle between shoulder physical and calculation 0

#define ELBOW_MAX_ANGLE 90
#define SHOULDER_MAX_ANGLE 180

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
  if (shoulderAngle > SHOULDER_MAX_ANGLE || elbowAngle > ELBOW_MAX_ANGLE) {
    return; 
  }

  if (baseAngle != a1) {
    RCServo0.write(angleConv(baseAngle));
    a1 = baseAngle;
  }
  if (shoulderAngle != a2) {
    RCServo1.write(angleConv(shoulderAngle+SHOULDER_OFFSET));
    a2 = shoulderAngle;
  }
  if (elbowAngle != a3) {
    RCServo2.write(angleConv(elbowAngle-ELBOW_OFFSET));
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

void goSmoothTo(float r, float z, float b, float t)
{

  float a1final = b;
  float a2final, a3final;
  coorMap(r, z, b, a2final, a3final); //sets a2final, a3final

  float a1step = a1;
  float a2step = a2;
  float a3step = a3;

  float rDiff = a1final - a1;
  float zDiff = a2final - a2;
  float bDiff = a3final - a3;

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

float angleConv(float baseAngle){
  float newAngle;
  newAngle = baseAngle*180.0/SERVO_RANGE;
  return newAngle;
}

