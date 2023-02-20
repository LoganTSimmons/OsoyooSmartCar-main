#include "pitches.h"

int serialDebug=1; //enter 1 for on, 0 for off

/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define speedPinR 5   // RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  2    //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  4    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4
#define LPT 2 // scan loop counter
#define SERVO_PIN     9  //servo connect to D9
#define Echo_PIN    11 // Ultrasonic Echo pin connect to D11
#define Trig_PIN    12  // Ultrasonic Trig pin connect to D12
#define peizoPin     13
#define FAST_SPEED  250     //both sides of the motor speed
#define SPEED  100     //both sides of the motor speed
#define TURN_SPEED  150     //both sides of the motor speed
#define BACK_SPEED1  255     //back speed
#define BACK_SPEED2  90     //back speed
#define distancelimit 30 //distance limit for obstacles in front
#define sidedistancelimit 30 //minimum distance in cm to obstacles at both sides (the car will allow a shorter distance sideways)
#define backtime 300 //Time the robot spends turning (miliseconds)
#define servoTurnPause 1000

int turnRatio=10;
int degree;
int obstacle_status = B100000;
int leftscanval, centerscanval, rightscanval, ldiagonalscanval, rdiagonalscanval, distance;
int numcycles = 0;
int thereis=0;
unsigned long timeElapsed = 0;

// notes in the errorMelody
int errorMelody[]={NOTE_G4, NOTE_C4};
int greatingMelody[]={NOTE_G4, NOTE_D5, NOTE_A5};
int noteDurations[]={200, 300, 400};
int pauseBetweenNotes = 500;

//values for turn radius
int center = 90;
int halfLeft = 110;
int fullLeft = 130;
int halfRight = 60;
int fullRight = 40;