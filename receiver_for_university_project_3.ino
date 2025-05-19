#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4); // SDA - A4, SCL - A5, от 3.3V

RF24 radio(9, 10); // CE, CSN; MOSI - 11, MISO - 12, SCK - 13, от 3.3V

char s[] = "helo";

const byte address[6] = "00001";
int ticketCounter;
int warningCount = 2;
void setup() {
  //pinMode(3,OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true) {
      Serial.println("failure\n");
    }
  }
  drawString(s, 10, 0, 0, 2);
}


void loop() {
  if (radio.available()) {
    radio.read(&ticketCounter, sizeof(ticketCounter));
    Serial.println(ticketCounter);
    if (ticketCounter <= 0) {
      char s1[] = "No paper";
      drawString(s1, 100, 0, 0, 2);
    }
    else if (ticketCounter <= warningCount) {
      char s2[] = "Paper will end soon";
      drawString(s2, 100, 0, 0, 2);
    }
  }
}

void drawString(char s[], int _size, int x, int y, int __size) {
  display.clearDisplay();

  display.setTextSize(__size);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(x, y);
  /*
          x
     |------------->
     |
     |
    y|
     |
     |
     v
   */
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  for(int16_t i=0; i<_size; i++) {
    if(s[i] == '\0') {break;}
    if(s[i] == '\n') display.write(' ');
    else          display.write(s[i]);    // Перенос строки делается автоматически
  }

  display.display();
  //delay(2000);
}
