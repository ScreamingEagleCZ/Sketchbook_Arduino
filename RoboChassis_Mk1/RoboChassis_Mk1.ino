//Libraries
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// Define the stepper motors and the pins the will use
AccelStepper LeftBackWheel(1, 42, 43);   // (Type:driver, STEP, DIR) - Stepper1
AccelStepper LeftFrontWheel(1, 40, 41);  // Stepper2
AccelStepper RightBackWheel(1, 44, 45);  // Stepper3
AccelStepper RightFrontWheel(1, 46, 47); // Stepper4

//Data Structure and auxiliary variables forcommunication
struct TDATA {
  byte Hedding;
  byte CarSpeed;
  byte Servo1;
  byte Servo2;
  byte Servo3;
  byte Servo4;
  byte Servo5;
  byte Servo6;
  byte ServoSpeed;
  byte CameraX;
  byte CameraY;
};

int index;
TDATA data;
byte Buffer[16];
unsigned long actual;
unsigned long last;
unsigned long timeout;
int DataSize;

//Create Serial communication for bluetooth module
SoftwareSerial hc06(10, 11); // RX, TX - propabbly TX,RX in case of HC06 - if not working swap those I/Os between themselves

void setup() {
  //Initialization of communication - bluetooth + serial, reset variables in communication data
  Serial.begin(9600);
  Serial.println("Booting up...");
  hc06.begin(115200);
  while (!hc06) {}
  index = 0;
  timeout = 50;
  DataSize = sizeof(data);
  data.Hedding = 0;
  data.CarSpeed = 0;
  data.Servo1 = 0;
  data.Servo2 = 0;
  data.Servo3 = 0;
  data.Servo4 = 0;
  data.Servo5 = 0;
  data.Servo6 = 0;
  data.ServoSpeed = 0;
  data.CameraX = 0;
  data.CameraY = 0;
  
  //PWM servo shield setup
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // Set initial seed values for the steppers
  LeftFrontWheel.setMaxSpeed(3000);
  LeftBackWheel.setMaxSpeed(3000);
  RightFrontWheel.setMaxSpeed(3000);
  RightBackWheel.setMaxSpeed(3000);
}

void loop() {
  //Bluetooth communication
  if (hc06.available() > 0)
  {
    last = millis() + timeout;
    Buffer[index] = hc06.read();
    index++;
  }

  if (millis() > last)
  {
    index = 0;
  }
  if (index > DataSize - 1)
  {
    memcpy(&data, Buffer, DataSize);  //saves data from buffer to data

    //Send information back to Android device regarding position and states received (remove after commisioning)
    hc06.println(sizeof(data));
    hc06.println(data.Hedding);
    hc06.println(data.CarSpeed);
    hc06.println(data.Servo1);
    hc06.println(data.Servo2);
    hc06.println(data.Servo3);
    hc06.println(data.Servo4);
    hc06.println(data.Servo5);
    hc06.println(data.Servo6);
    hc06.println(data.ServoSpeed);
    hc06.println(data.CameraX);
    hc06.println(data.CameraY);
    index = 0;
    Serial.println(data.Hedding); //prints value of heading to serial monitor (for testing latency)
  }
  //Leftover from copy from Arduino Mecanum Wheels Robot chassis
  //LeftFrontWheel.runSpeed();
  //LeftBackWheel.runSpeed();
  //RightFrontWheel.runSpeed();
  //RightBackWheel.runSpeed();
  
  //Move Servos to positions
  pwm.setPWM(1, 0, map(data.Servo1,0,180,SERVOMIN,SERVOMAX));
  pwm.setPWM(2, 0, map(data.Servo2,0,180,SERVOMIN,SERVOMAX));
  pwm.setPWM(3, 0, map(data.Servo3,0,180,SERVOMIN,SERVOMAX));
  pwm.setPWM(4, 0, map(data.Servo4,0,180,SERVOMIN,SERVOMAX));
  pwm.setPWM(5, 0, map(data.Servo5,0,180,SERVOMIN,SERVOMAX));
  pwm.setPWM(6, 0, map(data.Servo6,0,180,SERVOMIN,SERVOMAX));
}

//Functions for Stepper mottors movements
void moveForward() {
  LeftFrontWheel.setSpeed(data.CarSpeed);
  LeftBackWheel.setSpeed(data.CarSpeed);
  RightFrontWheel.setSpeed(data.CarSpeed);
  RightBackWheel.setSpeed(data.CarSpeed);
}
void moveBackward() {
  LeftFrontWheel.setSpeed(-data.CarSpeed);
  LeftBackWheel.setSpeed(-data.CarSpeed);
  RightFrontWheel.setSpeed(-data.CarSpeed);
  RightBackWheel.setSpeed(-data.CarSpeed);
}
void moveSidewaysRight() {
  LeftFrontWheel.setSpeed(data.CarSpeed);
  LeftBackWheel.setSpeed(-data.CarSpeed);
  RightFrontWheel.setSpeed(-data.CarSpeed);
  RightBackWheel.setSpeed(data.CarSpeed);
}
void moveSidewaysLeft() {
  LeftFrontWheel.setSpeed(-data.CarSpeed);
  LeftBackWheel.setSpeed(data.CarSpeed);
  RightFrontWheel.setSpeed(data.CarSpeed);
  RightBackWheel.setSpeed(-data.CarSpeed);
}
void rotateLeft() {
  LeftFrontWheel.setSpeed(-data.CarSpeed);
  LeftBackWheel.setSpeed(-data.CarSpeed);
  RightFrontWheel.setSpeed(data.CarSpeed);
  RightBackWheel.setSpeed(data.CarSpeed);
}
void rotateRight() {
  LeftFrontWheel.setSpeed(data.CarSpeed);
  LeftBackWheel.setSpeed(data.CarSpeed);
  RightFrontWheel.setSpeed(-data.CarSpeed);
  RightBackWheel.setSpeed(-data.CarSpeed);
}
void moveRightForward() {
  LeftFrontWheel.setSpeed(data.CarSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(data.CarSpeed);
}
void moveRightBackward() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(-data.CarSpeed);
  RightFrontWheel.setSpeed(-data.CarSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftForward() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(data.CarSpeed);
  RightFrontWheel.setSpeed(data.CarSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftBackward() {
  LeftFrontWheel.setSpeed(-data.CarSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(-data.CarSpeed);
}
void stopMoving() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(0);
}



//paste destination







//Servo movements
//  pwm.setPWM(1, 0, 3);
//  pwm.setPWM(2, 0, 375);
//  pwm.setPWM(3, 0, 375);
//  pwm.setPWM(4, 0, 375);
//  pwm.setPWM(5, 0, 375);
//  pwm.setPWM(6, 0, 375);








