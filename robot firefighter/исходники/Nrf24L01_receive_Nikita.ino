
#include <SPI.h>  // Подключаем библиотеку для работы с SPI-интерфейсом
#include <nRF24L01.h> // Подключаем файл конфигурации из библиотеки RF24
#include <RF24.h> // Подключаем библиотеку для работа для работы с модулем NRF24L01

#define PIN_LED 3  // Номер пина Arduino, к которому подключен светодиод
#define PIN_CE  22  // Номер пина Arduino, к которому подключен вывод CE радиомодуля
#define PIN_CSN 53 // Номер пина Arduino, к которому подключен вывод CSN радиомодуля

RF24 radio(PIN_CE, PIN_CSN); // Создаём объект radio с указанием выводов CE и CSN

int potValue[1]; // Создаём массив для приёма значений потенциометра

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT); // Настраиваем на выход пин светодиода
  radio.begin();  // Инициализация модуля NRF24L01
  radio.setChannel(5); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.setDataRate (RF24_1MBPS); // Скорость обмена данными 1 Мбит/сек
  radio.setPALevel(RF24_PA_HIGH); // Выбираем высокую мощность передатчика (-6dBm)
  radio.openReadingPipe (1, 0x7878787878LL); // Открываем трубу ID передатчика
  radio.startListening(); // Начинаем прослушивать открываемую трубу
}

void loop() {
  if(radio.available())
  { // Если в буфер приёмника поступили данные
    radio.read(&potValue,sizeof(potValue)); // Читаем показания потенциометра
    if(potValue[0] == 1) digitalWrite(PIN_LED, HIGH);
    if(potValue[0] == 0) digitalWrite(PIN_LED, LOW);
    Serial.println(potValue[0]);
   // delay(100);
  }
}
