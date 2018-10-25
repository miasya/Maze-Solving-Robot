// This file includes only all the helper functions for motor control and movement
// Note: this code is a modified version of the manufacturer's code provided in the
// kit, and so it contains Arduino-specific commands.

// GO FORWARDS - Sets right and left motor to go both forwards at RunSpeed
void run() {
  digitalWrite(Right_motor_go, HIGH); // Reminder that HIGH means on and LOW means off
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}

// STOPS THE CAR FOR int time SECONDS, often helps with accuracy for turns
void brake(int time) {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  delay(time * 100);  // By default, the Arduino uses microsec, so we convert to sec
}

// Starts the car turning left, used to adjust robot for line following and tracking
void left() {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 0);
}

// Starts the car turning right, used to adjust robot for line following and tracking
void right() {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}

// Turn left on the spot by moving the left motor backwards and the right motor forwards
void spin_left() {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, RunSpeed);
}

// Turn right on the spot by moving the right motor backwards and the left motor forwards
void spin_right() {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, RunSpeed);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}
