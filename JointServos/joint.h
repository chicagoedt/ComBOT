typedef struct Joint_struct{
  String jointName;     // Printed in Serial messages to identify this joint
  Servo servo;          // Servo object is created by initializeJoint and does not need to be specified
  int pin;              // Which pin this servo is connected to
  int angleMin;         // Servo will be prevented from moving below this angle
  int angleMax;         // Servo will be prevented from moving above this angle
  int stepSize;         // Determines how far servo will attempt to rotate when moveServo() is called. Possibly a placebo.
  int currentPosition;  // Current position of Servo. Only updated by checkJoint()
  int controllerNum;    // Xbox can support up to 4 controllers. Default is 0.
  // Button:
  ButtonEnum clockwiseBtn;    // Clockwise and counterclockwise buttons
  ButtonEnum ctrclockwiseBtn; // See controllerEnums.h in the USB Host Shield library for all options
  // Stick:
  bool isStick;               // True if this joint takes input from a stick. False otherwise.
  AnalogHatEnum stickAxis;    // Valid options are: LeftHatX, LeftHatY, RightHatX, RightHatY
  bool upOrRightIsClockwise;  // True if up or left on stick corresponds to Clockwise rotation. False otherwise.
  int deadZone;               // Stick inputs (whose absolute value is) smaller than this value will be ignored
} Joint;
