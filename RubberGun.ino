

#include <Servo.h>

Servo myservo1;
Servo myservo2;


int pos1min = 0;  //min10
int pos1max = 180; //max53

int pos2min = 60;   //min0
int pos2max = 100;  //max180


int k = 0;
int l = 1;
const int MotorPin = 2;
const int FireButtonPin = 12;

int MotorState = LOW;
unsigned long previousMillis = 0;
const long interval = 100;
int pos1 = 90;
int pos2 = 90;

int servopause= 10;  //pause ms

int VerticalPin = 0;  // analog pin used to connect the potentiometer
int RotatePin = 1;
int VerticalVal;    // variable to read the value from the analog pin
int RotateVal;



void setup()
{
   myservo1.attach(11);  // Vertical
   myservo2.attach(10);  //Rotate
   myservo2.write(pos1);
   delay(500);
   myservo1.write(pos2);
   delay(500);
   pinMode (MotorPin, OUTPUT);
   pinMode (FireButtonPin, INPUT);
   previousMillis = millis();
   MotorState = LOW;
   digitalWrite (MotorPin, MotorState);
}
void klm(){
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
    if (MotorState == LOW) {
      MotorState = HIGH;
      digitalWrite (MotorPin, MotorState);
    }   
}

void kln(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    if (MotorState == HIGH) {
      MotorState = LOW;
      digitalWrite (MotorPin, MotorState);
    }   
  }
}

void loop()
{
  if (digitalRead(FireButtonPin) == 1) klm();

   VerticalVal = analogRead(VerticalPin);                       // reads the value of the potenciometer (value between 0 and 1023);
   VerticalVal = map(VerticalVal, 0, 1023, pos1min, pos1max);   // scale it to use it with the servo (value between pos1min and pos1max)
   myservo1.write(VerticalVal);                                 // sets the servo position according to the scaled value
   delay(servopause);

   RotateVal = analogRead(RotatePin);                           // reads the value of the potentiometer (value between 0 and 1023)
   RotateVal = map(RotateVal, 0, 1023, pos2min, pos2max);       // scale it to use it with the servo (value between pos2min and pos2max)
   myservo2.write(RotateVal);                                   // sets the servo position according to the scaled value
   delay(servopause);

   kln();
}

