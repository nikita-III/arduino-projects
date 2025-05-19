#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); // CLK, DIN, DC, CE, RST, от 3.3V

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN; SCK - 52, MISO - 50, MOSI - 51, от 3.3V

#define button_pin 8

const int fioSize = 20;
const int timeeSize = 9;

// current data, updated in loop()
int id = 0;
char fio[fioSize] = "sample\0";
int price = 0;
char timee[timeeSize] = "00:00:00";
int left = 5;
// and the data buffer
// size: 4+20+4+9+4
const int databufSize = 41;
char databuf[databufSize] = "";

const byte address[6] = "00001";
bool buttonState = 0;
bool firstWarningSent = false;
bool secondWarningSent = false;
int ticketCounter = 5;
int warningCount = 2;

//uint8_t bitmap[1024] = {};

void setup() {
  pinMode(button_pin, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  display.begin();
    display.setContrast(50); // установка контраста
    display.setTextSize(1); // размер шрифта 1
    display.setTextColor(BLACK); // цвет текста чёрный
    display.clearDisplay();
}

void writeMessageToSerial() {
  // clearing the data buffer
  for (int i1 = 0; i1 < databufSize; i1++) {
    databuf[i1] = '\0';
  }
  
  // writing id
  int i = 0;
  int j = 0;
  char idToSend[10] = "ddddddddd";
  sprintf(idToSend, "%d", id);
  while (i < databufSize && idToSend[j] != 'd') {
    databuf[i] = idToSend[j];
    i++;
    j++; 
  }
  i--;
  databuf[i] = ';';
  i++;

  // writing fio
  j = 0;
  while (i < databufSize && j < fioSize && fio[j] != '\0') {
    databuf[i] = fio[j];
    j++;
    i++;
  }
  databuf[i] = ';';
  i++;

  // writing price
  j = 0;
  char priceToSend[10] = "ddddddddd";
  sprintf(priceToSend, "%d", price);
  while (i < databufSize && priceToSend[j] != 'd') {
    databuf[i] = priceToSend[j];
    i++;
    j++; 
  }
  i--;
  databuf[i] = ';';
  i++;

  // writing time
  j = 0;
  while (i < databufSize && j < timeeSize && timee[j] != '\0') {
    databuf[i] = timee[j];
    j++;
    i++;
  }
  databuf[i] = ';';
  i++;

  // writing how much paper is left
  j = 0;
  char leftToSend[10] = "ddddddddd";
  sprintf(leftToSend, "%d", left);
  while (i < databufSize && leftToSend[j] != 'd') {
    databuf[i] = leftToSend[j];
    i++;
    j++; 
  }
//  databuf[i] = ';';
//  i++;

  // go!
  Serial.println(databuf);
}

// just a placeholder function, meh
void setCurrentTime() {
  sprintf(timee, "11:11:11");
}

void loop() {
  display.clearDisplay(); // очищаем дисплей
  buttonState = digitalRead(button_pin);
  //Serial.print(buttonState);
  //Serial.print("   ");
  //Serial.println(ticketCounter);
  if (buttonState == HIGH) {
    id++;
    sprintf(fio, "sample");
    price = rand()%200;
    setCurrentTime();
    left = ticketCounter;
    if (left > 0)
      writeMessageToSerial();
    if (ticketCounter <= 0) {
      display.println("No paper for ticket"); // выводим текст
      display.display();
      delay(1000);
      display.clearDisplay();
      
    }
    else {
      ticketCounter--;
      display.println("Printing ticket..."); // выводим текст
      display.display();
      delay(700);
      display.clearDisplay();
      display.println("Take your ticket"); // выводим текст
      display.display();
      delay(700);
      display.clearDisplay();
      if (firstWarningSent == false && ticketCounter <= warningCount) {
        radio.write(&ticketCounter, sizeof(ticketCounter));
        //Serial.println("warning 1");
        firstWarningSent = true;
      }
      if (secondWarningSent == false && ticketCounter <= 0) {
        radio.write(&ticketCounter, sizeof(ticketCounter));
        //Serial.println("warning 2");
        secondWarningSent = true;
      }
    }
  }
  delay(200);
}
