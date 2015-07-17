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
MenuItem ProportionalGain = MenuItem("kP");
MenuItem DerivativeGain = MenuItem("kD");
MenuItem SpeetTestMode = MenuItem("Speed Test");
MenuItem IntegralGain = MenuItem("kI");
MenuItem IntegralMax = MenuItem("I_MAX");
MenuItem Threshold = MenuItem("Threshold");
MenuItem IRSpeed = MenuItem("IR-Speed");
MenuItem menuItems[] = { Speed, ProportionalGain, DerivativeGain, SpeetTestMode, IntegralGain, IntegralMax, Threshold, IRSpeed };

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
			return menuItems[3].Value;
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
				return menuItems[3].Value;
			}
		}
	}
}