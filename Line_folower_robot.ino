// Motor and Sensor Pin Definitions
const int ENA = 9;
const int IN1 = 4;
const int IN2 = 2;
const int ENB = 3;
const int IN3 = 6;
const int IN4 = 5;

const int SENSOR_LEFT = 13;
const int SENSOR_CENTER = 11;  // Can go LOW during gaps or track loss
const int SENSOR_RIGHT = 10;

int current_mode = 0;         // 0 = Bluetooth ; 1 = Line Follower
int lastHighSensor = 0;       // 0 = None ; 1 = Left ; 2 = Right

void setup() {
  // Motor Pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);

  // Sensor Pins
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_CENTER, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("Ready for commands. Default mode: Bluetooth.");
}

void loop() {
  // Handle Mode Switching via Serial Commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LINE_FOLLOWER") {
      current_mode = 1;
      Serial.println("Line Follower mode activated.");
    } else if (command == "BLUETOOTH_MODE") {
      current_mode = 0;
      Serial.println("Bluetooth control mode activated.");
    }

    // Handle Bluetooth Commands if in Bluetooth Mode
    if (current_mode == 0) {
      handleBluetoothCommands(command);
    }
  }

  // Run Line Follower Logic if in Line Follower Mode
  if (current_mode == 1) {
    handleLineFollower();
  }

  delay(10);  // Small delay for stability
}

// ------------------- Bluetooth Commands -------------------

void handleBluetoothCommands(String command) {
  if (command == "up") {
    moveForward();
    Serial.println("Moving forward (Bluetooth)");
  } else if (command == "down") {
    moveBackward();
    Serial.println("Moving backward (Bluetooth)");
  } else if (command == "left") {
    turnLeft();
    Serial.println("Turning left (Bluetooth)");
  } else if (command == "right") {
    turnRight();
    Serial.println("Turning right (Bluetooth)");
  } else if (command == "STOP") {
    stopMotors();
    Serial.println("Stopping motors (Bluetooth)");
  }
}

// ------------------- Line Follower Logic -------------------

void handleLineFollower() {
  int left = digitalRead(SENSOR_LEFT);
  int center = digitalRead(SENSOR_CENTER);
  int right = digitalRead(SENSOR_RIGHT);

  // Debugging Sensor Values
  Serial.print("Sensor Left: ");
  Serial.print(left);
  Serial.print(", Center: ");
  Serial.print(center);
  Serial.print(", Right: ");
  Serial.println(right);

  if (center == HIGH) {
    // Line is detected by center sensor
    if (left == LOW && right == LOW) {
      moveForward();
      Serial.println("Moving forward");
    } else if (left == HIGH && right == LOW) {
      turnLeft();
      lastHighSensor = 1;
      Serial.println("Turning left");
    } else if (left == LOW && right == HIGH) {
      turnRight();
      lastHighSensor = 2;
      Serial.println("Turning right");
    } else if (left == HIGH && right == HIGH) {
      turnLeft();  // Prioritize left turn
      lastHighSensor = 1;
      Serial.println("All sensors HIGH: Prioritizing left turn");
    }
  } else {
    // Center sensor is LOW: Handle gaps or track loss
    if (left == HIGH && right == LOW) {
      turnLeft();
      lastHighSensor = 1;
      Serial.println("Center LOW but Left HIGH: Turning left");
    } else if (left == LOW && right == HIGH) {
      turnRight();
      lastHighSensor = 2;
      Serial.println("Center LOW but Right HIGH: Turning right");
    } else if (left == HIGH && right == HIGH) {
      turnLeft();
      lastHighSensor = 1;
      Serial.println("Center LOW but Both Left and Right HIGH: Prioritizing left turn");
    } else {
      // All sensors LOW: Handle track loss
      Serial.println("Center LOW and no side sensors: Reversing to recover track");
      reverseWithTurning();
    }
  }
}

// ------------------- Motor Control Functions -------------------

void moveForward() {
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, 60);
  analogWrite(ENB, 30);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, 30);
  analogWrite(ENB, 60);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void reverseWithTurning() {
  if (lastHighSensor == 1) {
    analogWrite(ENA, 30);
    analogWrite(ENB, 60);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Reversing and turning left");
  } else if (lastHighSensor == 2) {
    analogWrite(ENA, 60);
    analogWrite(ENB, 30);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Reversing and turning right");
  } else {
    analogWrite(ENA, 40);
    analogWrite(ENB, 40);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Reversing straight");
  }

  // Continue reversing until a sensor detects the line
  while (digitalRead(SENSOR_CENTER) == LOW && digitalRead(SENSOR_LEFT) == LOW && digitalRead(SENSOR_RIGHT) == LOW) {
    delay(10);
  }
  stopMotors();
  Serial.println("Line detected: Resuming normal operation");
}


