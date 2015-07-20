/*
 Name:		BatBot.ino
 Created:	7/17/2015 11:35:42 AM
 Author:	Shahriar
*/


#include <phys253.h>          
#include <LiquidCrystal.h>  
#include <avr/EEPROM.h>


volatile uint16_t leftCount = 0;
volatile uint16_t rightCount = 0;

ISR(INT0_vect) 
{ 
	LCD.clear(); LCD.home(); LCD.print("All Motors Stopping:"); 
	motor.stop_all(); 
	disableExternalInterrupt(INT1); disableExternalInterrupt(INT2);
	leftCount = 0; rightCount = 0; 
	while (!(startbutton()))
	{ 
		LCD.setCursor(0, 1); 
		LCD.print(leftCount); LCD.print("  ");  LCD.print(rightCount);
	}
};

/* Wheel Encoders*/
ISR(INT1_vect) { leftCount++;  };
ISR(INT2_vect) { rightCount++; };


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
	
	LCD.clear(); LCD.home();
	LCD.print("BatBot says HI!");
	delay(1000);

	// Menu Setup
	uint16_t debugMode = MenuSetup();
	LCD.clear(); LCD.home();	LCD.print("BATBot");
	LCD.setCursor(0, 1);		LCD.print("AShLAW Product");
	delay(1000);

	enableExternalInterrupt(INT0, FALLING); // Stop Button
	enableExternalInterrupt(INT1, FALLING); // Left  Wheel Encoder
	enableExternalInterrupt(INT2, FALLING); // Right Wheel Encoder

	if ((debugMode > 0) && (debugMode < 100)) { speedTest(); }
	else if (debugMode >= 100) { tapeFollowTest(); }
}

void loop()
{
	tapeFollow1();

	//IRfollow();
	//tapeFollow2();
}