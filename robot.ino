#define PWMA 6
#define AIN1 5
#define AIN2 4
#define AENC1 3
#define AENC2 2

#define PWMB 10
#define BIN1 9
#define BIN2 11
#define BENC1 13
#define BENC2 12

#define LedPin 7
#define button1 A1
#define button2 A2

int encoderCountA = 0;
int encoderCountB = 0;

float radius = 3.4;
float half_width = 15.0 / 2;

int state = 0;

// ---- BUTTON CONTROL ----
bool runEnabled = true;
bool lastButton1 = HIGH;
bool lastButton2 = HIGH;

// instructions
char actions[] = {'d','t','d','t','d','t','d','t','d','t','d'};
float values[] = {38,70,31,26,20,13,64,17,64,14}; // ✅ FIXED: removed double comma

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AENC1, INPUT);
  pinMode(AENC2, INPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BENC1, INPUT);
  pinMode(BENC2, INPUT);

  pinMode(LedPin, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(AENC1), encoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BENC2), encoderB, CHANGE);

  Serial.begin(9600);
}

void loop() {
  bool b1 = digitalRead(button1);
  bool b2 = digitalRead(button2);

  bool pressed =
    (lastButton1 == HIGH && b1 == LOW) ||
    (lastButton2 == HIGH && b2 == LOW);

  if (pressed) {
    runEnabled = !runEnabled;

    if (!runEnabled) {
      stopMotors();
    } else {
      state = 0;
      resetEncoders();
    }
    delay(200);
  }

  lastButton1 = b1;
  lastButton2 = b2;

  if (!runEnabled) return;

  if (actions[state] == 'd') {
    drive(values[state], 150);
  }

  if (actions[state] == 't') {
    turn(values[state], 100);
    led();
  }
}

void drive(float distance, int speed) {
  float avgDist = (getDistanceA() + getDistanceB()) / 2.0;

  if (distance > 0 && avgDist < distance) {
    driveA(speed);
    driveB(-speed * 1.2);
  }
  else if (distance < 0 && avgDist > distance) {
    driveA(speed);
    driveB(-speed * 1.2);
  }
  else {
    stopMotors();
    state++;
    resetEncoders();
  }
}

void driveA(int speed) {
  digitalWrite(AIN1, speed > 0);
  digitalWrite(AIN2, speed < 0);
  analogWrite(PWMA, abs(speed));
}

void driveB(int speed) {
  digitalWrite(BIN1, speed > 0);                          
  digitalWrite(BIN2, speed < 0);
  analogWrite(PWMB, abs(speed));
}

// ✅ NONSTOP ROTATION FIXED HERE
void turn(float angle, int speed) {
  float curAngle = getAngle();

  if (abs(curAngle) < abs(angle)) {
    if (angle > 0) {
      driveA(-speed);
      driveB(speed * 1.2); //3
    } else {
      driveA(-speed);
      driveB(speed * 1.2);
    }
  } else {
    stopMotors();
    state++;
    resetEncoders();
  }
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void resetEncoders() {
  encoderCountA = 0;
  encoderCountB = 0;
}

float getAngle() {
  float dA = getDistanceA();
  float dB = getDistanceB();
  float angleRad = (dB - dA) / (2.0 * half_width);
  return angleRad * 180.0 / 3.14159;
}

float getDistanceA() { return getRotationsA() * 2 * 3.14159 * radius; }
float getDistanceB() { return getRotationsB() * 2 * 3.14159 * radius; }

float getRotationsA() { return encoderCountA / 14.0 / 100.0; }
float getRotationsB() { return encoderCountB / 14.0 / 100.0; }

void encoderA() {
  if (digitalRead(AENC1) != digitalRead(AENC2)) encoderCountA++;
  else encoderCountA--;
}

void encoderB() {
  if (digitalRead(BENC1) != digitalRead(BENC2)) encoderCountB++;
  else encoderCountB--;
}

void led() {
  digitalWrite(LedPin, HIGH);
  delay(125);
  digitalWrite(LedPin, LOW);
  delay(125);
}
