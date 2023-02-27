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
    digitalWrite(RightDirectPin1,LOW);
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
void goBack()  //Reverse
  {
    set_Motorspeed(BACK_SPEED2,BACK_SPEED2);
    digitalWrite(RightDirectPin1,LOW);
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
void buzz_OFF()  //close buzzer
  {
    digitalWrite(peizoPin, HIGH);
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
    //Serial.print("Echo Distance: ");
    //Serial.println((int)echo_distance);
    return round(echo_distance);
  }
int floorWatch()
  {
    long floorEcho_distance; 
    digitalWrite(floorTrig_PIN,LOW);
    microPause(2);                                                                          
    digitalWrite(floorTrig_PIN,HIGH);
    microPause(2);
    digitalWrite(floorTrig_PIN,LOW);
    floorEcho_distance=pulseIn(floorEcho_PIN,HIGH);
    floorEcho_distance=floorEcho_distance*0.01657; //how far away is the object in cm
    //Serial.print("Floor Echo Distance: ");
    //Serial.println((int)floorEcho_distance);
    return round(floorEcho_distance);
  }

//Meassures distances to the right, left, front, left diagonal, right diagonal and asign them in cm to the variables rightscanval, 
//leftscanval, centerscanval, ldiagonalscanval and rdiagonalscanval (there are 5 points for distance testing)
String watchsurrounding()
  {
    /*
      obstacle_status is a binary integer, its last 5 digits stands for if there is any obstacles in 5 directions,
      for example B101000 last 5 digits is 01000, which stands for Left front has obstacle, B100111 means front, right front and right ha
    */

    int obstacle_status =B100000;

    centerscanval = watch();
    if(centerscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | B100;
    }

    head.write(135);
    pause(100);
    ldiagonalscanval = watch();
    if(ldiagonalscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | B1000;
    }
    
    head.write(180);
    pause(100);
    leftscanval = watch();
    if(leftscanval<sidedistancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | B10000;
    }

    head.write(90); 
    pause(100);
    centerscanval = watch();
    if(centerscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | B100;
    }
    
    head.write(45);
    pause(100);
    rdiagonalscanval = watch();
    if(rdiagonalscanval<distancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | B10;
    }
    
    head.write(0);
    pause(100);
    rightscanval = watch();
    if(rightscanval<sidedistancelimit)
    {
      stop();
      playErrorMelody();
      obstacle_status  =obstacle_status | 1;
    }
  
    head.write(90); 
    pause(100);
  
    String obstacle_str= String(obstacle_status,BIN);
    obstacle_str= obstacle_str.substring(1,6);
    return obstacle_str; 
  }

void collisionAvoidance()
  {

    floorDistance=floorWatch();
    if(floorDistance>3) //drop detected, back up and turn around
    {
      stop();
      playErrorMelody();
      goBack();
      turnRight(90);
      return;
    }
    
    String obstacle_sign=watchsurrounding(); // 5 digits of obstacle_sign binary value means the 5 direction obstacle status
    //Serial.print("Obstacle Sign: ");
    //Serial.println(obstacle_sign);
    //Serial.print("Floor Distance: ");
    //Serial.println(floorDistance);
    if(obstacle_sign=="10000") //turn slight right
      {
        if(serialDebug==1)
          { 
            Serial.println("Turn slight right");
          }
        turnRight(15);
        pause(turnTime); 
      }
    else if(obstacle_sign=="11000" ||  obstacle_sign=="01000" ) // turn right
      {
        if(serialDebug==1)
          { 
            Serial.println("Turning Right");
          }
        turnRight(30);
        pause(turnTime); 
      }   
    else if(obstacle_sign=="11100" || obstacle_sign=="10100"  || obstacle_sign=="01100") //turn hard right
      {
        if(serialDebug==1)
          { 
            Serial.println("Turn slight right");
          }
        turnRight(60);
        pause(turnTime); 
      }
    else if(obstacle_sign=="00001") // turn slight left
      {
        if(serialDebug==1)
          { 
            Serial.println("Turn slight left");
          }
        turnLeft(15);
        pause(turnTime); 
      }
    else if(obstacle_sign=="00010" ||  obstacle_sign=="00011"  ) // turn left
    {
      if(serialDebug==1)
          { 
            Serial.println("Turning Left");
          }
        turnLeft(30);
        pause(turnTime); 
    }
    else if(obstacle_sign=="00111" || obstacle_sign=="00101" || obstacle_sign=="00110") //turn hard left
      {
        if(serialDebug==1)
          { 
            Serial.println("Turn slight right");
          }
        turnRight(60);
        pause(turnTime); 
      }
    else if(obstacle_sign=="01111" ||  obstacle_sign=="10111" || obstacle_sign=="11110" ||  obstacle_sign=="11101" || obstacle_sign=="11011" || obstacle_sign=="01110" || obstacle_sign=="11111" ||  obstacle_sign=="01010" || obstacle_sign=="00100") // turn around
      {
        if(serialDebug==1)
            { 
              Serial.println("Turning Back");
            }
        turnRight(90);
        pause(turnTime); 
      }   
    else // no obstacle, move forward
      {
        goForward();
        //Serial.println("Going Forward");
      }

    floorDistance=floorWatch();
    if(floorDistance>3) //drop detected, back up and turn around
    {
      stop();
      playErrorMelody();
      goBack();
      turnRight(90);
      return;
    }
  
  }

/*
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
*/