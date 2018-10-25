/*****************************************************************************
 *  Teacher:         Mr. Creelman                                            *
 *  Name:            Charles Zhang and Miasya Bulger                         *
 *  Program Title:   Maze Solving Algorithm for Arduino Robot Using DFS      *
 *  Course:          ICS4U                                                   *
 *  Date:            January 12, 2016                                        *
 *                                                                           *
 *  Description:     Program makes a robot solve a 4x4 physical maze using   *
 *                   DFS, line following, and sensors. Maps maze at end.     *
 *                                                                           *
 *  Usage:           Demonstrate Arduino robot capabilities and code for DFS *
 *                                                                           *
 * Revision History: DFS concept learned earlier on during the semester.     *
 *                   Manufacturers code referenced for operating hardware    *
 *                                                                           *
 *  Known Issues:    Maze must be set up properly for robot to solve maze.   *
 *                   (See user guide for instructions of how to set up)      *
 *                                                                           *
 *****************************************************************************/

#include <LiquidCrystal.h>
#include "maze_bot.h"

// The setup function runs once before the loop (main) function to set up!
void setup() {
  // Begin serial communication with the computer
  Serial.begin(9600);

  // Initialize pins to input/output for all sensors and motors
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  pinMode(key, INPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(SensorRight, INPUT);
  pinMode(SensorLeft, INPUT);
  pinMode(FrontSensorRight, INPUT);
  pinMode(FrontSensorLeft, INPUT);
  lcd.begin(16, 2);         // start LCD

  /* NUMROWS and NUMCOLS are both +2 so we can frame the outside with CLOSED cells.
    Note that numRows and numCols are defined in maze_algorithm.h
    The maze borders will be CLOSED so the robot doesn't move off the board.
    We will make the entire maze CLOSED then make the inner maze OPEN next loop */
  for (int i = 0; i < NUMROWS + 2; i++)
    for (int j = 0; j < NUMCOLS + 2; j++)
      maze[i][j] = CLOSED;

  // Set inner maze cell types to OPEN (all but the othermost frame which we want CLOSED)
  for (int i = 1; i < NUMROWS + 1; i++)
    for (int j = 1; j < NUMCOLS + 1; j++)
      maze[i][j] = OPEN;

}

// The loop function in Arduino is the same thing as the main function in C++, except it repeats
void loop() {

  // Output helpful LCD messages to improve user experience and to instruct user on how to start
  lcd.home();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PRESS START");
  lcd.setCursor(3, 2);
  lcd.print("TO BEGIN");

  // Check for beginning key input, we don't begin until the user hits the START button
  while (!keyscan());

  // Call the maze solving algorithm. Time to let the magic happen!
  findPath();

  // Reset variables to reset the program
  dir = SOUTH;
  curRow = 1;
  curCol = 1;
  
  // reset maze
   for (int i = 0; i < NUMROWS + 2; i++)
    for (int j = 0; j < NUMCOLS + 2; j++)
      maze[i][j] = CLOSED;
      
  for (int i = 1; i < NUMROWS + 1; i++)
    for (int j = 1; j < NUMCOLS + 1; j++)
      maze[i][j] = OPEN;
  
}
