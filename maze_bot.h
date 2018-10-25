#ifndef MAZE_BOT_H_
#define MAZE_BOT_H_

#define NUMROWS 4
#define NUMCOLS 4

//STATUS represents the type of grid cell of each part of the maze
enum STATUS {OPEN, CLOSED, PATH, START, FINISH};
//DIR is short for "direction" - This is used to navigate the maze
enum DIR {NORTH, EAST, SOUTH, WEST};

// ========================== Function Prototypes =============================
//  main_algorithm
bool findPath();       // solves maze recursively
void printMaze();      // prints maze to serial monitor on computer

// movement
void run();            // start the car going forwards
void brake(int time);  // stop the car for TIME seconds
void left();           // set the right motor to go (turn left)
void right();          // set the left motor to go (turn right)
void spin_left();      // set car to spin left (L motor back, R motor forward)
void spin_right();     // set car to spin right (R motor back, L motor forward)

// sensors
bool keyscan();                      // get button input from user
void Distance_display(int Distance); // display distance on LCD for the user
float Distance_test();               // test the front distance to objects
bool checkFinish();                  // check if the finish is reached
void turn_left();      // turn 90 degress left based on line following
void moveForward();    // move forwards until the next intersection (next cell)
void backtrack();      // backtrack to the last available path

// =============================== Pins =======================================
extern const short Trig; // output trigger pin for ultrasound (sends sound)
extern const short Echo; // input echo pin for ultrasound(echo from sound sent)

// motor pins
extern const short Left_motor_back;  // left motor back
extern const short Left_motor_go;    // left motor forwards
extern const short Right_motor_go;   // right motor forwards
extern const short Right_motor_back; // right motor back

extern const short key;              // Button
extern const short beep;             // Speaker

extern const short SensorRight;      // R bottom mounted IR sensor (P3.2 OUT1)
extern const short SensorLeft;       // L bottom mounted IR sensor (P3.3 OUT2)

extern const int FrontSensorRight;   // R front mounted IR sensor (P3.5 OUT4)
extern const int FrontSensorLeft;    // L front mounted IR sensor (P3.4 OUT3)

// ============================== Globals =====================================
extern const int RunSpeed;          // analog motor speed
extern LiquidCrystal lcd;           // LCD and pins

/* 2D Matrix holding the maze information, including each grid cell STATUS
  The cells can be defined as STATUS types START, FINISH, OPEN, CLOSED, or PATH.
  NUMROWS and NUMCOLS are both +2 so we can frame the outside with CLOSED cells*/
extern STATUS maze[NUMROWS + 2][NUMROWS + 2];

// Keep track of which direction our robot is facing
extern DIR dir;

//The robot's current position (row and col) in the maze
extern int curRow;
extern int curCol;
#endif
