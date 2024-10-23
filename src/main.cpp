#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>

// MLM 1
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

const unsigned long SECOND = 1000;
const unsigned long MINUTE = 60*SECOND;
const unsigned long HOUR = 3600*SECOND;
const unsigned long REVOLUTION = 200;

void SingleCoil();
void DoubleCoilFwd(int);
void DoubleCoilRev();
void InterleavedCoil();
void MicrostepCoil();
void AlertTheCat();
const int buzzerPin = 13;
void setup() 
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  while (!Serial);
  Serial.println("Stepper test!");

  pinMode(buzzerPin, OUTPUT);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  myMotor->setSpeed(50);  // 50 rpm
}

void loop() {

  AlertTheCat();
  
//SingleCoil();
  DoubleCoilRev();
  DoubleCoilFwd(REVOLUTION*2);
  DoubleCoilRev();
//InterleavedCoil();
//MicrostepCoil();
  
  myMotor->release();
  delay(1*MINUTE);
  
}

void SingleCoil()
{
  Serial.println("Single coil steps");
  myMotor->step(REVOLUTION, FORWARD, SINGLE);
  myMotor->step(REVOLUTION, BACKWARD, SINGLE);
}

void InterleavedCoil()
{
  Serial.println("Interleave coil steps");
  myMotor->step(100, FORWARD, INTERLEAVE);
  myMotor->step(100, BACKWARD, INTERLEAVE);
}

void DoubleCoilFwd(int rev)
{
  Serial.println("Double coil steps");
  for (int i=0;i < 5; i++) {
  myMotor->step(rev, FORWARD, DOUBLE);
  //  myMotor->step(100, BACKWARD, DOUBLE);
  }
}

void DoubleCoilRev()
{
  Serial.println("Double coil steps");
  for (int i=0;i < 5; i++) {
  //myMotor->step(REVOLUTION, FORWARD, DOUBLE);
  myMotor->step((REVOLUTION/4), BACKWARD, DOUBLE);
  }
}
void MicrostepCoil()
{
  Serial.println("Microstep steps");
  myMotor->step(50, FORWARD, MICROSTEP);
  myMotor->step(50, BACKWARD, MICROSTEP);
}

void AlertTheCat()
{
  Serial.println("Calling All Cats!");
  for (int t=0;t <2; t++) {
    for (int i=1;i < 4; i++) {
      tone(buzzerPin, (500*(0.5*i)));
      delay(600);
      noTone(buzzerPin);
      delay (50);
    }
  }
}
