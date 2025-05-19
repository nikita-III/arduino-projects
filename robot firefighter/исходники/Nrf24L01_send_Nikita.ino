#include <SPI.h>  // Подключаем библиотеку для работы с SPI-интерфейсом
#include <nRF24L01.h> // Подключаем файл конфигурации из библиотеки RF24
#include <RF24.h> // Подключаем библиотеку для работа для работы с модулем NRF24L01
#define PIN_POT 3  // Номер пина Arduino, к которому подключена кнопка
#define PIN_CE  14  // Номер пина Arduino, к которому подключен вывод CE радиомодуля
#define PIN_CSN 15 // Номер пина Arduino, к которому подключен вывод CSN радиомодуля

RF24 radio(PIN_CE, PIN_CSN); // Создаём объект radio с указанием выводов CE и CSN

int potValue[1]; // Создаём массив для передачи значений потенциометра

void setup() {
  Serial.begin(115200);
  radio.begin();  // Инициализация модуля NRF24L01
  radio.setChannel(5); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.setDataRate (RF24_1MBPS); // Скорость обмена данными 1 Мбит/сек
  radio.setPALevel(RF24_PA_HIGH); // Выбираем высокую мощность передатчика (-6dBm)
  radio.openWritingPipe(0x7878787878LL); // Открываем трубу с уникальным ID
}

void loop() {
  potValue[0] = digitalRead(PIN_POT); // Считываем показания
  radio.write(potValue, sizeof(potValue)); // Отправляем считанные показания по радиоканалу
  Serial.println(potValue[0]);
//  delay(500);
}
