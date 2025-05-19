bool a;
bool b;
bool c;
bool d;
  bool e;
#include <SPI.h> 
#include "nRF24L01.h"
#include "RF24.h" 

RF24 myRadio (14, 53); 

struct package
{
bool a;
bool b;
bool c;
bool d;
bool e;
};
typedef struct package Package;
Package data;

byte addresses[][6] = {"0"};

void setup() {
  // put your setup code here, to run once:
   Serial.begin (9600);
  myRadio.begin();
  myRadio.setChannel(115);               // диапазон 115 выше WiFi сигналов
  myRadio.setPALevel(RF24_PA_HIGH);       //  выходная мощность
  myRadio.setDataRate( RF24_250KBPS ) ;  // минимальная скорость


  Serial.print("Setup Initialized");

  digitalWrite (17, LOW);
pinMode (2, OUTPUT);
pinMode (3, OUTPUT);
pinMode (4, OUTPUT);
pinMode (5, OUTPUT);
pinMode (6, OUTPUT);
pinMode (7, OUTPUT);
pinMode (8, OUTPUT);
pinMode (9, OUTPUT);
pinMode (10, OUTPUT);
pinMode (11, OUTPUT);
pinMode (12, OUTPUT);
pinMode (13, OUTPUT);

pinMode (16, INPUT);
pinMode (17, INPUT);
pinMode (18, INPUT);
pinMode (19, OUTPUT);
pinMode (20, INPUT);
pinMode (21, OUTPUT);
pinMode (22, INPUT);
pinMode (23, OUTPUT);
pinMode (24, INPUT);
}
void W (){
  digitalWrite (2,LOW);
digitalWrite (3,HIGH);
analogWrite (4, 100);
digitalWrite (5,LOW);
digitalWrite (6,HIGH);
analogWrite (7, 100);
digitalWrite (8,HIGH);
digitalWrite (9,LOW);
analogWrite (10, 100);
digitalWrite (11,HIGH);
digitalWrite (12,LOW);
analogWrite (13, 100);

  }
  void D (){
  digitalWrite (2,HIGH);
digitalWrite (3,LOW);
analogWrite (4, 100);
digitalWrite (5,LOW);
digitalWrite (6,HIGH);
analogWrite (7, 100);
digitalWrite (8,LOW);
digitalWrite (9,HIGH);
analogWrite (10, 100);
digitalWrite (11,HIGH);
digitalWrite (12,LOW);
analogWrite (13, 100);

  }
  void A (){
  digitalWrite (2,LOW);
digitalWrite (3,HIGH);
analogWrite (4, 100);
digitalWrite (5,HIGH);
digitalWrite (6,LOW);
analogWrite (7, 100);
digitalWrite (8,HIGH);
digitalWrite (9,LOW);
analogWrite (10, 100);
digitalWrite (11,LOW);
digitalWrite (12,HIGH);
analogWrite (13, 100);

  }
  void S (){
  digitalWrite (2,HIGH);
digitalWrite (3,LOW);
analogWrite (4, 100);
digitalWrite (5,HIGH);
digitalWrite (6,LOW);
analogWrite (7, 100);
digitalWrite (8,LOW);
digitalWrite (9,HIGH);
analogWrite (10, 100);
digitalWrite (11,LOW);
digitalWrite (12,HIGH);
analogWrite (13, 100);
  }
void ReadData()
{ 
 
  myRadio.openReadingPipe(1, 0xF0F0F0F066); // Какой канал считывать, 40-разрядный адрес
         myRadio.startListening();          // Остановить передачу и начать прием
  if (myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
    }
 
  }
}
  void O (){

if (data.a>0){
  W();
  }
  else ;
  if (data.b>0){
  A();
  }
  else ;
  if (data.c>0){
  S();
  }
  else ;
  if (data.d>0){
  D();
  }
  else ;
  if (data.e>0){
  digitalWrite (19, HIGH);
  }
  else 
  digitalWrite (19, LOW);
  }
  
void loop() {
ReadData();

      O();

       Serial.print(data.a);
              Serial.print(data.b);
              Serial.print(data.c);
              Serial.print(data.d);
              Serial.println(data.e);
       
}
