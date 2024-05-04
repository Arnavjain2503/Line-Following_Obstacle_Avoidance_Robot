#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

//hc-sr04 sensor
#define TRIGGER_PIN A4
#define ECHO_PIN A3
#define max_distance 50

//ir sensor
#define irLeft A2
#define irRight A1

//motor
#define MAX_SPEED 10
#define MAX_SPEED_OFFSET 20

Servo servo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, max_distance);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int distance = 0;
int leftDistance;
int rightDistance;
boolean object;

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  servo.attach(10);
  servo.write(90);
}

void loop() {
  
  if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    //forword
  }
  else if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 1 ) {
    objectAvoid();
    Serial.println("TL");
    //leftturn
    moveLeft();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    Serial.println("TR");
    //rightturn
    moveRight();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 1 ) {
    //Stop
    Stop();
  }
}

void objectAvoid() {
  distance = getDistance();
  if (distance <= 30) {
    //stop
    Stop();
    Serial.println("Stop");

    lookLeft();
    lookRight();
    delay(100);
    if (rightDistance <= leftDistance) {
      //left
      object = true;
      turn();
      Serial.println("moveLeft");
    } else {
      //right
      object = false;
      turn();
      Serial.println("moveRight");
    }
    delay(100);
  }
  else {
    //forword
    Serial.println("moveforword");
    moveForward2();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 100;
  }
  return cm;
}

int lookLeft () {
  //lock left
  servo.write(150);
  delay(500);
  leftDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("Left:");
  Serial.print(leftDistance);
  return leftDistance;
  delay(100);
}

int lookRight() {
  //lock right
  servo.write(30);
  delay(500);
  rightDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("   ");
  Serial.print("Right:");
  Serial.println(rightDistance);
  return rightDistance;
  delay(100);
}
void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
void moveForward() {
  motor1.setSpeed(90);
  motor2.setSpeed(90);
  motor3.setSpeed(90);
  motor4.setSpeed(90);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveForward2() {
  motor1.setSpeed(70);
  motor2.setSpeed(70);
  motor3.setSpeed(70);
  motor4.setSpeed(70);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.setSpeed(120);
  motor2.setSpeed(120);
  motor3.setSpeed(120);
  motor4.setSpeed(120);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
void moveRight() {
  motor1.setSpeed(150);
  motor4.setSpeed(150);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}
void moveLeft() {
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void moveRight2() {
  motor1.setSpeed(150);
  motor4.setSpeed(150);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}
void moveLeft2() {
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}
void turn() {
  if (object == false) {
    Serial.println("turn Right");
    moveBackward();
    delay(10);
    moveLeft();
    delay(400);
    moveForward();
    delay(800);
    moveRight();
    delay(600);
    if ((digitalRead(irRight) == 1) || (digitalRead(irLeft) == 1)) {
      Stop();
      delay(8000);
      loop();
    } 
    else {
      moveForward2();
    }
  }
  else {
    Serial.println("turn left");
    moveBackward();
    delay(10);
    moveRight();
    delay(400);
    moveForward();
    delay(800);
    moveLeft();
    delay(600);
    if ((digitalRead(irRight) == 1) || (digitalRead(irLeft) == 1)) {
      Stop();
      delay(8000);
      loop();
    } 
    else {
      moveForward2();
    }
  }
}