/*
   Простой пример использования библиотеки QuickCharge
   Получаем 5,9,12 и 20В от совместимого с QC2.0, QC3.0 адаптера
   Для этого примера подойдет любой совместимый адаптер QuickCharge
   Схему подключения см. в libraries/QuickCharge/doc/scheme.png
*/

/* Можно использовать любые цифровые пины*/
#define DP_H_PIN  2
#define DP_L_PIN  3
#define DM_H_PIN  A4
#define DM_L_PIN  A5

/* Подключаем либу и создаем обьект */
#include <QuickCharge.h>
QuickCharge QC(DP_H_PIN, DP_L_PIN, DM_H_PIN, DM_L_PIN, QC_CLASS_B);
/*
   QC_CLASS_A - Адаптер класса А, с выходным напряжением до 12В
   QC_CLASS_B - Адаптер класса B, с выходным напряжением до 20В
   Класс можно не объявлять, по умолчанию QC_CLASS_A
*/

void setup() {
  Serial.begin(9600);                 // Открываем Serial port
  Serial.print(F("Charger type: "));  // Выводим сообщение о типе адаптера

  int type = QC.begin();              // Обязательно вызываем begin, метод вернет тип адаптера
  switch (type) {                     // Выводим тип обнаруженного адаптера
    case QC_NA:   Serial.println(F("QC is not available")); break;
    case QC_GEN1: Serial.println(F("QC1.0 - (5V 2A)")); break;
    case QC_GEN2: Serial.println(F("QC2.0 or QC3.0")); break;
  }

  // QC.setClass(QC_CLASS_A);         // При желании класс адаптера можно поменять
}

void loop() {
  QC.setMode(QC_5V);              // Получаем стандартные 5В
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.print(QC.voltage());
  Serial.println("mV");           // В милливольтах
  delay(3000);                    // Ждем

  QC.setMode(QC_9V);              // Получаем 9В
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.print(QC.voltage());
  Serial.println("mV");           // В милливольтах
  delay(3000);                    // Ждем

  QC.setMode(QC_12V);             // Получаем 12В
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.print(QC.voltage());
  Serial.println("mV");           // В милливольтах
  delay(3000);                    // Ждем

  QC.setMode(QC_20V);             // Получаем 20В (Доступно для адаптеров класса B)
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.print(QC.voltage());
  Serial.println("mV");           // В милливольтах
  delay(3000);                    // Ждем
}
