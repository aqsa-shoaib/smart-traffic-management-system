#include <LiquidCrystal.h>
#include <Servo.h>

// Traffic signals for the two directions of the intersection.
const int RED_1 = 13;
const int YELLOW_1 = 12;
const int GREEN_1 = 11;
const int RED_2 = 10;
const int YELLOW_2 = 9;
const int GREEN_2 = 8;

// Pedestrian buttons and simulated traffic sensors.
const int BUTTON_1 = A2;
const int BUTTON_2 = A3;
const int LIGHT_SENSOR_1 = A0;
const int LIGHT_SENSOR_2 = A1;

const int SERVO_1_PIN = A4;
const int SERVO_2_PIN = A5;
const int TRAFFIC_THRESHOLD = 20;

const unsigned long GREEN_TIME = 5000;
const unsigned long EXTENDED_GREEN_TIME = 10000;
const unsigned long YELLOW_TIME = 1000;
const unsigned long ALL_RED_TIME = 500;

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);
Servo laneServo1;
Servo laneServo2;

bool crossingRequested1 = false;
bool crossingRequested2 = false;
bool lastButton1 = false;
bool lastButton2 = false;
bool laneOpen = false;

// Keep both button requests until their corresponding green phase occurs.
void readButtons() {
  bool button1 = digitalRead(BUTTON_1) == HIGH;
  bool button2 = digitalRead(BUTTON_2) == HIGH;

  if (button1 && !lastButton1) crossingRequested1 = true;
  if (button2 && !lastButton2) crossingRequested2 = true;

  lastButton1 = button1;
  lastButton2 = button2;
}

// Use two photoresistors to simulate light-based traffic congestion.
void updateTrafficLevel() {
  bool heavyTraffic = analogRead(LIGHT_SENSOR_1) < TRAFFIC_THRESHOLD &&
                      analogRead(LIGHT_SENSOR_2) < TRAFFIC_THRESHOLD;

  if (heavyTraffic == laneOpen) return;
  laneOpen = heavyTraffic;
  lcd.clear();

  if (laneOpen) {
    lcd.setCursor(0, 0);
    lcd.print("Heavy Traffic");
    lcd.setCursor(0, 1);
    lcd.print("Extra Lane Open");
    laneServo1.write(45);
    laneServo2.write(135);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Moderate Traffic");
    lcd.setCursor(0, 1);
    lcd.print("Normal Lanes");
    laneServo1.write(0);
    laneServo2.write(45);
  }
}

// Display one signal state while checking for pedestrian requests.
void showSignals(bool red1, bool yellow1, bool green1,
                 bool red2, bool yellow2, bool green2) {
  digitalWrite(RED_1, red1);
  digitalWrite(YELLOW_1, yellow1);
  digitalWrite(GREEN_1, green1);
  digitalWrite(RED_2, red2);
  digitalWrite(YELLOW_2, yellow2);
  digitalWrite(GREEN_2, green2);
}

// Check inputs repeatedly rather than blocking for a full delay.
void holdPhase(unsigned long duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    readButtons();
    updateTrafficLevel();
    delay(20);
  }
}

void setup() {
  const int signalPins[] = {
    RED_1, YELLOW_1, GREEN_1, RED_2, YELLOW_2, GREEN_2
  };
  for (int pin : signalPins) pinMode(pin, OUTPUT);

  // Buttons are active HIGH, matching the original classroom circuit.
  // Each button needs a pull-down resistor to avoid floating inputs.
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(LIGHT_SENSOR_1, INPUT);
  pinMode(LIGHT_SENSOR_2, INPUT);

  Serial.begin(57600);
  laneServo1.attach(SERVO_1_PIN);
  laneServo2.attach(SERVO_2_PIN);
  lcd.begin(16, 2);

  // Set the initial lane positions before monitoring sensor values.
  laneServo1.write(0);
  laneServo2.write(45);
  lcd.print("Moderate Traffic");
  lcd.setCursor(0, 1);
  lcd.print("Normal Lanes");
  updateTrafficLevel();
  Serial.println("Traffic controller started");
}

void loop() {
  // Direction 2 moves while direction 1 remains stopped.
  showSignals(HIGH, LOW, LOW, LOW, LOW, HIGH);
  holdPhase(crossingRequested2 ? EXTENDED_GREEN_TIME : GREEN_TIME);
  crossingRequested2 = false;

  showSignals(HIGH, LOW, LOW, LOW, HIGH, LOW);
  holdPhase(YELLOW_TIME);
  showSignals(HIGH, LOW, LOW, HIGH, LOW, LOW);
  holdPhase(ALL_RED_TIME);

  // Direction 1 moves while direction 2 remains stopped.
  showSignals(LOW, LOW, HIGH, HIGH, LOW, LOW);
  holdPhase(crossingRequested1 ? EXTENDED_GREEN_TIME : GREEN_TIME);
  crossingRequested1 = false;

  showSignals(LOW, HIGH, LOW, HIGH, LOW, LOW);
  holdPhase(YELLOW_TIME);
  showSignals(HIGH, LOW, LOW, HIGH, LOW, LOW);
  holdPhase(ALL_RED_TIME);
}
