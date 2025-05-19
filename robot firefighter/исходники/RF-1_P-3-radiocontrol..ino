/*
 * 
 *  Соединение выводов
 *  CE   - 14
    MISO - 12
    MOSI - 11
    SCK  - 13
    CS   - 15
    POT  - A7
*/
int a;
int b;
int c;
int d;
int e;

#include <SPI.h> 
#include "nRF24L01.h"
#include "RF24.h"

RF24 myRadio (9, 10);

struct package
{
int a;
int b;
int c;
int d;
int e;
};
byte addresses[][6] = {"0"};
typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115);           // Диапазон 115 находится выше WIFI
  myRadio.setPALevel(RF24_PA_HIGH);   //  выходная мощность
  myRadio.setDataRate(RF24_250KBPS); // Минимальная скорость
 
 pinMode (2, INPUT);
pinMode (3, INPUT);
pinMode (4, INPUT);
pinMode (5, INPUT);
pinMode (6, INPUT);
}
void WriteData()
{
 
             
  myRadio.openWritingPipe(0xF0F0F0F066); // Отправить данные на этот 40-разрядный адрес
  myRadio.write(&data, sizeof(data)); 
  

}
void loop()
{
data.a = digitalRead(2);
data.b = digitalRead(3);
data.c = digitalRead(4);
data.d = digitalRead(5);
data.e = digitalRead(6);

Serial.print(data.a);
Serial.print(data.b);
Serial.print(data.c);
Serial.print(data.d);
Serial.println(data.e);
WriteData();
}
