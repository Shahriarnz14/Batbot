void setup() {
  LCD.print("Set both arms");
  LCD.setCursor(0,1);
  LCD.print("to lowest position");
  goFastToAngles(90,0,0);
}
