// Sudiksha & Aysha
// Professor Micheal Shiloh
// Performing Robots Fall '25

/*
   Using the nRF24L01 radio module to communicate
   between two Arduinos with much increased reliability following
   various tutorials, conversations, and studying the nRF24L01 datasheet
   and the library reference.

   Transmitter is
   https://github.com/michaelshiloh/resourcesForClasses/tree/master/kicad/Arduino_Shield_RC_Controller

  Receiver is
  https://github.com/michaelshiloh/resourcesForClasses/blob/master/kicad/nRF_servo_Mega

   This file contains code for both transmitter and receiver.
   Transmitter at the top, receiver at the bottom.
   One of them is commented out, so you need to comment in or out
   the correct section. You don't need to make changes to this
   part of the code, just to comment in or out depending on
   whether you are programming your transmitter or receiver

   You need to set the correct address for your robot.

   Search for the phrase CHANGEHERE to see where to
   comment or uncomment or make changes.

   These sketches require the RF24 library by TMRh20
   Documentation here: https://nrf24.github.io/RF24/index.html

   change log

   11 Oct 2023 - ms - initial entry based on
                  rf24PerformingRobotsTemplate
   26 Oct 2023 - ms - revised for new board: nRF_Servo_Mega rev 2
   28 Oct 2023 - ms - add demo of NeoMatrix, servo, and Music Maker Shield
   20 Nov 2023 - as - fixed the bug which allowed counting beyond the limits
   22 Nov 2023 - ms - display radio custom address byte and channel
   12 Nov 2024 - ms - changed names for channel and address allocation for Fall 2024
                      https://github.com/michaelshiloh/resourcesForClasses/blob/master/kicad/nRF_servo_Mega
                      https://github.com/michaelshiloh/resourcesForClasses/blob/master/kicad/nRFControlPanel

   [USER] Nov 2025 - Custom choreography for gavel, head, and cigar robot
                     with servo + NeoPixel helpers mapped to M1–M6.
*/

// Common code
//

// CHANGEHERE
// For the transmitter
const int NRF_CE_PIN = A4, NRF_CSN_PIN = A5;

// CHANGEHERE
// for the receiver
// const int NRF_CE_PIN = A11, NRF_CSN_PIN = A15;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN); // CE, CSN

// CHANGEHERE
const byte CUSTOM_ADDRESS_BYTE = 0x73; // change as per the above assignment
const int CUSTOM_CHANNEL_NUMBER = 40;  // change as per the above assignment

// Do not make changes here
const byte xmtrAddress[] = {CUSTOM_ADDRESS_BYTE, CUSTOM_ADDRESS_BYTE, 0xC7, 0xE6, 0xCC};
const byte rcvrAddress[] = {CUSTOM_ADDRESS_BYTE, CUSTOM_ADDRESS_BYTE, 0xC7, 0xE6, 0x66};

const int RF24_POWER_LEVEL = RF24_PA_LOW;

// global variables
uint8_t pipeNum;
unsigned int totalTransmitFailures = 0;

struct DataStruct
{
  uint8_t stateNumber;
};
DataStruct data;

bool isTransmitting = false;

void setupRF24Common()
{
  if (!radio.begin())
  {
    Serial.println(F("radio  initialization failed"));
    while (1)
      ;
  }
  else
  {
    Serial.println(F("radio successfully initialized"));
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(CUSTOM_CHANNEL_NUMBER);
  radio.setPALevel(RF24_POWER_LEVEL);
}

// ============ TRANSMITTER CODE (COMMENTED OUT HERE) =============

// Transmitter code

// Transmitter pin usage
const int LCD_RS_PIN = 3, LCD_EN_PIN = 2, LCD_D4_PIN = 4, LCD_D5_PIN = 5, LCD_D6_PIN = 6, LCD_D7_PIN = 7;
const int SW1_PIN = 8, SW2_PIN = 9, SW3_PIN = 10, SW4_PIN = A3, SW5_PIN = A2;

#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

const int NUM_OF_STATES = 32;
char *theStates[] = {
    "Smoke",
    "Gavel",
    "Dance",
    "They",
    "Court",
    "Logs",
    "Prepos",
    "Over",
    "Impos",
    "Succu",
    "WoWifi",
    "Object",
    "NoImJu",
    "CtAdj",
    "CtIsNow",
    "Salsa",
    "ChildDo",
    "iMeantM",
    "ChiPvt",
    "SaySmt",
    "Case",
    "TooLate"};

void updateLCD()
{
  lcd.clear();
  lcd.print(theStates[data.stateNumber]);
  lcd.setCursor(0, 1);
  lcd.print("not transmitted yet");
}

void countDown()
{
  data.stateNumber = (data.stateNumber > 0) ? (data.stateNumber - 1) : 0;
  updateLCD();
}

void countUp()
{
  if (++data.stateNumber >= NUM_OF_STATES)
  {
    data.stateNumber = NUM_OF_STATES - 1;
  }
  updateLCD();
}
void spare1() {}
void spare2() {}

void rf24SendData()
{
  radio.stopListening();
  int retval = radio.write(&data, sizeof(data));

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("transmitting");
  lcd.setCursor(14, 0);
  lcd.print(data.stateNumber);

  Serial.print(F(" ... "));
  if (retval)
  {
    Serial.println(F("success"));
    lcd.setCursor(0, 1);
    lcd.print("success");
  }
  else
  {
    totalTransmitFailures++;
    Serial.print(F("failure, total failures = "));
    Serial.println(totalTransmitFailures);

    lcd.setCursor(0, 1);
    lcd.print("error, total=");
    lcd.setCursor(13, 1);
    lcd.print(totalTransmitFailures);
  }
}

class Button
{
  int pinNumber;
  bool previousState;
  void (*buttonFunction)();

public:
  Button(int pn, void *bf)
  {
    pinNumber = pn;
    buttonFunction = (void (*)())bf;
    previousState = 1;
  }

  void update()
  {
    bool currentState = digitalRead(pinNumber);
    if (currentState == LOW && previousState == HIGH)
    {
      Serial.print("button on pin ");
      Serial.print(pinNumber);
      Serial.println();
      buttonFunction();
    }
    previousState = currentState;
  }
};

const int NUMBUTTONS = 5;
Button theButtons[] = {
    Button(SW1_PIN, (void *)countDown),
    Button(SW2_PIN, (void *)rf24SendData),
    Button(SW3_PIN, (void *)countUp),
    Button(SW4_PIN, (void *)spare1),
    Button(SW5_PIN, (void *)spare2),
};

void setupRF24()
{
  setupRF24Common();

  radio.openWritingPipe(xmtrAddress);
  radio.openReadingPipe(1, rcvrAddress);

  Serial.println(F("I am a transmitter"));
  data.stateNumber = 0;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Setting up LCD"));

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Radio setup");

  lcd.setCursor(0, 1);
  lcd.print("addr 0x");
  lcd.setCursor(7, 1);
  char s[5];
  sprintf(s, "%02x", CUSTOM_ADDRESS_BYTE);
  lcd.print(s);

  lcd.setCursor(10, 1);
  lcd.print("ch");
  lcd.setCursor(13, 1);
  lcd.print(CUSTOM_CHANNEL_NUMBER);

  Serial.println(F("Setting up radio"));
  setupRF24();

  lcd.setCursor(0, 0);
  lcd.print("Radio OK ");
  lcd.print(theStates[data.stateNumber]);

  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);
  pinMode(SW5_PIN, INPUT_PULLUP);
}

void loop()
{
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    theButtons[i].update();
  }
  delay(50);
}

void clearData()
{
  data.stateNumber = 0;
}

// ============ END TRANSMITTER (COMMENTED) =============

/*
// ================= RECEIVER CODE ======================

#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Music Maker Shield pins
#define SHIELD_RESET -1
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define CARDCS 4
#define DREQ 3

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(
    SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Servo pins / mapping
const int GAVEL_SERVO_PIN = 18; // M5
const int CIGAR_SERVO_PIN = 20; // M3
const int HEAD_SERVO_PIN  = 17; // M6

Servo gavel;
Servo cigar;
Servo head;

// Servo IDs for smooth helper
enum
{
  SERVO_HEAD  = 0,
  SERVO_GAVEL = 1,
  SERVO_CIGAR = 2
};

// Gavel angles
const int GAVEL_REST_ANGLE = 150;
const int GAVEL_UP_ANGLE   = 50;
const int GAVEL_BANG_ANGLE = 90;

// Cigar angles
const int CIGAR_DOWN_ANGLE = 70;
const int CIGAR_UP_ANGLE   = 180;

// Head angles
const int HEAD_CENTER_ANGLE = 130;
const int HEAD_RIGHT_ANGLE  = 90;
const int HEAD_LEFT_ANGLE   = 180;

// NeoPixel configuration
const uint8_t HEAD_LED_COUNT  = 16;
const uint8_t CIGAR_LED_COUNT = 16;

const int HEAD_NEOPIXEL_PIN_1 = 19; // M1
const int HEAD_NEOPIXEL_PIN_2 = 16; // M4
const int CIGAR_NEOPIXEL_PIN  = 21; // M2

Adafruit_NeoPixel headStrip1 = Adafruit_NeoPixel(
    HEAD_LED_COUNT, HEAD_NEOPIXEL_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel headStrip2 = Adafruit_NeoPixel(
    HEAD_LED_COUNT, HEAD_NEOPIXEL_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel cigarStrip = Adafruit_NeoPixel(
    CIGAR_LED_COUNT, CIGAR_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ----- Colour helpers -----

uint32_t headColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
  return headStrip1.Color(r, g, b);
}

uint32_t cigarColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
  return cigarStrip.Color(r, g, b);
}

uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return headColorRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return headColorRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return headColorRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// ----- Head NeoPixels -----

void headSetColor(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t c = headColorRGB(r, g, b);
  for (uint8_t i = 0; i < HEAD_LED_COUNT; i++)
  {
    headStrip1.setPixelColor(i, c);
    headStrip2.setPixelColor(i, c);
  }
  headStrip1.show();
  headStrip2.show();
}

void headSetDefault() { headSetColor(255, 255, 255); }
void headSetRed()     { headSetColor(255, 0, 0); }
void headSetYellow()  { headSetColor(255, 255, 0); }
void headSetGreen()   { headSetColor(0, 255, 0); }
void headSetPink()    { headSetColor(255, 0, 128); }

void headColorWheelAnimation(unsigned long durationMs)
{
  const uint8_t COLORS[5][3] = {
      {255, 0,   128},
      {255, 255, 0},
      {160, 0,   255},
      {0,   255, 0},
      {0,   255, 255}
  };

  unsigned long start = millis();
  uint8_t idx = 0;
  while (millis() - start < durationMs)
  {
    headSetColor(COLORS[idx][0], COLORS[idx][1], COLORS[idx][2]);
    delay(180);
    idx = (idx + 1) % 5;
  }
}

void headRedYellowAltAnimation(unsigned long durationMs)
{
  unsigned long start = millis();
  bool flip = false;
  while (millis() - start < durationMs)
  {
    for (uint8_t i = 0; i < HEAD_LED_COUNT; i++)
    {
      bool isRed = ((i + (flip ? 1 : 0)) % 2 == 0);
      if (isRed)
      {
        headStrip1.setPixelColor(i, headColorRGB(255, 0, 0));
        headStrip2.setPixelColor(i, headColorRGB(255, 0, 0));
      }
      else
      {
        headStrip1.setPixelColor(i, headColorRGB(255, 255, 0));
        headStrip2.setPixelColor(i, headColorRGB(255, 255, 0));
      }
    }
    headStrip1.show();
    headStrip2.show();
    flip = !flip;
    delay(150);
  }
}

void headBlushAnimation(unsigned long durationMs)
{
  const uint8_t SHADES[3][3] = {
      {255, 40, 140},
      {255, 80, 180},
      {255, 0,  128}
  };

  unsigned long start = millis();
  uint8_t idx = 0;
  while (millis() - start < durationMs)
  {
    headSetColor(SHADES[idx][0], SHADES[idx][1], SHADES[idx][2]);
    delay(160);
    idx = (idx + 1) % 3;
  }
  headSetPink();
}

// ----- Cigar NeoPixel -----

void cigarSetColor(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t c = cigarColorRGB(r, g, b);
  for (uint8_t i = 0; i < CIGAR_LED_COUNT; i++)
  {
    cigarStrip.setPixelColor(i, c);
  }
  cigarStrip.show();
}

void cigarOff()
{
  cigarSetColor(0, 0, 0);
}

// Smoking animation with orange-red ember
void cigarSmokeAnimation(unsigned long durationMs)
{
  unsigned long start = millis();

  // Smooth lift to mouth locally
  int step = 3;
  int delayMs = 15;
  int current = cigar.read();
  int dir = (CIGAR_UP_ANGLE > current) ? 1 : -1;
  for (int pos = current; dir > 0 ? pos <= CIGAR_UP_ANGLE : pos >= CIGAR_UP_ANGLE; pos += dir * step)
  {
    cigar.write(pos);
    delay(delayMs);
  }
  cigar.write(CIGAR_UP_ANGLE);

  while (millis() - start < durationMs)
  {
    uint8_t red   = random(230, 255);
    uint8_t green = random(30, 80);
    uint8_t blue  = 0;
    cigarSetColor(red, green, blue);
    delay(80);
  }

  cigarSetColor(180, 40, 0);
}

// ----- Smooth servo helper (no Servo& in signature) -----

void moveServoSmooth(int whichServo, int targetAngle, int step, int delayMs)
{
  Servo *s;

  if (whichServo == SERVO_HEAD)
    s = &head;
  else if (whichServo == SERVO_GAVEL)
    s = &gavel;
  else
    s = &cigar;

  int current = s->read();
  if (current == targetAngle) return;

  int dir = (targetAngle > current) ? 1 : -1;

  for (int pos = current; dir > 0 ? pos <= targetAngle : pos >= targetAngle; pos += dir * step)
  {
    s->write(pos);
    delay(delayMs);
  }

  s->write(targetAngle);
}

// ----- Gavel helpers using smooth -----

void gavelToRest()
{
  moveServoSmooth(SERVO_GAVEL, GAVEL_REST_ANGLE, 3, 15);
}

void gavelUp()
{
  moveServoSmooth(SERVO_GAVEL, GAVEL_UP_ANGLE, 3, 15);
}

void gavelBangOnce()
{
  gavelUp();
  moveServoSmooth(SERVO_GAVEL, GAVEL_BANG_ANGLE, 4, 12);
  moveServoSmooth(SERVO_GAVEL, GAVEL_REST_ANGLE, 4, 15);
}

void gavelBangTwice()
{
  gavelBangOnce();
  delay(180);
  gavelBangOnce();
}

// ----- Head helpers using smooth -----

void headLookCenter()
{
  moveServoSmooth(SERVO_HEAD, HEAD_CENTER_ANGLE, 2, 15);
}

void headLookRight()
{
  moveServoSmooth(SERVO_HEAD, HEAD_RIGHT_ANGLE, 2, 15);
}

void headLookLeft()
{
  moveServoSmooth(SERVO_HEAD, HEAD_LEFT_ANGLE, 2, 15);
}

// ----- Default pose -----

void setDefaultPose()
{
  headSetDefault();
  cigarOff();

  moveServoSmooth(SERVO_HEAD,  HEAD_CENTER_ANGLE, 2, 15);
  moveServoSmooth(SERVO_GAVEL, GAVEL_REST_ANGLE,  3, 15);
  moveServoSmooth(SERVO_CIGAR, CIGAR_DOWN_ANGLE,  3, 15);
}

// ----- Dance + expressive motions -----

void danceEntranceCase0(unsigned long durationMs)
{
  // Soft pastel-ish mood colours
  const uint8_t COLORS[4][3] = {
      {255, 200, 220}, // soft pink
      {255, 240, 180}, // warm cream / light yellow
      {200, 230, 255}, // light blue
      {210, 255, 210}  // pale green
  };

  const unsigned long COLOR_INTERVAL = 2200; // very slow colour changes
  const unsigned long HEAD_STEP_INT  = 400;  // slow head movement

  const int HEAD_STEP      = 10;                       // tiny steps
  const int HEAD_MIN_ANGLE = HEAD_CENTER_ANGLE - 20;  // small sway only
  const int HEAD_MAX_ANGLE = HEAD_CENTER_ANGLE + 20;  // small sway only

  unsigned long startTime       = millis();
  unsigned long lastColorChange = startTime;
  unsigned long lastHeadStep    = startTime;

  uint8_t colorIndex = 0;

  int headAngle = HEAD_CENTER_ANGLE;
  int headDir   = -1;

  bool cigarRaised = false; // we will lift cigar ONCE, mid-dance

  // Initial gentle pose
  moveServoSmooth(SERVO_HEAD,  HEAD_CENTER_ANGLE, 2, 15);
  moveServoSmooth(SERVO_GAVEL, GAVEL_REST_ANGLE,  3, 15);
  moveServoSmooth(SERVO_CIGAR, CIGAR_DOWN_ANGLE,  3, 15);
  cigarOff();

  headSetColor(COLORS[colorIndex][0], COLORS[colorIndex][1], COLORS[colorIndex][2]);

  while (millis() - startTime < durationMs)
  {
    unsigned long now = millis();

    // --- Very slow colour fade-through ---
    if (now - lastColorChange >= COLOR_INTERVAL)
    {
      lastColorChange = now;
      colorIndex = (colorIndex + 1) % 4;
      headSetColor(COLORS[colorIndex][0], COLORS[colorIndex][1], COLORS[colorIndex][2]);
    }

    // --- Small, graceful head sway around centre ---
    if (now - lastHeadStep >= HEAD_STEP_INT)
    {
      lastHeadStep = now;

      headAngle += headDir * HEAD_STEP;

      if (headAngle <= HEAD_MIN_ANGLE)
      {
        headAngle = HEAD_MIN_ANGLE;
        headDir   = +1;
      }
      else if (headAngle >= HEAD_MAX_ANGLE)
      {
        headAngle = HEAD_MAX_ANGLE;
        headDir   = -1;
      }

      head.write(headAngle); // tiny continuous adjustments
    }

    // --- One elegant cigar gesture halfway through ---
    if (!cigarRaised && (now - startTime > durationMs / 2))
    {
      cigarRaised = true;
      // Slow, elegant lift + soft ember glow
      moveServoSmooth(SERVO_CIGAR, CIGAR_UP_ANGLE, 3, 18);
      cigarSetColor(220, 70, 10); // warm orange-red ember
      delay(400);
      moveServoSmooth(SERVO_CIGAR, CIGAR_DOWN_ANGLE, 3, 18);
      cigarSetColor(140, 30, 0);  // dimmer ember
    }

    delay(10);
  }

  // Gently return to neutral, composed pose
  setDefaultPose();
}


// “No, no” head shake
void headShakeNo()
{
  for (int i = 0; i < 2; i++)
  {
    headLookRight();
    delay(220);
    headLookLeft();
    delay(220);
  }
  headLookCenter();
}

// Slow left → right → left look
void headSlowJitterToWife()
{
  headLookLeft();
  delay(600);
  headLookRight();
  delay(600);
  headLookLeft();
  delay(600);
}

// Gavel + cigar accent
void gavelAndCigarJitter()
{
  for (int i = 0; i < 3; i++)
  {
    gavelBangOnce();
    moveServoSmooth(SERVO_CIGAR, CIGAR_UP_ANGLE, 3, 15);
    cigarSetColor(255, 50, 0);
    delay(220);
    moveServoSmooth(SERVO_CIGAR, CIGAR_DOWN_ANGLE, 3, 15);
    cigarOff();
    delay(220);
  }
  moveServoSmooth(SERVO_CIGAR, CIGAR_DOWN_ANGLE, 3, 15);
  gavelToRest();
}

// ----- Setup helpers -----

void setupMusicMakerShield()
{
  if (!musicPlayer.begin())
  {
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1)
      ;
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS))
  {
    Serial.println(F("SD card failed or not present"));
    while (1)
      ;
  }

  musicPlayer.setVolume(20, 20);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void setupServoMotors()
{
  gavel.write(GAVEL_REST_ANGLE);
  gavel.attach(GAVEL_SERVO_PIN);
  cigar.write(CIGAR_DOWN_ANGLE);
  cigar.attach(CIGAR_SERVO_PIN);
  head.write(HEAD_CENTER_ANGLE);
  head.attach(HEAD_SERVO_PIN);
}

void flashHeadPixels()
{
  headSetDefault();
  cigarOff();
  delay(300);
  headSetColor(0, 0, 0);
  cigarOff();
  delay(150);
}

void setupRF24()
{
  setupRF24Common();

  radio.openWritingPipe(rcvrAddress);
  radio.openReadingPipe(1, xmtrAddress);

  Serial.println(F("I am a receiver"));
}

void setup()
{
  Serial.begin(9600);

  setupMusicMakerShield();
  setupServoMotors();

  headStrip1.begin();
  headStrip1.show();
  headStrip2.begin();
  headStrip2.show();
  cigarStrip.begin();
  cigarStrip.show();

  setupRF24();
  flashHeadPixels();
}

// ----- Main loop: handle incoming cases -----

void loop()
{
  radio.startListening();
  if (radio.available(&pipeNum))
  {
    radio.read(&data, sizeof(data));

    Serial.print(F("message received Data = "));
    Serial.println(data.stateNumber);

    switch (data.stateNumber)
    {
    case 0:
      cigarSmokeAnimation(2400);
      headSetYellow();
      setDefaultPose();
      break;

    case 1:
      headSetRed();
      gavelBangOnce();
      setDefaultPose();
      break;

    case 2:
      Serial.println(F("Case 2: Dance entrance"));
      danceEntranceCase0(6000);
      break;

    case 3:
      Serial.println(F("Case 3: Cigar + 'They look'"));
      cigarSmokeAnimation(800);
      musicPlayer.playFullFile("/track101.mp3");
      headSetYellow();
      setDefaultPose();
      break;

    case 4:
      Serial.println(F("Case 4: Court assumes control + Blender"));
      headLookLeft();
      headSetRed();
      musicPlayer.playFullFile("/track401.mp3");
      musicPlayer.playFullFile("/track402.mp3");
      headLookCenter();
      break;

    case 5:
      Serial.println(F("Case 5: Logs"));
      musicPlayer.playFullFile("/track403.mp3");
      headSetGreen();
      break;

    case 6:
      Serial.println(F("Case 6: Preposterous"));
      headSetRed();
      musicPlayer.playFullFile("/track404.mp3");
      break;

    case 7:
      Serial.println(F("Case 7: Over a rug?"));
      headSetYellow();
      musicPlayer.playFullFile("/track405.mp3");
      break;

    case 8:
      Serial.println(F("Case 8: Impossible + NO nod"));
      headSetYellow();
      musicPlayer.playFullFile("/track406.mp3");
      headShakeNo();
      break;

    case 9:
      Serial.println(F("Case 9: Succulents"));
      musicPlayer.playFullFile("/track407.mp3");
      headSetDefault();
      setDefaultPose();
      break;

    case 10:
      Serial.println(F("Case 10: Without wifi"));
      cigarSmokeAnimation(4400);
      cigarOff();
      headSetDefault();
      musicPlayer.playFullFile("/track408.mp3");
      setDefaultPose();
      break;

    case 11:
      Serial.println(F("Case 11: Objection"));
      headSetRed();
      gavelBangOnce();
      musicPlayer.playFullFile("/track409.mp3");
      setDefaultPose();
      break;

    case 12:
      Serial.println(F("Case 12: No, I'm judicially"));
      cigarSmokeAnimation(800);
      musicPlayer.playFullFile("/track410.mp3");
      setDefaultPose();
      headSetYellow();
      break;

    case 13:
      Serial.println(F("Case 13: Court adjourned"));
      headSetGreen();
      gavelBangTwice();
      setDefaultPose();
      musicPlayer.playFullFile("/track411.mp3");
      break;

    case 14:
      Serial.println(F("Case 14: Court is now in session"));
      gavelBangOnce();
      musicPlayer.playFullFile("/track501.mp3");
      headSetDefault();
      setDefaultPose();
      break;

    case 15:
      headBlushAnimation(2000);
      Serial.println(F("Case 15: Salsa encourages (blushing)"));
      musicPlayer.playFullFile("/track502.mp3");
      setDefaultPose();
      break;

    case 16:
      Serial.println(F("Case 16: Child, colour wheel"));
      headLookRight();
      headColorWheelAnimation(2000);
      musicPlayer.playFullFile("/track503.mp3");
      break;

    case 17:
      headRedYellowAltAnimation(2000);
      headSetRed();
      Serial.println(F("Case 17: Guilty red/yellow alt"));
      setDefaultPose();
      break;

    case 18:
      Serial.println(F("Case 18: Child, that is priv (NO + blush)"));
      musicPlayer.playFullFile("/track504.mp3");
      headShakeNo();
      headBlushAnimation(2000);
      setDefaultPose();
      break;

    case 19:
      Serial.println(F("Case 19: Say something"));
      headSlowJitterToWife();
      musicPlayer.playFullFile("/track505.mp3");
      setDefaultPose();
      headRedYellowAltAnimation(2000);
      break;

    case 20:
      Serial.println(F("Case 20: We were studying"));
      headBlushAnimation(2000);
      musicPlayer.playFullFile("/track506.mp3");
      headLookLeft();
      break;

    case 21:
      Serial.println(F("Case 21: Court will be rec"));
      gavelBangOnce();
      musicPlayer.playFullFile("/track507.mp3");
      setDefaultPose();
      break;

    case 22:
      Serial.println(F("Case 22: Too late (cigar up, red ring)"));
      headBlushAnimation(2000);
      musicPlayer.playFullFile("/track508.mp3");
      moveServoSmooth(SERVO_CIGAR, CIGAR_UP_ANGLE, 3, 15);
      cigarSmokeAnimation(4400);
      setDefaultPose();
      break;

    default:
      Serial.println(F("Unknown state, resetting visuals"));
      setDefaultPose();
      break;
    }
  }
}
*/
