// In this file are the definitions of all functions that involve the following hardware:
// ultrasound sensors, bottom mounted infrared sensors, and front mounted infrared sensors

// Move forwards until the next intersection (moves into the next cell)
void moveForward() {
  int SL = LOW;    // Left and Right sensor status, HIGH means on and LOW means off
  int SR = LOW;

  // Update where we are, curRow and curCol, based on which direction we are moving
  switch (dir) {
    case NORTH:
      curRow--;
      break;
    case EAST:
      curCol++;
      break;
    case SOUTH:
      curRow++;
      break;
    case WEST:
      curCol--;
      break;
  }

  lcd.clear();  // LCD Output what we are doing to make the robot more user friendly
  lcd.home();
  lcd.print("MOVING FORWARDS");
  delay(100);

  /* Line following. We go forward staying on a line. The line is black (HIGH)
    and we are straddled on top of it so we only see the white on the side of the line.
    If either one of the sensors (SL and SR) becomes HIGH (detects the line), it means we
    aren't centered on top of the line and we have to correct by either shifting left or right.
    If both sensors detect black (HIGH), it means that we've hit a junction. */
  run();
  while (SL != HIGH || SR != HIGH) {
    // read in bottom mounted IR and update them for line detection
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);

    if (SL == LOW && SR == LOW)
      run();
    // correct if the car is drifting off to the side
    else if (SL == HIGH && SR == LOW)
      left();
    else if (SL == LOW && SR == HIGH)
      right();
  }
  /* If the car hits a junction (intersection), they are now in a new cell.
    Since the line detection infrared sensors are near the front of the vehicle,
    We need to run just a little bit so we are more centered above the intersection point*/
  brake(5);
  run();
  delay(300);
  brake(5);
}

/* backtrack to the last available path (do this when you hit a dead end).
   The concept is like how in a Depth First Search (DFS) you explore a path
   until you either reach the goal, or you hit a dead end and have to return
   to the last vertex and then explore from there */
void backtrack() {
  // When we look around, we check adjacent cells one by one
  int checkRow;   // Keeps track of what cell we are checking
  int checkCol;

  lcd.clear();    // LCD Output what we are doing to make the robot more user friendly
  lcd.home();
  lcd.print("Backtracking...");

  // We run a loop four times to check all four directions (N, E, W, S)
  for (int i = 0; i < 4; i++) {
    // We set the cell we are checking to our current cell, then increment either
    // checkRow or checkCol based on which adjacent cell we are facing
    checkRow = curRow;
    checkCol = curCol;
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

    /*If the cell we are facing and checking is not physically blocked at 15cm
      (distance_test returns the distance to objects in front of the ultrasound sensors)
      AND it's marked as PATH, which indicates that it's where we explored, we go there
      by using the moveForward function and stop backtracking by returning */
    if (Distance_test() > 15 && maze[checkRow][checkCol] == PATH) {
      delay(100);
      moveForward();
      return;
    }
    // If the cell we are facing is not where we came from, we turn and check
    // a different cell, looking for our previous path so we can backtrack
    turn_left();
    delay(100);
  }

  // Display error message to LCD Output to communicate with user
  while (!keyscan()) {
    delay(500);
    lcd.clear();
    lcd.home();
    lcd.setCursor(5, 0);
    lcd.print("ERROR:");
    lcd.setCursor(0, 2);
    lcd.print("Can't backtrack");
  }
}

// turn 90 degrees left - based on line following
void turn_left() {
  int SL, SR; // bottom mounted infrared sensor status, left and right

  /* Spin left for at least 2s so the robot moves off its current line.
    We move because our current line would interfere with our detection.
    We are looking for the next line on our left */
  spin_left();
  delay(200);

  /* Now we keep turning until the car just hits the next line. Since, the line is
    black, when we turn left, the left sensor will detect black (HIGH) at the line,
    and the right sensor should still only detect the white board (LOW) */
  while (SL != HIGH || SR != LOW) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);
  }
  brake(1);   // brake for accuracy

  /* Now we keep turning until the car is centered on top of the next line, so that
    means straddling the black line as so the line detectors only detect the white (LOW)
    that runs along the outer side edges of the narrow strip of black tape */
  while (SL != LOW || SR != LOW) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);

    // Adjusting our position - spin left if car is off to the right
    if (SL == HIGH && SR == LOW)
      spin_left();
    // Adjusting our position - spin right if car is off to the left
    if (SL == LOW && SR == HIGH)
      spin_right();
  }
  brake(1);

  // Since we just turned left, we modify our direction (dir) so that it's updated
  dir = DIR((dir + 3) % 4);
}

// gets button input (refer to top diagram if you need help finding the START button)
bool keyscan() {
  return digitalRead(key) == HIGH;
}

// display distance on the LCD, clear messages improve user experience
void Distance_display(int Distance) {
  lcd.clear();

  // If something is within the hardware range of the sensor, print the distance
  if (Distance > 2 && Distance < 400) {
    lcd.home();
    lcd.print("    Distance: ");
    lcd.setCursor(6, 2);
    lcd.print(Distance);
    lcd.print("cm");
  }

  // If out of range, report the appropriate error message so the user knows
  else {
    lcd.home();
    lcd.print("Out of range!!!     ");
  }
  delay(250);
}

// Measures and returns the distance between the ultrasound sensors
// and the closest object placed in front of them
float Distance_test() {
  float Fdistance; // holds front distance value

  // send out sonic pulses, once they hit an object they should bounce back (echo)
  digitalWrite(Trig, LOW); // Trig is the output trigger pin for ultrasound (sends sound when HIGH)
  delayMicroseconds(2);    // Delays are for pauses when the robot "listens" for echo
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  // get the echo. Sensors determine distance based on strength and speed of return
  // Echo is input echo pin for ultrasound, HIGH means detected.
  Fdistance = pulseIn(Echo, HIGH);
  // calculate, display, and return distance, 58 is the calibrated conversion from ms to cm
  Fdistance = Fdistance / 58;
  Distance_display(Fdistance);
  return Fdistance;
}

// check if the finish is reached. True when we are facing the black board at the end of the maze
bool checkFinish() {
  int frontSR; // front infrared detection sensors, right and left.
  int frontSL;
  frontSR = digitalRead(FrontSensorRight);
  frontSL = digitalRead(FrontSensorLeft);

  // If there's a board in front of us, and it's black (HIGH), the maze is complete
  if (Distance_test() < 15 && frontSL == HIGH && frontSR == HIGH)
    return true;

  return false;
}

