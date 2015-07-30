/*
 Name:		BatBot.ino
 Created:	7/17/2015 11:35:42 AM
 Author:	Shahriar
*/


#include <phys253.h>          
#include <LiquidCrystal.h>  
#include <avr/EEPROM.h>
#include <Servo.h>

#define SERVO_0_PIN (35)
#define SERVO_1_PIN (31)
#define SERVO_2_PIN (34)

Servo s0;
Servo s1;
Servo s2;

volatile uint16_t leftCount = 0;
volatile uint16_t rightCount = 0;

/* Stop Button: Resets Wheel Encoder Values */
ISR(INT0_vect) 
{ 
	LCD.clear(); LCD.home(); LCD.print("Motors Stopped!"); 
	motor.stop_all(); 
	//disableExternalInterrupt(INT1); disableExternalInterrupt(INT2);
	LCD.setCursor(0, 1);
	//LCD.print(digitalRead(1)); LCD.print("  "); LCD.print(digitalRead(2));
	leftCount = 0; rightCount = 0; 
	while (!(startbutton()))
	{ 
		LCD.clear(); LCD.home();
		LCD.print(analogRead(0)); /* LEFT QRD  */ LCD.print("   ");
		LCD.print(analogRead(1)); /* RIGHT QRD */ LCD.print("   ");
		LCD.print(analogRead(2)); /* SIDE QRD */ LCD.print("   ");

		LCD.setCursor(0, 1);
		LCD.print(analogRead(3)); /* LEFT IR  */ LCD.print("     ");
		LCD.print(analogRead(4)); /* RIGHT IR */ LCD.print("     ");
		//LCD.setCursor(0, 1); 
		//LCD.print(leftCount); LCD.print("  ");  LCD.print(rightCount);
		// LCD.print(analogRead(4)); LCD.print("  "); LCD.print(analogRead(5));
	}
	//enableExternalInterrupt(INT1, RISING); 
	//enableExternalInterrupt(INT2, RISING);
	leftCount = 0; rightCount = 0;
};

/* Wheel Encoders*/
#define LEFT_ENCODER 1
#define RIGHT_ENCODER 2
//
//volatile unsigned long lastRightCountMs_L = 0;
//volatile unsigned long lastRightCountMs_R = 0;
//volatile unsigned int delayCount = 0xFFFFU;
//const unsigned int MIN_SWITCH_DELAY_MS = 50;
//const unsigned int MIN_HOLD_TIME_MS = 10;
//
//ISR(INT1_vect) 
//{ 
//	if (millis() > lastRightCountMs_L + MIN_SWITCH_DELAY_MS)
//	{
//		leftCount++;
//		lastRightCountMs_L = millis();
//	}
//};
volatile int x = 0;
ISR(INT1_vect)
{
	digitalWrite(8, !digitalRead(8));
	leftCount++;
};
ISR(INT2_vect)
{
	digitalWrite(9, !digitalRead(9));
	rightCount++;
		//x++;
};

//ISR(INT2_vect)
//{
//	if (millis() > lastRightCountMs_R + MIN_SWITCH_DELAY_MS)
//	{
//		volatile unsigned int count = 0;
//		while (count) count--;
//        
//		count = 0;
//		while (count)
//		{
//			if (digitalRead(2)) return;
//			count--;
//		}
//		digitalWrite(8, !digitalRead(8));
//		rightCount++;
//		lastRightCountMs_R = millis();
//	}
//};


// while (!(digitalRead(LEFT_ENCODER))){} };


////
//ISR(INT2_vect)
//{
//	if (millis() > lastRightCountMs_R + MIN_SWITCH_DELAY_MS)
//	{
//		rightCount++;
//		lastRightCountMs_R = millis();
//	}
//};

//ISR(INT2_vect) 
//{ 
//	uint16_t startTime = millis();
//	while ((millis() - startTime) < 50) {}
//	//while (!(digitalRead(3))){}
//	rightCount++;
//};

volatile int16_t tapeFlag1 = 4;
/*ISR(INT1_vect)
{
	bool pressed = 1;
	for (int i = 0; i < 5; i++)
	{
		pressed &= !digitalRead(1);
	}
	
	if (pressed) 
	{ 
		//motor.stop_all();

		delay(5000);

		if (tapeFlag1 > 0) { tapeFlag1--; }
	}

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

void disableExternalInterrupt(unsigned int INTX)
{
	if (INTX > 3) return;
	EIMSK &= ~(1 << INTX);
}


void setup()
{
	#include <phys253setup.txt>
	Serial.begin(9600);
	
	pinMode(SERVO_0_PIN, OUTPUT);
	pinMode(SERVO_1_PIN, OUTPUT);
	pinMode(SERVO_2_PIN, OUTPUT);


	LCD.clear(); LCD.home();
	LCD.print("BatBot says HI!");
	delay(1000);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);

	// Arm Initialization
	startPosition();
	s0.attach(SERVO_0_PIN);
	s1.attach(SERVO_1_PIN);
	s2.attach(SERVO_2_PIN);

	// Menu Setup
	uint16_t debugMode = MenuSetup();
	LCD.clear(); LCD.home();	LCD.print("BATBot");
	LCD.setCursor(0, 1);		LCD.print("AShLAW Product");
	delay(500);

	enableExternalInterrupt(INT0, FALLING); // Stop Button
	//enableExternalInterrupt(INT1, RISING); // Left  Wheel Encoder
	//enableExternalInterrupt(INT2, RISING); // Right Wheel Encoder

	if (debugMode > 0) { speedTest1(); }
	//if (debugMode > 0) { speedTest2(); }
	//if ((debugMode > 0) && (debugMode < 100)) { speedTest(); }
	//else if (debugMode >= 100) { tapeFollowTest(); }
}

// Arm
//void loop()
//{
//	petPickUp();
//}

// Drive System
void loop()
{
	tapeFollow1();
	fixation(600, 500, 2000);
	IRfollow1(4000);

//	/*pickUpNum6();
//	while (!stopbutton())
//	{
//
//	}*/

	goBack();
	turn();
	IRfollow2();
	tapeFollow2();
	LCD.clear(); LCD.home();
	LCD.print("DONE");
	while (true) {}
//	//LCD.print(x);
//	//delay(100);
	//IRfollow();
	//tapeFollow2();
}