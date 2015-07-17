/*
 Name:		BatBot.ino
 Created:	7/17/2015 11:35:42 AM
 Author:	Shahriar
*/


#include <phys253.h>          
#include <LiquidCrystal.h>  
#include <avr/EEPROM.h>

volatile int16_t tapeFlag1 = 4;

ISR(INT0_vect) { LCD.clear(); LCD.home(); LCD.print("All Motors Stopping:"); motor.stop_all(); while (!(startbutton())){} };
ISR(INT1_vect)
{
	bool pressed = 1;
	for (int i = 0; i < 5; i++)
	{
		pressed &= !digitalRead(1);
	}

	if (pressed) { motor.stop_all(); delay((2000)); }
	//tapeFlag1; 
};

/*  Enables an external interrupt pin
INTX: Which interrupt should be configured?
INT0    - will trigger ISR(INT0_vect)
INT1    - will trigger ISR(INT1_vect)
INT2    - will trigger ISR(INT2_vect)
INT3    - will trigger ISR(INT3_vect)
mode: Which pin state should trigger the interrupt?
LOW     - trigger whenever pin state is LOW
FALLING - trigger when pin state changes from HIGH to LOW
RISING  - trigger when pin state changes from LOW  to HIGH
*/
void enableExternalInterrupt(unsigned int INTX, unsigned int mode)
{
	if (INTX > 3 || mode > 3 || mode == 1) return;
	cli();
	/* Allow pin to trigger interrupts        */
	EIMSK |= (1 << INTX);
	/* Clear the interrupt configuration bits */
	EICRA &= ~(1 << (INTX * 2 + 0));
	EICRA &= ~(1 << (INTX * 2 + 1));
	/* Set new interrupt configuration bits   */
	EICRA |= mode << (INTX * 2);
	sei();
}


void setup()
{
#include <phys253setup.txt>
	Serial.begin(9600);
	LCD.clear(); LCD.home();
	LCD.print("BatBot says HI!");

	// Menu Setup
	uint16_t debugMode = MenuSetup();
	LCD.clear();
	LCD.home();				LCD.print("BATBot");
	LCD.setCursor(0, 1);	LCD.print("AShLAW Product");
	delay(1000);
	enableExternalInterrupt(INT0, FALLING);
	enableExternalInterrupt(INT1, RISING);
	if (debugMode > 0) speedTest();
}

void loop()
{
	tapeFollow2QRD(&tapeFlag1);
	while (tapeFlag1 <= 0) { LCD.clear(); LCD.home(); LCD.print("GoodBye!!"); };
	//IRfollow();
	//park();
	//int val = analogRead(0);
	//Serial.println(val);
	//LCD.clear(); LCD.home(); LCD.print(val);
	//delay(500);
}

//int target_angle = 180;
//unsigned long last_update_time_ms = 0;
//unsigned int servo_update_delay_ms = 10;

/*
void move_servo_to_target()
{
RCServo0.write(target_angle);
last_update_time_ms = millis();
}

void loop()
{
if (millis() > last_update_time_ms + servo_update_delay_ms)
{
if (target_angle)
{
target_angle--;
move_servo_to_target();
}
}


}


*/
