#include <Stepper.h>

const int ledPin = 13;    // The red light marked with 'L' on the board is also connected to this.

const int potPin = A5;    // Potentiometer, input

const int btnLightPin = 2;  // Button light, output
const int btnPin =  A0;     // Button input

// Stepper motor requires all 4 pins.
const int motorPin1 = 8; 
const int motorPin2 = 9;
const int motorPin3 = 10; 
const int motorPin4 = 11;

const int stepsPerRevolution = 200; // A guess based on motor documentation.
const int gearRatio = 19; // 19 motor rotations is 1 shaft rotation.
const int stepperSpeed = 100; // Suitable speed depends on power supply voltage and steps per revolution.
Stepper stepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4);

enum modeEnum {Manual, Auto, Pause};
enum modeEnum mode = Manual;

enum autoDirEnum {CW, CCW};
enum autoDirEnum autoDir = CW;

// Store the previous value of potentiometer and motor "position" 
//  to detect their change.
// Values are initialized in setup(). Unit is "steps".
int potPos;
int prevPotPos;
int potMoveThreshold = 10; // How much pot needs to move before motor engages.
int motorPos = 0; // Motor startign position is assumed to be in the middle when system is powered on.

// Set the limits for potentiometer and motor.
// Effectively +- 1/3rd rotation of shaft in each direction
const int maxPos = stepsPerRevolution * gearRatio / 3 / 10; // All the way to right (div by 10)
const int minPos = -maxPos; // All the way to left (div by 10)


void setup() {
  // Inputs
  pinMode(potPin, INPUT);
  pinMode(btnPin, INPUT);
  
  // Outputs
  pinMode(ledPin, OUTPUT);
  pinMode(btnLightPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  stepper.setSpeed(stepperSpeed); 

  // Printing to serial monitor
  Serial.begin(9600);
  Serial.println("Hello");

  // Wait a bit before starting
  delay(3000);
}

void loop() {
  if(HasPotBeenMoved()) // Pot has been moved to engage manual mode
  {
    mode = Manual;
    digitalWrite(btnLightPin, LOW);
  }
  else if(digitalRead(btnPin) == HIGH) // Button has been pressed to engage Auto mode
  {
    if(mode == Manual)
    {
      if(potPos >= 0)
      {
        autoDir = CW;
      }
      else
      {
        autoDir = CCW;
      }
      mode = Auto;
    }
    else if(mode == Auto)
    {
      mode = Pause;
    }
    else // Current mode is Pause -> Continue Auto
    {
      mode = Auto;
    }

    while(digitalRead(btnPin) == HIGH)
    {
      BlinkBtnLight(100, 2);
      delay(200);
    }
  }
  
  if(mode == Manual)
  {
    if(HasPotBeenMoved())
    {      
      stepper.step(10 * (motorPos - potPos));
      motorPos = potPos;
      prevPotPos = potPos;
    }
    else
    {
      delay(100);
    }
  }
  else if(mode == Auto)
  {
    OperateAuto();
  }
}


void OperateAuto()
{
  int nSteps;
  if(autoDir == CW) // If going right, go all the way.
  {
    // Move 1/10th towards max pos. Make sure we don't go over.
    nSteps = min(maxPos / 10, maxPos - motorPos);  
  }
  else // If on left-hand side.
  {
    // Move 1/10th towards min pos. Make sure we don't go over.
    nSteps = max(minPos / 10, minPos - motorPos); // Values are negative
  }

  stepper.step(10 * nSteps);
  motorPos += nSteps;

  // When the end is reached, start turning to the other direction.
  if(motorPos >= maxPos)
  {
    autoDir = CCW;
    delay(100);
  }
  else if(motorPos <= minPos)
  {
    autoDir = CW;
    delay(100);
  }
}


bool HasPotBeenMoved()
{
  potPos = map(analogRead(potPin), 0, 1023, minPos, maxPos);
  return abs(potPos - prevPotPos) >= potMoveThreshold;
}

bool BlinkBtnLight(int delayAmount, int nTimes)
{
  for(int i = 0; i < nTimes; ++i)
  {
    digitalWrite(btnLightPin, HIGH);
    delay(delayAmount);
    digitalWrite(btnLightPin, LOW);
    delay(delayAmount);
  }
}

void SetMotorPinsToLow()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
