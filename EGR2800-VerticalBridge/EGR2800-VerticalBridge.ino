This code is made for the EGR2800 final project Group 3 (drawbridge)


#include<Servo.h>

Servo myservo;

int buzzer=12;    // the alarm that will sound when bridge is moving
int frontled=9;   // led that will flash on the 'front' side of bridge to alert boats of moving bridge
int backled=10;   // led that will flash on the 'back' side of bridge to alert boats of moving bridge
int frontpot=0;   // photocell at the 'front' side of the bridge that will detect if a boat is too high (connected to A0)
int backpot=1;    // photocell at the 'front' side of the bridge that will detect if a boat is too high (connected to A1)

int motor1pin1 = 2; // pin definitions of motor 1
int motor1pin2 = 3; // pin definitions of motor 1

int motor2pin1 = 4; // pin definitions of motor 2
int motor2pin2 = 5; // pin definitions of motor 2

int red = 6;        // red light of traffic light
int green = 3;      // green light of traffic light
int blue = 5;       // blue light of traffic light

int bottombutton = 2;     // this button controlls when the motor is lowering
int topbutton = 5;        // (CHECK PIN NUMBER) this button controlls when the motor is raising

bool fronttrip; // true/false variable to determine if front laser is tripped or not
bool backtrip;  // true/false variable to determine if back laser is tripped or not

void setup() {
  // put your setup code here, to run once:

    Serial.begin(9600); // starting serial monitor

  pinMode(buzzer,OUTPUT);    // setting buzzer output
  pinMode(frontled, OUTPUT); // setting front waterside led output
  pinMode(backled,OUTPUT);   // setting back waterside led output
  pinMode(red, OUTPUT);      // setting red pin of rgbled as out
  pinMode(green, OUTPUT);    // setting green pin of rgbled as out
  pinMode(blue, OUTPUT);     // setting blue pin of rgbled as out

  pinMode(motor1pin1, OUTPUT); // seting output for motor 1
  pinMode(motor1pin2, OUTPUT); // seting output for motor 1
  pinMode(motor2pin1, OUTPUT); // seting output for motor 2
  pinMode(motor2pin2, OUTPUT); // seting output for motor 2

  myservo.attach(11); // atach servo to pin 11

}

void loop() {
  // put your main code here, to run repeatedly:

  frontpot=analogRead(0); // read value of front photocell
  backpot=analogRead(1);  // read value of back photocell

  bool topbuttonstate = digitalRead_debounced(topbutton);        // true/false variable to determine if top button is //pressed or not
  bool bottombuttonstate = digitalRead_debounced(bottombutton);  // true/false variable to determine if bottom button //is pressed or not

   if (frontpot < 500) // checking if front tripwire is tripped
  {
    fronttrip = true; // if it is then variable is true
  }

   if ( backpot < 500) // checking if back tripwire is tripped
  {
    backtrip = true; // if its then variable is true
  }

  if (fronttrip == true) // if a boat that is too tall is coming from the 'front' of the brigde run this code to raise
  {
    
    while (backtrip != true) // run the code to raise the bridge while the back tripwire is not tripped which indicates that //the boat has safely passed through
    {
    
    // traffic light yellow -> red
     analogWrite(red,   255); //yellow
     analogWrite(green, 255);
     analogWrite(blue,  0);
     
       delay(1000);
     analogWrite(red,   255); //red
     analogWrite(green, 0);
     analogWrite(blue,  0);

    // drop servo gate
      myservo.write(180);       // horizontal -> the gate is closed
    
      while(topbuttonstate != HIGH)     // raise the bridge until top button is pressed
      {
        // sound buzzer
        tone(12,349);
        delay(500);
        tone(12,440);
        delay(500);
        
        digitalWrite(motor1pin1, HIGH); // motors will run to pull bridge up
        digitalWrite(motor1pin2, HIGH);

        digitalWrite(motor2pin1, HIGH);  // motors will run to pull bridge up
        digitalWrite(motor2pin2, HIGH);
        Serial.println("going up");      // will print 'going up' in serial monitor

       // flash red lights on both sides of bridge to alert the boat
      digitalWrite(frontled, HIGH); 
      digitalWrite(backled, HIGH); 
      delay(500);              
      digitalWrite(frontled, LOW); 
      digitalWrite(backled, LOW);   
      delay(500);
      }// end while button

    } // end while loop
  } // end if statement

  else if (backtrip == true) // if a boat that is too tall is coming from the 'back' of the brigde, run this code to raise bridge
  {

    while (fronttrip != true) // run the code to raise the bridge while the front tripwire is not tripped which indicates that //the boat has safely passed through
    {

    // traffic light yellow -> red
     analogWrite(red,   255); //yellow
     analogWrite(green, 255);
     analogWrite(blue,  0);
     
       delay(1000);
     analogWrite(red,   255); //red
     analogWrite(green, 0);
     analogWrite(blue,  0);

    // drop servo gate
      myservo.write(180); // horizontal -> the gate is closed
      
      while(topbuttonstate != HIGH) // raise the bridge until top button is pressed
      {
        // sound buzzer
        tone(12,349);
        delay(500);
        tone(12,440);
        delay(500);
        
        digitalWrite(motor1pin1, HIGH); // motors will run to pull bridge up
        digitalWrite(motor1pin2, HIGH);

        digitalWrite(motor2pin1, HIGH); // motors will run to pull bridge up
        digitalWrite(motor2pin2, HIGH);
        Serial.println("going up");     // will print 'going up' in serial monitor

       // flash red lights on both sides of bridge to alert the boat
      digitalWrite(frontled, HIGH); 
      digitalWrite(backled, HIGH); 
      delay(500);              
      digitalWrite(frontled, LOW); 
      digitalWrite(backled, LOW);   
      delay(500);
      } // end while button

    } //end while loop
  } // end if statement
  
  else // BASE CODE (NO BOAT)
  {

      while (bottombuttonstate != HIGH) // lower bridge until bottom botton is pressed
      {
        // sound buzzer
        tone(12,349);
        delay(500);
        tone(12,440);
        delay(500);
        
        digitalWrite(motor1pin1,LOW); //reverse the motors
        digitalWrite(motor1pin2,HIGH);
        
        digitalWrite(motor2pin1,LOW); //reverse the motors
        digitalWrite(motor2pin2,HIGH);

        Serial.println("going down"); // will print 'going down' in serial monitor
        
        fronttrip = false;  // set tripped value of front to false( reset it)
        backtrip = false;   // set tripped value of back to false (reset it)
      }// end while button

  // lift servo gate
  myservo.write(90);        // vertical -> the gate is open

  // turn green traffic led on
  analogWrite(red,   0); 
  analogWrite(green, 255);
  analogWrite(blue,  0);

  } // end else

} // end loop

// debouncing function
boolean digitalRead_debounced(int button_id) {
  boolean state[3]; // to hold three consecutive readings of input 
     
  do {
    state[0] = digitalRead(button_id);  // read the button and store
    delay(5);       // wait a little
    state[1] = digitalRead(button_id);  // read the button and store
    delay(5);       // wait a little
    state[2] = digitalRead(button_id);  // read the button and store
    
  } while (state[0] != state[1]  || state[1] != state[2]);  // do again if not stable
  
  return state[0];      // return any one of them, they are all the same
}
