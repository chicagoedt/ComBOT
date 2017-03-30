#include "Joint.h"

// Set up joint. Additionally, either setButtons() or setStick() must be called to choose input method
void Joint::initialize(XBOXRECV *Xbox, Frame *frame, String jointName, int pin, int angleMin, int angleMax, int stepSize, int controllerNum)
{
  this->frame = frame;
  this->jointName = jointName;
  this->angleMin = angleMin;
  this->angleMax = angleMax;
  this->stepSize = stepSize;
  this->controllerNum = controllerNum;
  this->pin = pin;
  this->servo.attach(pin);
  this->currentPosition = this->servo.read();   // Set current position of Servo
}


// Set two buttons to control joint
void Joint::setButtons(ButtonEnum clockwise, ButtonEnum ctrclockwise)
{
  this->clockwiseBtn = clockwise;
  this->ctrclockwiseBtn = ctrclockwise;
  this->isStick = false;
}


// Set a single axis of an analog stick to control joint
void Joint::setStick(AnalogHatEnum stickAxis, bool upOrRightIsClockwise, int deadZone)
{
   this->isStick = true;
   this->stickAxis = stickAxis;
   this->upOrRightIsClockwise = upOrRightIsClockwise;
   this->deadZone = deadZone;
}


// Check buttons or stick for input; return direction
Direction Joint::getInputDirection ()
{
  Serial.println(F("Checking input"));
  if (isStick)
  {
    int16_t stickVal = Xbox->getAnalogHat(stickAxis, controllerNum);
    Serial.println(F("StickVal is"));
    Serial.println(stickVal);
    if (abs(stickVal) < deadZone) { return NONE; }
    if (upOrRightIsClockwise)
    {
      if (stickVal > deadZone) { return CLOCKWISE; }
      return COUNTERCLOCKWISE;
    }
    else
    {
      if (stickVal > deadZone) { return COUNTERCLOCKWISE; }
      return CLOCKWISE;
    }
  }
  else
  {
    bool c_btn = Xbox->getButtonPress(clockwiseBtn, controllerNum);
    bool cw_btn = Xbox->getButtonPress(ctrclockwiseBtn, controllerNum);
    Serial.println(F("C and Cw buttons are"));
    Serial.println(c_btn + " " + cw_btn);
    if (c_btn && cw_btn) { return NONE; }
    else if (c_btn) { return CLOCKWISE; }
    else if (cw_btn) { return COUNTERCLOCKWISE; }
    return NONE;  
  }
}


// Returns true if joint can safely move in given direction according to its parameters, false otherwise
bool Joint::canMove(Direction d)
{
  if (d == NONE) { return false; }
  
  //int sign = (d == CLOCKWISE) ? 1 : -1 ;

  if (d == CLOCKWISE) {
    if (currentPosition + stepSize > angleMax) { return false; }
  }
  else if (d == COUNTERCLOCKWISE)
  {
    if (currentPosition - stepSize < angleMax) { return false; }
  }
  else { return false; }

  return frame->canMove(this, d);
}


// Move joint servo in given direction, checking for safety
void Joint::moveServo(Direction d) 
{
  if (d == NONE)
  { 
    Serial.print(F("Function moveServo() called with direction NONE and joint "));
    Serial.println(jointName);
    return;
  }

  char* directionStr = (d == CLOCKWISE) ? "Clockwise" : "Counterclockwise";
  
  if ( ! canMove(d)) 
  {
    Serial.print(jointName);
    Serial.print(F(" cannot move "));
    Serial.println(directionStr);
    return; 
  }
  
  int sign = (d == CLOCKWISE) ? 1 : -1 ;
  int delta = stepSize * sign;

  servo.write(currentPosition + delta);

  Serial.print(jointName);
  Serial.print(F(" moved "));
  Serial.print(stepSize);
  Serial.print(F(" degrees "));
  Serial.println(directionStr);
  // e.g. "Left Elbow moved 5 degrees Clockwise
}


// Checks joint for input, makes move if input given and safe to do so
void Joint::check()
{
  //Serial.print(F("Checking"));
  //Serial.println(jointName);
  Direction d = getInputDirection();
  if (d != NONE)
  {
    moveServo(d);
  }
  currentPosition = servo.read();
}



void Joint::moveByAngle(Direction d, int angle)
{
  if (d == NONE) { return; }
  
  char* directionStr = (d == CLOCKWISE) ? "Clockwise" : "Counterclockwise";
  
  if (d == CLOCKWISE)
  {
    if(currentPosition + angle < angleMax)
    {
      servo.write(currentPosition + angle);
      
      Serial.print(jointName);
      Serial.print(F(" moved "));
      Serial.print(angle);
      Serial.print(F(" degrees "));
      Serial.println(directionStr);
    }
    else if ( currentPosition < angleMax)
    {
      servo.write(angleMax);

      Serial.print(jointName);
      Serial.print(F(" moved "));
      Serial.print(directionStr);
      Serial.print(F(" to maximum angle "));
      Serial.println(angleMax);      
    }
  }
  else if (d == COUNTERCLOCKWISE)
  {
    if (currentPosition - angle > angleMin)
    {
      servo.write(currentPosition - angle);
      
      Serial.print(jointName);
      Serial.print(F(" moved "));
      Serial.print(angle);
      Serial.print(F(" degrees "));
      Serial.println(directionStr);
    }
    else if ( currentPosition > angleMin)
    {
      servo.write(angleMin);

      Serial.print(jointName);
      Serial.print(F(" moved "));
      Serial.print(directionStr);
      Serial.print(F(" to minimum angle "));
      Serial.println(angleMax);    
    }
  }
  currentPosition = servo.read();
}


Frame::Frame()
{
  arrSize = 0;
  arrMax = 5;
  constraints = (ConstraintElem*) malloc (sizeof(ConstraintElem) * arrMax);
}


void Frame::growArr()
{
  arrMax += 5;
  ConstraintElem *temp = (ConstraintElem*) malloc (sizeof(ConstraintElem) * arrMax);
  for (int i = 0; i < arrSize; i++)
  {
    temp[i] = constraints[i];
  }
  free(constraints);
  constraints = temp;
}


void Frame::addConstraint(Joint *blockingJoint, int blockingAngleMin, int blockingAngleMax, Joint *blockedJoint, int blockedAngleMin, int blockedAngleMax)
{
  if (arrSize + 1 >= arrMax) { growArr(); }

  constraints[arrSize].blockingJoint = blockingJoint;
  constraints[arrSize].blockingAngleMin = blockingAngleMin;
  constraints[arrSize].blockingAngleMax = blockingAngleMax;
  constraints[arrSize].blockedJoint = blockedJoint;
  constraints[arrSize].blockedAngleMin = blockedAngleMin;
  constraints[arrSize].blockedAngleMax = blockedAngleMax;

  arrSize++;
}


bool Frame::canMove(Joint *blockedJoint, Direction d)
{
  int blockingPosition;
  int blockedPosition;
  for (int i = 0; i < arrSize; i++)
  {
    if ( constraints[i].blockedJoint == blockedJoint)
    {
      blockingPosition = constraints[i].blockingJoint->currentPosition;
      if (blockingPosition > constraints[i].blockingAngleMin && blockingPosition < constraints[i].blockingAngleMax)
      {
        blockedPosition = blockedJoint->currentPosition;
        if (blockedPosition > constraints[i].blockedAngleMin && blockedPosition < constraints[i].blockedAngleMax)
        {
          return false;
        }
      }
    }
  }
  return true;
}


