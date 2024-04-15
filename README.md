[![latest](https://img.shields.io/github/v/release/GyverLibs/QuickCharge.svg?color=brightgreen)](https://github.com/GyverLibs/QuickCharge/releases/latest/download/QuickCharge.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/QuickCharge.svg)](https://registry.platformio.org/libraries/gyverlibs/QuickCharge)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/QuickCharge?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# QuickCharge
Библиотека для управления адаптерами с поддержкой технологии QuickCharge 2.0/3.0
- QC 2.0: фиксированное напряжение 5V, 9V, 12V (класс A/B) и 20V (класс B)
- QC 3.0: фиксированное (как в QC 2.0) + настраиваемое напряжение с шагом 200mV: 3.6-12V (класс A/B) или 3.6-20V (класс B)
- Определение версии QC из программы
- Поддерживает "горячее" подключение
- Напряжение питания МК: 3.3-3.6В или 5V (см. схемы)
- Для работы необходимо 4 цифровых пина

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Подключение](#wiring)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **QuickCharge** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/QuickCharge/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="wiring"></a>
## Подключение
- Первая схема - МК питается от внешнего источника 3.3V
- Вторая схема - МК питается от внешнего источника 5V (например от соседнего USB порта)
- Третья схема - МК питается от этого же порта QC с высоким напряжением через стабилизатор

![scheme](/doc/scheme.png)

<a id="init"></a>
## Инициализация
```cpp
QuickCharge adapter(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L);                // указание пинов
QuickCharge adapter(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L, bool CLASS);    // + класс
```

<a id="usage"></a>
## Использование
```cpp
uint8_t begin();        // Выполнить согласование с адаптером, вернет версию адаптера:
  - QC_NA (0): не поддерживает быструю зарядку
  - QC_GEN1 (1): (QC 1.0) поддерживает быструю зарядку большим током - 5V 2A
  - QC_GEN2 (2): (QC 2.0, QC 3.0) поддерживает быструю зарядку высоким напряжением
  
void setClass(class);   // Установить класс адаптера (по умолчению QC_CLASS_A). class:
  - QC_CLASS_A: макс. напряжение  12V
  - QC_CLASS_B: макс. напряжение  20V
  
void setMode(mode);     // Установить фиксированное напряжение или режим настраиваемого напряжения. mode:
  - QC_5V, QC_9V, QC_12V, QC_20V - фиксированное напряжение (QC 2.0/3.0)
  - QC_VAR - настраиваемое напряжение (QC 3.0)
  
void set(int);          // Установить выходное напряжение в милливольтах (необходим режим QC_VAR и QC3.0)
void inc();             // Увеличить выходное напряжение на 200мВ (необходим режим QC_VAR и QC3.0)
void dec();             // Уменьшить выходное напряжение на 200мВ (необходим режим QC_VAR и QC3.0)
int voltage();          // Получить текущее напряжение адаптера
```

Дополнительно:
- Выйти из режима **QC_VAR** можно только переключением в режим **QC_5V**
- Вход в режим **QC_VAR** сохраняет уже установленное напряжение (например, при переходе из QC_9V)
- Из программы невозможно определить различие между QC 2.0 и QC 3.0, версия адаптера указана на корпусе. Соответственно режим QC_VAR просто не будет работать на адаптерах с версией QC меньше 3.0
- Не все адаптеры могут выдать напряжение ниже 5V, см. наклейку на корпусе

<a id="example"></a>
## Пример
```cpp
#define DP_H_PIN  2
#define DP_L_PIN  3
#define DM_H_PIN  A4
#define DM_L_PIN  A5

/* Подключаем либу и создаем обьект */
#include <QuickCharge.h>
QuickCharge QC(DP_H_PIN, DP_L_PIN, DM_H_PIN, DM_L_PIN, QC_CLASS_B);

void setup() {
  // QC.setClass(QC_CLASS_A);         // При желании класс адаптера можно поменять
  
  Serial.begin(9600);                 // Открываем Serial port
  Serial.print(F("Charger type: "));  // Выводим сообщение о типе адаптера

  int type = QC.begin();              // Обязательно вызываем begin, вернет тип адаптера
  switch (type) {                     // Выводим тип обнаруженного адаптера
    case QC_NA:   Serial.println(F("QC is not available")); break;
    case QC_GEN1: Serial.println(F("QC1.0 - (5V 2A)")); break;
    case QC_GEN2: Serial.println(F("QC2.0 or QC3.0")); break;
  }
  
  // ФИКСИРОВАННОЕ НАПРЯЖЕНИЕ
  QC.setMode(QC_9V);              // Получаем 9В
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.println(QC.voltage());
  delay(3000);                    // Ждем

  QC.setMode(QC_12V);             // Получаем 12В
  Serial.print("Voltage: ");      // Выводим текущее напряжение
  Serial.println(QC.voltage());
  delay(3000);                    // Ждем
  
  QC.setMode(QC_VAR);             // устанавливаем режим настраиваемого напряжения
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
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!


При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
