Servo head;

//this pause function takes in a an integer and will pause for that many milliseconds
void pause(int pauseTime)
 {
  
  timeElapsed = millis();
  while (millis() < timeElapsed + pauseTime){}
  if(serialDebug==0)
    {
      Serial.print("Pausing for: "); 
      Serial.print(pauseTime);
      Serial.println(" milliseconds");
      Serial.println("");
      Serial.print("Time elapsed: ");
      Serial.println(timeElapsed);
      Serial.println(" milliseconds");
      Serial.println("");
    }
 }
//this pause function takes in an integer and will pause for that many microseconds
void microPause(int pauseTime)
 {
  timeElapsed = micros();
  while (micros() < timeElapsed + pauseTime){}
  if(serialDebug==0)
    {
      Serial.print("Pausing for: "); 
      Serial.print(pauseTime);
      Serial.println(" microseconds");
      Serial.println("");
      Serial.print("Time elapsed: ");
      Serial.print(timeElapsed);
      Serial.println(" microseconds");
      Serial.println("");
    }
 }

/*motor control*/
/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
  {
    analogWrite(speedPinL,speed_L); 
    analogWrite(speedPinR,speed_R);   
  }
void stop() 
  {
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2,LOW);
    digitalWrite(LeftDirectPin1,LOW);
    digitalWrite(LeftDirectPin2,LOW);
    set_Motorspeed(0,0);
  }
void goForward(void)  //Forward
  {
    set_Motorspeed(SPEED,SPEED);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2,LOW);
    digitalWrite(LeftDirectPin1,HIGH);
    digitalWrite(LeftDirectPin2,LOW);
  }
void turnLeft(int degree)  //Turn left
  {
    set_Motorspeed(TURN_SPEED, TURN_SPEED);
    int turntime = degree*turnRatio; //Time the robot spends turning (miliseconds)
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2,LOW);
    digitalWrite(LeftDirectPin1,LOW);
    digitalWrite(LeftDirectPin2,HIGH);
    pause(turntime);
    stop();
  }
void turnRight(int degree)  //Turn right
  {
    set_Motorspeed(TURN_SPEED, TURN_SPEED);
    int turntime = degree*turnRatio; //Time the robot spends turning (miliseconds)
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2,HIGH);
    digitalWrite(LeftDirectPin1,HIGH);
    digitalWrite(LeftDirectPin2,LOW);
    pause(turntime);
    stop();
  }

//update this so it takes in a number that reflects distance
//code should do the math for the user and will determine the 
//amount of time to pause and let the wheels spin to reach the desired disdtance
void goBack()  //Reverse
  {
    set_Motorspeed(BACK_SPEED1,BACK_SPEED1);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2,HIGH);
    digitalWrite(LeftDirectPin1,LOW);
    digitalWrite(LeftDirectPin2,HIGH);
    pause(backtime);
    stop();
  }

//these functions use for loops to cycle through the index of notes we created, thus playing a melody
//write new function called play() that takes in a string variable and plays the related melody
void playErrorMelody()  
  {
    // iterate over the notes of the errorMelody:
    for (int thisNote = 0; thisNote < 2; thisNote++) 
      {
        tone(peizoPin, errorMelody[thisNote], *noteDurations);
        // to distinguish the notes, set a minimum time between them.
        pause(pauseBetweenNotes);
        // stop the tone playing
        noTone(peizoPin);
      }
  }
void playGreatingMelody()  
  {
    // iterate over the notes of the errorMelody:
    for (int thisNote = 0; thisNote < 3; thisNote++) 
      {
        tone(peizoPin, greatingMelody[thisNote], *noteDurations);
        // to distinguish the notes, set a minimum time between them.
        pause(pauseBetweenNotes);
        // stop the tone playing
        noTone(peizoPin);
      }
  }
void buzz_ON()   
{
  for(int i=0;i<100;i++)
  {
   digitalWrite(peizoPin,LOW);
   pause(2);//wait for 1ms
   digitalWrite(peizoPin,HIGH);
   pause(2);//wait for 1ms
  }
}
void buzz_OFF()  
{
  digitalWrite(peizoPin, HIGH); 
}
void alarm()
{
   buzz_ON();
   buzz_OFF();
}

/*detection of ultrasonic distance*/
int watch()
  {
    long echo_distance;
    digitalWrite(Trig_PIN,LOW);
    microPause(5);                                                                              
    digitalWrite(Trig_PIN,HIGH);
    microPause(15);
    digitalWrite(Trig_PIN,LOW);
    echo_distance=pulseIn(Echo_PIN,HIGH);
    echo_distance=echo_distance*0.01657; //how far away is the object in cm
    Serial.print("Echo Distance: ");
    Serial.println((int)echo_distance);
    return round(echo_distance);
  }
void lookCenter()
  {
    head.write(center);//90 
    pause(servoTurnPause);//this gives the servo time to turn before trying to do anything else, can also slow the servo down so its not so spastic
    centerscanval = watch();
    centerscanval = watch();//we watch twice to account for false positves/negatives
    if(centerscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  = obstacle_status | B100;
    }
    if(serialDebug==1)
    {
      Serial.print("Center Scan distance: ");
      Serial.println(centerscanval);
      Serial.println("");
      Serial.print("Obstacle status: ");
      Serial.println(obstacle_status);
      Serial.println("");
    }
  }
void lookFullLeft()
  {
    head.write(fullLeft);//130
    pause(servoTurnPause);
    leftscanval = watch();
    leftscanval = watch();
    if(leftscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status = obstacle_status | B10000;
    }
    if(serialDebug==0)
    {
      Serial.print("Full left Scan distance: ");
      Serial.println(leftscanval);
      Serial.println("");
      Serial.print("Obstacle status: ");
      Serial.println(String(obstacle_status,BIN));
      Serial.println("");
    } 
  }
void lookHalfLeft()
  {
    head.write(halfLeft);
    pause(servoTurnPause);
    ldiagonalscanval = watch();
    ldiagonalscanval = watch();
    if(ldiagonalscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status = obstacle_status | B1000;
    }
    if(serialDebug==0)
    {
      Serial.print("Half left Scan distance: ");
      Serial.println(ldiagonalscanval);
      Serial.println("");
      Serial.print("Obstacle status: ");
      Serial.println(String(obstacle_status,BIN));
      Serial.println("");
    }
  }
void lookFullRight()
  {
    head.write(fullRight);//50
    pause(servoTurnPause);
    rightscanval = watch();
    rightscanval = watch();
    if(rightscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  = obstacle_status | B1;
    }
    if(serialDebug==0)
    {
      Serial.print("Full right Scan distance: ");
      Serial.println(rightscanval);
      Serial.println("");
      Serial.print("Obstacle status: ");
      Serial.println(String(obstacle_status,BIN));
      Serial.println("");
    }
  }
void lookHalfRight()
  {
    head.write(halfRight);//70
    pause(servoTurnPause);
    rdiagonalscanval = watch();
    rdiagonalscanval = watch();
    if(rdiagonalscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status = obstacle_status | B10;
    }
    if(serialDebug==0)
    {
      Serial.print("Half Right Scan distance: ");
      Serial.println(rdiagonalscanval);
      Serial.println("");
      Serial.print("Obstacle status: ");
      Serial.println(String(obstacle_status,BIN));
      Serial.println("");
    }
  }

//Meassures distances to the right, left, front, left diagonal, right diagonal and asign them in cm to the variables rightscanval, 
//leftscanval, centerscanval, ldiagonalscanval and rdiagonalscanval (there are 5 points for distance testing)
String watchsurrounding()
  {
    /*  obstacle_status is a binary integer, its last 5 digits stands for if there is any obstacles in 5 directions,
    *   for example B101000 last 5 digits is 01000, which stands for Left front has obstacle, B100111 means front, right front and right ha
    */
    int obstacle_status =B100000;
    centerscanval = watch();
    
    if(centerscanval<distancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | B100;
    }
    
    head.write(120);
    pause(100);
    ldiagonalscanval = watch();
    
    if(ldiagonalscanval<distancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | B1000;
    }
    
    head.write(170); //Didn't use 180 degrees because my servo is not able to take this angle
    pause(300);
    leftscanval = watch();
    
    if(leftscanval<sidedistancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | B10000;
    }

    head.write(90); //use 90 degrees if you are moving your servo through the whole 180 degrees
    pause(100);
    centerscanval = watch();
    
    if(centerscanval<distancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | B100;
    }
    
    head.write(40);
    pause(100);
    rdiagonalscanval = watch();
    
    if(rdiagonalscanval<distancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | B10;
    }
    
    head.write(0);
    pause(100);
    rightscanval = watch();
    
    if(rightscanval<sidedistancelimit)
    {
      stop();
      alarm();
      obstacle_status  =obstacle_status | 1;
    }
  
    head.write(90); //Finish looking around (look forward again)
    pause(300);
  
    String obstacle_str= String(obstacle_status,BIN);
    obstacle_str= obstacle_str.substring(1,6);
    return obstacle_str; //return 5-character string standing for 5 direction obstacle status
  }

void collisionAvoidance()
  {
    ++numcycles;
    if(numcycles>=LPT)
      { //Watch if something is around every LPT loops while moving forward 
        //stop(); removing this allows it to move forward while looking around
        String obstacle_sign=watchsurrounding(); // 5 digits of obstacle_sign binary value means the 5 direction obstacle status
        Serial.print("Obstacle Sign");
        Serial.println(obstacle_sign);

        if(obstacle_sign=="10000") //turn slight right
          {
            if(serialDebug==1)
              { 
                Serial.println("Turn slight right");
              }
            turnRight(15);
          }
        else if(obstacle_sign=="11000" ||  obstacle_sign=="01000" ) // turn right
          {
            if(serialDebug==1)
              { 
                Serial.println("Turning Right");
              }
            turnRight(30);
          }   
        if(obstacle_sign=="11100" || obstacle_sign=="10100"  || obstacle_sign=="01100") //turn hard right
          {
            if(serialDebug==1)
              { 
                Serial.println("Turn slight right");
              }
            turnRight(60);
          }
        else if(obstacle_sign=="00001") // turn slight left
          {
            if(serialDebug==1)
              { 
                Serial.println("Turn slight left");
              }
            turnLeft(15);
          }
        else if(obstacle_sign=="00010" ||  obstacle_sign=="00011"  ) // turn left
        {
          if(serialDebug==1)
              { 
                Serial.println("Turning Left");
              }
            turnLeft(30);
        }
        if(obstacle_sign=="00111" || obstacle_sign=="00101" || obstacle_sign=="00110") //turn hard left
          {
            if(serialDebug==1)
              { 
                Serial.println("Turn slight right");
              }
            turnRight(60);
          }
        else if(obstacle_sign=="01111" ||  obstacle_sign=="10111" || obstacle_sign=="11110" ||  obstacle_sign=="11101" || obstacle_sign=="11011" || obstacle_sign=="01110" || obstacle_sign=="11111" ||  obstacle_sign=="01010" || obstacle_sign=="00100") // turn around
          {
            if(serialDebug==1)
                { 
                  Serial.println("Turning Back");
                }
            turnRight(90);
          }   
        
        else Serial.println("no handle");
        numcycles=0; //Restart count of cycles
      } 
    else 
      {
        if(serialDebug==1)
          { 
            Serial.println("Going Forward");
          }
        goForward();
      }
    distance = watch(); // use the watch() function to see if anything is ahead (when the robot is just moving forward and not looking around it will test the distance in front)
    // The robot will just stop if it is completely sure there's an obstacle ahead (must test 25 times) (needed to ignore ultrasonic sensor's false signals)  
    if (distance<distancelimit)
      { 
        Serial.println("Obstacle?");
        ++thereis;
      }
    if (distance>distancelimit)
      {
        Serial.println("No obstacle");
        thereis=0;//Count is restarted
      } 
    if (thereis > 25)
      {
        Serial.println("Obstacle confirmed");
        stop(); // Since something is ahead, stop moving.
        thereis=0;
      }
  }

void testDance()
  {
    lookCenter();
    
    turnLeft(45);
    lookHalfLeft();
    
    turnLeft(45);
    lookFullLeft();
    
    turnRight(45);
    lookHalfLeft();
    
    turnRight(45);
    lookCenter();
    
    turnRight(45);
    lookHalfRight();
    
    turnRight(45);
    lookFullRight();
    
    turnLeft(45);
    lookHalfRight();
    
    turnLeft(45);
    lookCenter();
  }
