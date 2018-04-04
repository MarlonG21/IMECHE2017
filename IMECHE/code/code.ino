#define encoderA 2
#define encoderB 16
//lcd screen
#include <LiquidCrystal.h> //LCD display library
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//initializing pins for display
int x;  //variable used to store the value for the voltage read
float SetDistance=0;
int Time;
int currentTime;
float disteveryThick= 0.00041; //costant of ditance covered for each encoder count in meters(try also with 0.00043)
//standby
int STBY = 13; 
int stopCount =0;
//Motor 
int PWMA = 3; //Speed control 
int AIN1 = 12; //Direction
int AIN2 = 11; //Direction
int sp=150;
int dir;
//microswitch
int button =17;
int buttonState;
int preButtonState=0;
int preButtonState1=0;
int i=0;
//Encoder
//int encoderA=2;
//int encoderB=16;
int count=0;
void setup(){
  //Standby pin
  pinMode(STBY, OUTPUT);
  //Setting pins for motor
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  //Setting button and encoders pins
  pinMode(button,INPUT);
  digitalWrite(15, HIGH);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(0,counting,CHANGE);
  
  //setting LCD's number of colums and rows
  lcd.begin(16, 2);           
  lcd.print("2nd Distance:"); //Printing message
  lcd.setCursor(0,1);
  lcd.print("C-DISTANCE:");
  
  
  Serial.begin(9600);  
}
void loop(){
  //setting keypad button, to be able to input and adjust the value of the second target
  x= analogRead(0);
  delay(50);
  lcd.setCursor(13,0);
    if(x<200)//corrisponding value of the up button when pressed
    {
      SetDistance=SetDistance+0.1;
      lcd.print(SetDistance);
      Serial.print(SetDistance);
    }
    else if (x<400)//corrisponding value of the down button when pressed
    {
      SetDistance=SetDistance-0.1;
      lcd.print(SetDistance);
      Serial.print(SetDistance);
    }
  
  float distance;
  distance= disteveryThick*count;
 
  //lcd.print(distance);
  lcd.setCursor(12,1);
  buttonState=digitalRead(button);
  if(buttonState != preButtonState)  //memorizing button's change state with counter
  {
    if( buttonState == LOW)
    {
      i++;
 //   Serial.print(i);
    }else {
     //Serial.print("off");
    }
    delay(5);
  }
  preButtonState = buttonState;   //using counter to set the conditions.
  if(i == 1)                      //When the button is pressed the first time the counter "i" is equal to 1 so the motor rotates CC.
  {                               // When the button is pressed a second time, we assume the robot encountered an obstacle, 
    delay(1000);                  //so we set the motor to rotate CW until the distance travelled is equal to 0 and make sure the current condition is false by assigning the value of i to 3.
    forward();                    //when it stops move until we reach a predefined distance.
    lcd.print(distance);
   
  }
  if(i==2)
  {
    stop1();
    backward(0);
    lcd.print(distance);
    if(distance<0.5)
    {
      move(1,90,0);
    }
      if(distance<0.3)
    {
      move(1,70,0);
    }
    
    if(distance<0)
    {

     digitalWrite(AIN1, HIGH);
     digitalWrite(AIN2, HIGH);
     delay(2000);
     stop1();
     delay(1500);
      i=3;
    }
  }
  Serial.print(i);
  if(i==3)
  { 
    lcd.print(distance);
    forward();
      if(distance<SetDistance/2)
    {
      move(1,70,1);
    }
    if(distance>SetDistance)
    {
    
     digitalWrite(AIN1, HIGH);
     digitalWrite(AIN2, HIGH);
     delay(2000);
     stop1();
      
    }

  }

} 
void move(int motor, int speed1, int direction1){
//Move specific motor at speed and direction
//motor: 1 for A(more motors can be added)
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction1 == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed1);
  }
}
void stop1(){
//enable standby 
  stopCount++;
  digitalWrite(STBY, LOW); 
}
void counting()                                      //function for reading the encoder counts
{
  if(digitalRead(encoderA)== digitalRead(encoderB))
  {
    count--;
  }else{count++;}

}
void forward(){            //forward function
  move(1,130,1);
}
void backward(int dir2)   //backward function
{
  dir=dir2;
  move(1,130,dir);
}




