// This file includes functions for the maze solving algorithm

// Maze solving recursive algorithm, returns false if the maze is unsolvable
// A complex version of the Depth First Search (DFS) concept
bool findPath() {

  // This keeps track of how many turns we do on a spot, and we know that
  // if we turn all the way around and don't see an OPEN spot, we know to backtrack
  static int rotations = 0;

  maze[curRow][curCol] = PATH;  // Mark the cell we are on as part of the path

  // We set the cell we are checking to our current cell, then increment either
  // checkRow or checkCol based on which adjacent cell we are directly facing
  int checkRow = curRow;
  int checkCol = curCol;
  switch (dir) {
    case NORTH:
      checkRow--;
      break;
    case EAST:
      checkCol++;
      break;
    case SOUTH:
      checkRow++;
      break;
    case WEST:
      checkCol--;
      break;
  }
  delay(200); // for accuracy

  if (checkFinish()) {    // See if we've reached the goal or not
    lcd.home();   // Give user a congratulatory message if it's solved
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("MAZE COMPLETE");
    lcd.setCursor(0, 2);
    lcd.print("PLUG IN & PRESS");

    // Mark the START and FINISH cells to output to monitor later
    maze[1][1] = START;
    maze[curRow][curCol] = FINISH;

    // Ending procedures
    for (;;) {
      // print maze to monitor (serial) if the user presses the button
      if (keyscan()) {
        printMaze();
        delay(10000);

        // if the user presses the button again, restart the program
        lcd.home();
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("PRESS BUTTON");
        lcd.setCursor(3, 2);
        lcd.print("TO RESTART");
        while (!keyscan());
        return true;
      }
    }
  }

  /* If we aren't done yet, check if forwards is not physically blocked
    by a wall and is not a part of the solution (PATH) nor already
    explored or a boundary (CLOSED) */
  else if (Distance_test() > 15 && maze[checkRow][checkCol] == OPEN) {
    delay(100);
    rotations = 0;  // If we can move forward, we reset our rotation counter
    moveForward();
  }
  /* Since we want to check all four directions around us for a possible
    unexplored route, we check if the robot has already rotated three times,
    if not, we still have adjacent cells to check */
  else if (rotations < 3) {
    turn_left();  // turn 90 degrees left
    rotations++;  // We just turned so we update our rotations counter
  }
  /*If we've already rotated all the way around and not found a possible
    unexplored route, we've hit a dead end, so we mark as CLOSED
    so we don't go there again and then return false to remove it from the stack */
  else {
    lcd.clear();  // Provide the user with helpful messages
    lcd.home();
    lcd.print("DEAD END");
    maze[curRow][curCol] = CLOSED;

    // Since we're going to now backtrack onto another cell, we reset rotations counter
    rotations = 0;
    return false;
  }

  // Now we call recursive function and check if dead end
  // This ensures that any dead ends aren't on the recursive stack
  if (findPath() == false) {
    // Now we backtrack to the last vertex (intersection) and we begin exploring
    // from there, just like we would in a typical Depth First Search (DFS) algorithm
    backtrack();
    return findPath();
  }
}

// Print maze to serial monitor on computer, for improved user experience
void printMaze() {
  lcd.clear();
  lcd.home();
  lcd.print("PRINTING MAZE...");

  // 1 to 5 because that prints the inside of the maze, not the CLOSED border we added
  // We represent each type of cell using a different symbol or letter. Legend below
  for (int i = 1; i < 5; i++) {
    for (int j = 1; j < 5; j++) {
      switch (maze[i][j]) {
        case OPEN:
          Serial.print(".\t");
          break;
        case CLOSED:
          Serial.print("x\t");
          break;
        case PATH:
          Serial.print("+\t");
          break;
        case START:
          Serial.print("S\t");
          break;
        case FINISH:
          Serial.print("F\t");
      }
    }
    Serial.print("\n");
  }
  // Output the legend to the monitor so the user can easily understand it
  Serial.print(". is an open space\n x is a closed space\n + is the path");
  Serial.print(" of the robot\n. S is the start\n F is the finish");
}

