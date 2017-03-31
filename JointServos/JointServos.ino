#include "XBOXRECV.h" // Xbox wireless controller code
#include <Servo.h>
#include "enums.h"    // Contains definition of Direction enum
#include "joint.h"    // Contains structure for type joint

// Below include statements are required by Xbox controller code
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>///this is for the teensy
#include <SPI.h>
#endif
// End Xbox requirements

USB Usb;
XBOXRECV Xbox(&Usb);

//int ledPin = 0;

// Default Joint values
// (_d is short for "default")
// See below for explanations
const int controllerNum_d = 0;
const int deadZone_d      = 3500;
const int stepSize_d      = 3;
const int angleMin_d      = 0;
const int angleMax_d      = 180;

// Time values
unsigned long currentmillis;
unsigned long previousmillis;
unsigned long ontime = 45;

// Joints
// Add new servo here!

Joint Grip;
Joint Tilt;
Joint Actuator;
Joint Back;

/* --------------------------------- Functions --------------------------------- */

// Set up joint. Additionally, either setButtons() or setStick() must be called to choose input method
void initializeJoint(Joint *joint, String jointName, int pin, int angleMin, int angleMax, int stepSize, int controllerNum)
{
  joint->jointName = jointName;
  joint->angleMin = angleMin;
  joint->angleMax = angleMax;
  joint->stepSize = stepSize;
  joint->controllerNum = controllerNum;
  joint->pin = pin;
  joint->servo.attach(pin);
  joint->currentPosition = joint->servo.read();   // Set current position of Servo
  Serial.print(F("Initializing: Joint "));        // Servo always resets to 93
  Serial.print(jointName);
  Serial.print(F(" is at location "));
  Serial.println(joint->currentPosition);
}


// Set two buttons to control joint
void setButtons(Joint *joint, ButtonEnum clockwise, ButtonEnum ctrclockwise)
{
  joint->clockwiseBtn = clockwise;
  joint->ctrclockwiseBtn = ctrclockwise;
  joint->isStick = false;
}


// Set a single axis of an analog stick to control joint
void setStick(Joint *joint, AnalogHatEnum stickAxis, bool upOrRightIsClockwise, int deadZone)
{
   joint->isStick = true;
   joint->stickAxis = stickAxis;
   joint->upOrRightIsClockwise = upOrRightIsClockwise;
   joint->deadZone = deadZone;
}


// Check buttons or stick for input; return direction
Direction getInputDirection (Joint *joint)
{
  if (joint->isStick)
  {
    int16_t stickVal = Xbox.getAnalogHat(joint->stickAxis, joint->controllerNum);
    Serial.print(F("StickVal: "));
    Serial.println(stickVal);
    stickVal = stickVal / 2;  // Larger negative values cause problems for abs()
    if (abs(stickVal) < joint->deadZone)
    {
      return NONE;
    }
    if (joint->upOrRightIsClockwise)
    {
      if (stickVal > 0) { return CLOCKWISE; }
      return COUNTERCLOCKWISE;
    }
    else
    {
      if (stickVal > 0) { return COUNTERCLOCKWISE; }
      return CLOCKWISE;
    }
  }
  else
  {
    bool c_btn = Xbox.getButtonPress(joint->clockwiseBtn, joint->controllerNum);
    bool cw_btn = Xbox.getButtonPress(joint->ctrclockwiseBtn, joint->controllerNum);
    if (c_btn && cw_btn) { return NONE; }
    else if (c_btn)
    {
      Serial.println(F("Clockwise input"));
      return CLOCKWISE; }
    else if (cw_btn)
    {
      Serial.println(F("Counterclockwise input"));
      return COUNTERCLOCKWISE;
    }
    return NONE;
  }
}


// Returns true if joint can safely move in given direction according to its parameters, false otherwise
bool canMove(Joint *joint, Direction d)
{
  if (d == NONE) { return false; }

  //int sign = (d == CLOCKWISE) ? 1 : -1 ;

  if (d == CLOCKWISE) {
    if (joint->currentPosition + joint->stepSize < joint->angleMax)
    {
      Serial.println(F("canMove says joint can move Clockwise"));
      return true;
    }
    Serial.println(F("canMove says joint cannot move Clockwise"));
    return false;
  }
  else if (d == COUNTERCLOCKWISE)
  {
    if (joint->currentPosition - joint->stepSize > joint->angleMin)
    {
      Serial.println(F("canMove says joint can move CounterClockwise"));
      return true;
    }
    Serial.println(F("canMove says joint cannot move CounterClockwise"));
    return false;
  }
}


// Move joint servo in given direction, checking for safety
void moveServo(Joint *joint, Direction d)
{
  if (d == NONE)
  {
    Serial.print("Function moveServo() called with direction NONE and joint ");
    Serial.println(joint->jointName);
    return;
  }

  char* directionStr = (d == CLOCKWISE) ? "Clockwise" : "Counterclockwise";

  if ( ! canMove(joint, d))
  {
    Serial.print(joint->jointName);
    Serial.print(" cannot move ");
    Serial.println(directionStr);
    return;
  }

  int sign = (d == CLOCKWISE) ? 1 : -1 ;
  int delta = joint->stepSize * sign;

  joint->servo.write(joint->currentPosition + delta);

  Serial.print(joint->jointName);
  Serial.print(F(" moved "));
  Serial.print(joint->stepSize);
  Serial.print(F(" degrees "));
  Serial.print(directionStr);
  Serial.print(F(" to position "));
  Serial.println(joint->currentPosition);
  // e.g. "Left Elbow moved 5 degrees Clockwise
}


// Checks joint for input, makes move if input given and safe to do so
void checkJoint(Joint *joint)
{
  Direction d = getInputDirection(joint);
  if (d != NONE)
  {
    moveServo(joint, d);
  }
  joint->currentPosition = joint->servo.read();
}


/* --------------------------------- Main ---------------------------------  */


void setup() {
  Serial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);

  if (Usb.Init() == -1)
  {
    Serial.print(F("USB Device failed to start. Execution will now halt."));
    while (1); //halt
  }
  Serial.println(F("\r\nXBOX USB Library Started"));

  // For reference:
  // void initializeJoint(Joint *joint, String jointName, int pin, int angleMin, int angleMax, int stepSize, int controllerNum)
  // void setButtons(Joint *joint, ButtonEnum clockwise, ButtonEnum ctrclockwise)
  // void setStick(Joint *joint, AnalogHatEnum stickAxis, bool upOrRightIsClockwise, int deadZone)

  initializeJoint(&Grip, "Grip", 4, angleMin_d, angleMax_d, stepSize_d, controllerNum_d);
  setButtons(&Grip, A, B);

  initializeJoint(&Tilt, "Tilt", 2, angleMin_d, angleMax_d, stepSize_d, controllerNum_d);
  setStick(&Tilt, LeftHatX, true, deadZone_d);

  initializeJoint(&Actuator, "Actuator", 9, angleMin_d, angleMax_d, stepSize_d, controllerNum_d);
  setButtons(&Actuator, L2, R2);

  // On USB Shield, Pins 10 and 11 do not work. Pins 2, 4, 8, and 9 have been tested to work.

  // Special setup for "one-way servo"
  initializeJoint(&Back, "Back Flipper", 3, -180, 10, 4, controllerNum_d);
  setButtons(&Back, X, Y);
  while(Back.currentPosition > 5)
  {
    Back.servo.write(Back.currentPosition - 5);
    Back.currentPosition = Back.servo.read();
  }
  // End special setup
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  // I don't know what this does, but the USB Shield requires it
  Usb.Task();


  if (Xbox.XboxReceiverConnected != 1) {
    Serial.print(F("Waiting for Xbox Receiver connection... \n"));
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);            // Go easy on the power supply
    return;                 // Wait until receiver is connected
  }

  // Wait a reasonable amount of time between checking the button
  currentmillis = millis();
  if (currentmillis - previousmillis < ontime) {
    delay(20);
    return;
  }
  previousmillis = currentmillis;

  // Check the buttons/stick of each joint for input, and make move if it's safe to do so

  checkJoint(&Grip);
  checkJoint(&Tilt);
  checkJoint(&Actuator);
  checkJoint(&Back);
}
