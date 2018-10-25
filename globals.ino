// All globals are declared in this file

// =============================== Pins ===========================================
const short Trig = A4;            // output trigger pin for ultrasound (sends sound)
const short Echo = A5;            // input echo pin for ultrasound (gets echo back)

// motor pins
const short Left_motor_back = 9;  // left motor back
const short Left_motor_go = 5;    // left motor forwards
const short Right_motor_go = 6;   // right motor forwards
const short Right_motor_back = 10;// right motor back

const short key = 2;              // Button

const short SensorRight = A2;     // Right bottom mounted IR sensor (P3.2 OUT1)
const short SensorLeft = A3;      // Left bottom mounted IR sensor(P3.3 OUT2)

const int FrontSensorLeft = A0;   // Left front mounted IR sensor(P3.4 OUT3)
const int FrontSensorRight = A1;  // Right front mounted IR sensor(P3.5 OUT4)

// ============================== Globals ==========================================
const int RunSpeed = 100;                  // analog motor speed

LiquidCrystal lcd(3, 4, 7, 8, 11, 12, 13); // LCD and pins

/* 2D Matrix holding the maze information, including each grid cell STATUS
  The cells can be defined as STATUS types START, FINISH, OPEN, CLOSED, or PATH.
  NUMROWS and NUMCOLS are both +2 so we can frame the outside with CLOSED cells.
  Note that numRows and numCols are defined in maze_algorithm.h */
STATUS maze[NUMROWS + 2][NUMCOLS + 2];

// Keep track of which direction our robot is facing, for mapping and navigation
DIR dir = SOUTH;

// The robot's current position in the maze (row, col)
int curRow = 1;
int curCol = 1;

