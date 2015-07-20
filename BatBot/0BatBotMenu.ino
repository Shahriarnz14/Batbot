class MenuItem
{
public:
	String    Name;
	uint16_t  Value;
	uint16_t* EEPROMAddress;
	static uint16_t MenuItemCount;
	MenuItem(String name)
	{
		MenuItemCount++;
		EEPROMAddress = (uint16_t*)(2 * MenuItemCount);
		Name = name;
		Value = eeprom_read_word(EEPROMAddress);
	}
	void Save()
	{
		eeprom_write_word(EEPROMAddress, Value);
	}
};

uint16_t MenuItem::MenuItemCount = 0;
/* Add the menu items here */
MenuItem Speed = MenuItem("Speed");
MenuItem IRSpeed = MenuItem("IR-Speed");
MenuItem SpeedBack = MenuItem("Speed-Back");
MenuItem SpeetTestMode = MenuItem("Speed Test");

MenuItem ProportionalGain = MenuItem("kP");
MenuItem DerivativeGain = MenuItem("kD");
MenuItem IntegralGain = MenuItem("kI");
MenuItem IntegralMax = MenuItem("I_MAX");

MenuItem LeftThreshold = MenuItem("L-Threshold");
MenuItem RightThreshold = MenuItem("R-Threshold");
MenuItem SideThreshold = MenuItem("S-Threshold");

MenuItem menuItems[] = { Speed, IRSpeed, SpeedBack, SpeetTestMode,
							ProportionalGain, DerivativeGain, IntegralGain, IntegralMax,
								LeftThreshold, RightThreshold, SideThreshold };

/* Index Definition To Be Used */
#define SPEED0_IDX 0
#define IR_SPEED_IDX 1
#define SPEEDB_IDX 2
#define SPEED_TEST_IDX 3

#define KP_IDX 4
#define KD_IDX 5
#define KI_IDX 6
#define IMAX_IDX 7

#define L_THRESH_IDX 8
#define R_THRESH_IDX 9
#define S_THRESH_IDX 10



uint16_t MenuSetup()
{
	/* Menu Options */
	while (true)
	{
		LCD.clear();
		LCD.home();				LCD.print("Press Start");
		LCD.setCursor(0, 1);	LCD.print("To Open Menu");
		delay(100);
		if (startbutton())
		{
			delay(100);
			if (startbutton()) return Menu();
			return menuItems[SPEED_TEST_IDX].Value;
		}
	}
}


/*
*	Menu Functionality to change Speed, Proportional Gain (kP) and Derivative Gain (kD).
*/
uint16_t Menu()
{
	LCD.clear();
	LCD.home();		LCD.print("Entering Menu");
	delay(500);

	while (true)
	{
		/* Show MenuItem value and knob value */
		int menuIndex = knob(6) * (MenuItem::MenuItemCount) / 1024;

		// Display Current Value
		LCD.clear(); LCD.home();
		LCD.print(menuItems[menuIndex].Name); LCD.print(" "); LCD.print(menuItems[menuIndex].Value);

		// Display Value To Change To
		LCD.setCursor(0, 1); LCD.print("Set To: ");
		LCD.print(knob(7));
		delay(100);

		/* Press Start Button To Save the New Value */
		if (startbutton())
		{
			delay(100);
			if (startbutton())
			{
				menuItems[menuIndex].Value = knob(7);
				menuItems[menuIndex].Save();
				delay(250);
			}
		}

		/* Press Stop Button to Exit Menu */
		if (stopbutton())
		{
			delay(100);
			if (stopbutton())
			{
				LCD.clear(); LCD.home();
				LCD.print("BatBot Menu: 'Good Bye!'");
				delay(1000);
				return menuItems[SPEED_TEST_IDX].Value;
			}
		}
	}
}