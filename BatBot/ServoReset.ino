#define ALPHA 40 //angle between elbow physical and calculation 0
#define BETA 5 //angle between shoulder physical and calculation 0

void setup() {
  LCD.print("Shoulder horizontal");
  LCD.setCursor(0,1);
  LCD.print("Elbow lowest");
  goFastToAngles(90,180-BETA,0);
}
