// Tape Following Constants

int16_t lerr = 0;
int16_t recerr = 0;
int16_t q = 0;
int16_t m = 0;
int16_t c;
uint16_t speed0 = menuItems[0].Value;
uint16_t speedIR = menuItems[7].Value;
int16_t I_MAX = menuItems[5].Value;
//float kp = knob(6) / 1023.0 * 500;
//float kd = knob(7) / 1023.0 * 500;

// Motors:
uint16_t LEFT_MOTOR = 0;
uint16_t RIGHT_MOTOR = 1;

// Tape Following:

uint16_t THRESH = menuItems[6].Value;
uint16_t THRESH_L = THRESH + 10;
uint16_t kp = menuItems[1].Value;
uint16_t kd = menuItems[2].Value;