// Pins
/* Analog */
#define LEFT_QRD 0
#define RIGHT_QRD 1
#define SIDE_QRD 2
#define LEFT_IR 3
#define RIGHT_IR 4

/* Motors */
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1


// Drive System Constants
uint16_t speed0 = menuItems[SPEED0_IDX].Value;
uint16_t speedIR = menuItems[IR_SPEED_IDX].Value;
uint16_t speedBack = menuItems[SPEEDB_IDX].Value;

uint16_t kp = menuItems[KP_IDX].Value; //68
uint16_t kd = menuItems[KD_IDX].Value; //82
uint16_t kI = menuItems[KI_IDX].Value;
uint16_t I_MAX = menuItems[IMAX_IDX].Value;


//float kp = knob(6) / 1023.0 * 500;
//float kd = knob(7) / 1023.0 * 500;


// Tape Following Thresholds
uint16_t THRESH_L = menuItems[L_THRESH_IDX].Value;  //350
uint16_t THRESH_R = menuItems[R_THRESH_IDX].Value;  //300
uint16_t THRESH_S = menuItems[S_THRESH_IDX].Value;  //300

// UpHill Constants (Wheel Encoders)
const uint16_t MAX_COUNT = 50;

// IR Constants
const uint16_t diffMaxIR = 1;
