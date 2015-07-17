void coorMap(float r, float z, float baseAngle, float &shoulderAngle, float &elbowAngle)
{
	float barLength = 8.*2.54;

	float R = sqrt(pow(r, 2) + pow(z,2));
	float beta = atan(z / r);
	float alpha = acos(R / (2 * barLength));
	float gamma = acos(1 - (pow(R,2) / (2 * pow(barLength,2))));

	shoulderAngle = (beta + alpha)*180./PI;
	elbowAngle = (180.0 - (gamma + shoulderAngle)) * 180.0 / PI;
	//return true;
}

void goFastTo(float r, float z, float baseAngle)
{
	float heightOfBase; // add in height of base
	z -= heightOfBase;

	float shoulderAngle, elbowAngle;
	coorMap(r, z, baseAngle, shoulderAngle, elbowAngle);

	if (175. - (shoulderAngle + elbowAngle) >= 0)
	{
		RCServo0.write(baseAngle);
		RCServo1.write(shoulderAngle);
		RCServo2.write(elbowAngle);
	}
}

void goSmoothTo(float r, float z, float baseAngle, uint16_t maxSteps)
{
	float percent;

	for (uint16_t i = 1; i <= maxSteps; i++){
		percent = tween(i, maxSteps);
		goFastTo(percent*r, percent*z, percent*baseAngle);
		delay(100);
	}
}

float tween(uint16_t currentStep, uint16_t maxSteps)
{
	float t = currentStep / (maxSteps / 2);
	if (t < 1) return (t*t*t) / 2;
	else t -= 2;
	return (t*t*t + 2) / 2;
}
