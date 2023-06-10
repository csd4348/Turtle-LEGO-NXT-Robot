#include <stdlib.h>
#include <assert.h>
#include "AT91SAM7S256.h"
#include "hwinit.h"
#include "pit.h"
#include "aic.h"
#include "display.h"
#include "sound.h"
#include "i2c.h"
#include "input.h"
#include "button.h"
#include "output.h"
#include "led.h"
#include "aclock.h"
#include "input.h"
#include "output.h"
#include "arm2avr.h"

#define MOVING_SPEED 70
#define STOP 0
#define MAX_UP 3
#define MAX_DOWN -1
#define SCARED 0
#define HAPPY 1
#define DOWN 0
#define UP 1
#define OPEN 0
#define CLOSED 1
#define INITIALLIGHT 700
#define INITIALSOUND 300

int speed1 = 70, speed2 = -70;
unsigned char flag = 0;
unsigned char flag1 = 0;
unsigned char isLedOn = 0;

int direction = 0;
int ldirection = DOWN;
int location = 0;
int mood = HAPPY;
int eyes = CLOSED;
int frames = 0;

void display_face(UBYTE y, UBYTE mood);

char *PrintButtons()
{
  enum button_t type = ButtonRead();

  switch (type)
  {
  case BUTTON_NONE:
    return "BUTTON_NONE";
  case BUTTON_LEFT:
    return "BUTTON_LEFT";
  case BUTTON_RIGHT:
    return "BUTTON_RIGHT";
  case BUTTON_ENTER:
    return "BUTTON_ENTER";
  case BUTTON_EXIT:
    return "BUTTON_EXIT";
  }

  return "ERROR";
}

void Button_Actions()
{
  enum button_t type = ButtonRead();

  switch (type)
  {
  case BUTTON_NONE:
    return;
  case BUTTON_LEFT:
    return;
  case BUTTON_RIGHT:
    return;
  case BUTTON_ENTER:
  {
    setMotors(0, 0);
    I2CCtrl(REPROGRAM);
  }
    return;
  case BUTTON_EXIT:
  {
    setMotors(0, 0);
    I2CCtrl(POWERDOWN);
  }
    return;
  }
}

void printInputSensor()
{
  DisplayString(0, 8, "P1-BTB: "); // btn Back
  DisplayNum(9 * 6, 8, IoFromAvr.AdValue[0]);
  DisplayString(0, 16, "P2-MIC: "); // Mic
  DisplayNum(8 * 6, 16, IoFromAvr.AdValue[1]);
  DisplayString(0, 24, "P3-LSL: "); // light :p3
  DisplayNum(8 * 6, 24, IoFromAvr.AdValue[2]);
  DisplayString(0, 32, "P4-BTF: "); // btn Front
  DisplayNum(8 * 6, 32, IoFromAvr.AdValue[3]);
}

void setMotors(int motor1_val, int motor2_val)
{
  OutputSetSpeed(0, motor1_val);
  OutputSetSpeed(1, motor2_val);
}

void setMotorsHead(int motor3_val)
{
  OutputSetSpeed(2, motor3_val);
}

void printMotors(int motor1_val, int motor2_val, int motor3_val)
{

  DisplayString(0, 40, "In-SND: "); // btn Front
  DisplayNum(8 * 6, 40, INITIALSOUND);
  DisplayString(0, 48, "In-lGH: "); // btn Front
  DisplayNum(8 * 6, 48, INITIALLIGHT);
}

void PIT_Handler()
{
  frames++;
  switch (location)
  {
  case MAX_UP:
    ldirection = DOWN;
    break;

  case MAX_DOWN:
    ldirection = UP;
    break;
  }

  switch (ldirection)
  {
  case UP:
    location++;
    break;

  default:
    location--;
    break;
  }

  if (mood == SCARED || (eyes == OPEN && frames == 40))
  {
    eyes = CLOSED;
    frames = 0;
  }
  else if (eyes == CLOSED && frames == 15){
    eyes = OPEN;
    frames = 0;
  }

  switch (mood)
  {
  case HAPPY:
    DisplayString(25, 0, (UBYTE *)"HI THERE!");
    break;

  default:
    DisplayString(0, 0, (UBYTE *)" DON'T SPOOK ME!");
    break;
  }
  display_face(location, mood);
}

void HandleMotion()
{
  // Button Front
  if (IoFromAvr.AdValue[3] < 200)
    direction = 1;

  // Button Back
  if (IoFromAvr.AdValue[0] < 200)
    direction = 0;

  // Move with closed lights
  if (IoFromAvr.AdValue[2] > INITIALLIGHT)
  {
    LedSwitchOn(2);
    isLedOn = 1;
    flag1 = 1;
  }
  else // Move with opened lights
  {
    LedSwitchOff(2);
    isLedOn = 0;
    flag1 = 0;
  }

  switch (isLedOn)
  {
  case 1:
    switch (direction)
    {
    case 0:
      speed1 = 70;
      speed2 = -70;
      break;

    case 1:
      speed1 = -70;
      speed2 = 70;
      break;
    }
    break;

  case 0:
    speed1 = 0;
    speed2 = 0;
    break;
  }

  // Stop if you hear something
  if (IoFromAvr.AdValue[1] < INITIALSOUND - 100)
  {
    flag = 1;
    speed1 = 0;
    speed2 = 0;
    mood = SCARED;
  }
  else if (flag == 1 && IoFromAvr.AdValue[1] > INITIALSOUND)
  { // Mic
    flag = 0;

    switch (flag1)
    {
    case 1:
      switch (direction)
      {
      case 0:
        speed1 = 80;
        speed2 = -80;
        break;
      case 1:
        speed1 = -80;
        speed2 = 80;
        break;
      }
      break;

    default:
      break;
    }
  }
  setMotors(speed1, speed2);
}

void display_face(UBYTE y, UBYTE mood)
{
  // Face
  AclockDisplayFrame(50, 33 + y, 25);

  switch (eyes)
  {
  case OPEN:
    // Right eye
    AclockDisplayFrame(40, 21 + y, 2);

    // Left eye
    AclockDisplayFrame(60, 21 + y, 2);
    break;

  case CLOSED:
    switch (mood)
    {
    case SCARED: // X eyes
      DisplayLineXY(38, 19 + y, 42, 23 + y);

      DisplayLineXY(58, 19 + y, 62, 23 + y);

      DisplayLineXY(42, 19 + y, 38, 23 + y);

      DisplayLineXY(62, 19 + y, 58, 23 + y);
      break;

    default: // -- eyes
      DisplayLineXY(38, 21 + y, 42, 21 + y);
      DisplayLineXY(58, 21 + y, 62, 21 + y);
      break;
    }
  }

  switch (mood)
  {
  case HAPPY: // Mouth Happy
    AclockDisplayHalfFrame(50, 33 + y, 16, 1);
    break;

  default: // Mouth Sad
    AclockDisplayHalfFrame(50, 49 + y, 16, 0);
    break;
  }
}

int main(void)
{
  LedSwitchOff(2);
  HardwareInit(); // need this to init PIOA clock
  DisplayInit();
  PITEnable();
  AICInit();
  SoundInit();
  I2CInit();
  InputInit();
  ButtonInit();
  OutputInit();

  flag = 0;
  int stopMs = 100;
  int headMs = 0;
  setMotors(0, 0);
  int isScared = 0;
  int speed3 = 0;
  int isHeadStoped = 0;
  while (1)
  {
    DisplayErase();
    Button_Actions();
    PIT_Handler();

    if (isScared == 0 && (IoFromAvr.AdValue[1] < INITIALSOUND - 100))
    {
      isScared = 1;
      headMs = 0;
      speed3 = -70;
      mood = SCARED;
      stopMs = 1500;
      isHeadStoped = 0;
    }

    if (!isHeadStoped && ++headMs == 80)
    {
      isScared = 0;
      stopMs = 0;
      mood = HAPPY;
    }

    if (++stopMs < 20)
    {
      speed3 = 70;
      headMs = 0;
      isHeadStoped = 0;
    }
    else if (stopMs == 20)
    {
      isHeadStoped = 1;
      speed3 = 0;
    }

    setMotorsHead(speed3);

    switch (isScared)
    {
    case 1:
      setMotors(0, 0);
      break;

    default:
      HandleMotion();
      break;
    }

    DisplayUpdateSync();
    I2CTransfer();
  }

  ButtonExit();
  InputExit();
  I2CExit();
  PITDisable();
  SoundExit();
  DisplayExit();
  return 0;
}
