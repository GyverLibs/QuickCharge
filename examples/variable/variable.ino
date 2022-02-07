/*
   Пример использования библиотеки QuickCharge
   Получаем напряжение с шагом 200мВ в диапазоне 3.6-12В или 3.6-20В от совместимого с QC3.0 адаптера
   Для этого примера подойдет любой адаптер с поддержкой QC3.0
   Схему подключения см. в libraries/QuickCharge/doc/scheme.png
*/

/* Можно использовать любые цифровые пины*/
#define DP_H_PIN  2
#define DP_L_PIN  3
#define DM_H_PIN  A4
#define DM_L_PIN  A5

/* Подключаем либу и создаем обьект */
#include <QuickCharge.h>
QuickCharge QC (DP_H_PIN, DP_L_PIN, DM_H_PIN, DM_L_PIN, QC_CLASS_B);
/*
   QC_CLASS_A - Адаптер класса А, с выходным напряжением до 12В
   QC_CLASS_B - Адаптер класса B, с выходным напряжением до 20В
   Класс можно не объявлять, по умолчанию QC_CLASS_A
*/

void setup() {
  Serial.begin(9600);                 // Открываем Serial port
  Serial.setTimeout(100);             // Устанавливаем таймаут парсинга поменьше
  Serial.print(F("Charger type: "));  // Выводим сообщение о типе адаптера

  int type = QC.begin();              // Обязательно вызываем begin, метод вернет тип адаптера
  switch (type) {                     // Выводим тип обнаруженного адаптера
    case QC_NA:   Serial.println(F("QC is not available")); break;
    case QC_GEN1: Serial.println(F("QC1.0 - (5V 2A)")); break;
    case QC_GEN2: Serial.println(F("QC2.0 or QC3.0")); break;
  }

  QC.setMode(QC_VAR);                 // устанавливаем режим настраиваемого напряжения
  /* Выход из этого режима возможен ТОЛЬКО переключением в режим QC_5V, после чего можно перейти на нужный вам */

  // QC.setClass(QC_CLASS_A);         // При желании класс адаптера можно поменять
}

void loop() {
  /* Управляем выходным напряжением при помощи Serial монитора, отправляя напряжение в целых милливольтах! */
  if (Serial.available() > 1) {           // Пришло сообщение по Serial
    int mv = Serial.parseInt();           // Парсим напряжение в милливольтах
    QC.set(mv);                           // Устанавливаем нужное напряжение
    Serial.print(F("Set voltage to: "));  // Выводим текущее напряжение
    Serial.print(QC.voltage());
    Serial.println(F(" mV"));
    /*
      Также напряжение можно увеличивать / уменьшать вручную:
      QC.inc();  // Увеличить напряжение на 200 мВ
      QC.dec();  // Уменьшить напряжение на 200 мВ
    */
  }
}
