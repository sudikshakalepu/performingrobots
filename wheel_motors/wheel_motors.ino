#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3

// pin usage
#define RC_CH1_PIN  6
#define RC_CH2_PIN  5
#define RC_CH3_PIN  4
#define RC_CH4_PIN  3

// motor driver pins
const int EN1_PIN = 9;
const int IN1_PIN = 8;
const int IN2_PIN = 7;
const int IN3_PIN = 10;
const int IN4_PIN = 12;
const int EN2_PIN = 11;

// separate PWM thresholds
int MIN_PWM_M1 = 25;
int MIN_PWM_M2 = 35;     // Motor 2 needs more "kick" to spin

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_PIN); }
void calc_ch2() { calc_input(RC_CH2, RC_CH2_PIN); }
void calc_ch3() { calc_input(RC_CH3, RC_CH3_PIN); }
void calc_ch4() { calc_input(RC_CH4, RC_CH4_PIN); }

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);

  enableInterrupt(RC_CH1_PIN, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_PIN, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_PIN, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_PIN, calc_ch4, CHANGE);

  pinMode(EN1_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(EN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
}

// =====================================================
// Smooth stop function
// =====================================================
void smoothStop() {
  for (int speed = 180; speed >= 0; speed -= 4) {
    analogWrite(EN1_PIN, speed);
    analogWrite(EN2_PIN, speed);
    delay(20);
  }

  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

void loop() {
  rc_read_values();

  delay(50);

  bool neutral_forward = (rc_values[RC_CH2] > 1300 && rc_values[RC_CH2] < 1700);
  bool neutral_turn = (rc_values[RC_CH1] > 1300 && rc_values[RC_CH1] < 1700);

  // ============================================================
  // FORWARD / REVERSE - Now proportional to trigger position
  // ============================================================
  if (rc_values[RC_CH2] > 1700) {
    // Forward - map the trigger position to motor speed
    int speed = map(rc_values[RC_CH2], 1700, 2300, MIN_PWM_M1, 180);
    forward(speed);
  }
  else if (rc_values[RC_CH2] < 1300) {
    // Reverse - map the trigger position to motor speed
    int speed = map(rc_values[RC_CH2], 600, 1300, 180, MIN_PWM_M1);
    reverse(speed);
  }
  else if (neutral_forward && neutral_turn) {
    smoothStop();
  }

  // ============================================================
  // TURNING - Now proportional to steering knob position
  // ============================================================
  if (rc_values[RC_CH1] > 1750) {
    // Right turn - map the knob position to turn speed
    int speed = map(rc_values[RC_CH1], 1750, 2500, MIN_PWM_M1, 180);
    right(speed);
  }
  else if (rc_values[RC_CH1] < 1250) {
    // Left turn - map the knob position to turn speed
    int speed = map(rc_values[RC_CH1], 600, 1250, 180, MIN_PWM_M1);
    left(speed);
  }
}

void forward(int value) {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);

  analogWrite(EN1_PIN, constrain(value, MIN_PWM_M1, 180));
  analogWrite(EN2_PIN, constrain(value, MIN_PWM_M2, 180));
}

void reverse(int value) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);

  analogWrite(EN1_PIN, constrain(value, MIN_PWM_M1, 180));
  analogWrite(EN2_PIN, constrain(value, MIN_PWM_M2, 180));
}

void right(int value) {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);

  analogWrite(EN1_PIN, constrain(value, MIN_PWM_M1, 180));
  analogWrite(EN2_PIN, constrain(value, MIN_PWM_M2, 180));
}

void left(int value) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);

  analogWrite(EN1_PIN, constrain(value, MIN_PWM_M1, 180));
  analogWrite(EN2_PIN, constrain(value, MIN_PWM_M2, 180));
}