#ifndef _QuickCharge_h
#define _QuickCharge_h

/*
    Библиотека для управления адаптерами с поддержкой технологии QuickCharge 2.0/3.0
    - QC 2.0: фиксированное напряжение 5V, 9V, 12V (класс A/B) и 20V (класс B)
    - QC 3.0: фиксированное (как в QC 2.0) + настраиваемое напряжение с шагом 200mV: 3.6-12V (класс A/B) или 3.6-20V (класс B)
    - Определение версии QC из программы
    - Поддерживает "горячее" подключение
    - Напряжение питания МК: 3.3-3.6В или 5V (см. схемы)
    - Для работы необходимо 4 цифровых пина

    Egor 'Nich1con' Zakharov & AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    v1.0
*/

// Режим выходного напряжения адаптера
#define QC_5V   0x00    // 5V - состояние по умолчанию, доступно любому адаптеру
#define QC_9V   0x01    // 9V - доступно для адаптеров QC2.0, QC3.0 класса A и B
#define QC_12V  0x02    // 12V - доступно для адаптеров QC2.0, QC3.0 класса A и B
#define QC_20V  0x03    // 20V - доступно для адаптеров QC2.0, QC3.0 класса B
#define QC_VAR  0x04    // напряжение настраивается - для адаптеров QC3.0, 3.6-12V (класса А), 3.6-20V (класса B)

// Класс выходного напряжения адаптера
#define QC_CLASS_A  0x00    // Class A: QC адаптеры с максимальным напряжением 12V
#define QC_CLASS_B  0x01    // Class B: QC адаптеры с максимальным напряжением 20V

// Тип адаптера, определенный библиотекой
#define QC_NA   0x00    // не поддерживает быструю зарядку
#define QC_GEN1 0x01    // (QC 1.0) поддерживает быструю зарядку большим током - 5V 2A
#define QC_GEN2 0x02    // (QC 2.0, QC 3.0) поддерживает быструю зарядку высоким напряжением

/* ---------------------------------- Private - определения ----------------------------------*/
#define GLITCH_CONT_CHANGE  3     // Задержка импульса инкремента / декремента напряжения (мс)
#define GLITCH_VOUT_CHANGE  80    // Задержка переключения выходного напряжения (мс)
#define GLITCH_BC_DONE      1500  // Задержка основного согласования устройств (мс)
#define GLITCH_BC_PLUS      5     // Задержка после согласования устройств (мс)
#define TEST_PULSE          10    // Задержка короткого тестового импульса (мкс)

#define QC_MV_MIN   3600		  // Минимальное напряжение для QC3.0	
#define QCA_MV_MAX  12000		  // Максимальное напряжение для QC3.0 адаптеров класса A	
#define QCB_MV_MAX  20000		  // Максимальное напряжение для QC3.0 адаптеров класса A	

#define SET_HIZ     0x00		  // Логические состояния линий D+ и D-	
#define SET_0MV     0x01
#define SET_600MV   0x02
#define SET_3300MV  0x03

class QuickCharge {
public:
    // Конструктор объекта без указания класса адаптера - по умолчанию класс A
    QuickCharge(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L) :
    _dp_h(DP_H), _dp_l(DP_L), _dm_h(DM_H), _dm_l(DM_L), _class(QC_CLASS_A) {}

    // Конструктор объекта с указанием класса адаптера
    QuickCharge(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L, bool CLASS) :
    _dp_h(DP_H), _dp_l(DP_L), _dm_h(DM_H), _dm_l(DM_L), _class(CLASS) {}

    // Ручная установка класса адаптера - QC_CLASS_A или QC_CLASS_B
    void setClass(bool type) {
        _class = type;
    }

    // Метод согласования с адаптером QC, возвращает тип адаптера
    uint8_t begin() {
        _set_dm(SET_HIZ);               // Страховка, (D-) переходит в Hi-Z
        _set_dp(SET_HIZ);               // Страховка, (D+) переходит в Hi-Z

        pinMode(_dp_h, OUTPUT);         // Верхнее плечо (D+) как выход
        pinMode(_dp_l, OUTPUT);         // Нижнее плечо (D+)как выход

        // Проверяем, соеденены ли (D+) и (D-) внутри адаптера, чтобы определить поддержку QC
        _set_dp(SET_3300MV);            // Кратковременно устанавливаем (D+) в 3.3В
        delayMicroseconds(TEST_PULSE);  // Задержка в несколько микросекунд
        if (!digitalRead(_dm_h)) {      // Проверяем (D-), если LOW - (D+) и (D-) внутри адаптера НЕ соеденены
            _set_dp(SET_HIZ);           // Отключаем (D+), он больше не понадобится
            _5vOnly = true;             // Поддерживается только 5 вольт
            return QC_NA;               // Возвращаем тип адаптера - QC не поддерживается
        }

        // Какой-то из QC явно поддерживается, пробуем договориться с адаптером
        _set_dp(SET_600MV);             // Устанавливаем 600 мВ на (D+)
        delay(GLITCH_BC_DONE);          // Ждем установленный стандартом промежуток времени

        // Проверяем, разъединил ли адаптер (D+) и (D-) внутри себя
        _set_dp(SET_3300MV);            // Кратковременно устанавливаем (D+) в 3.3В
        delayMicroseconds(TEST_PULSE);  // Задержка в несколько микросекунд
        if (!digitalRead(_dm_h)) {      // Проверяем (D-), если LOW - (D+) и (D-) внутри адаптера НЕ соеденены
            _set_dp(SET_600MV);         // Возвращаем предыдущий логический уровень на (D+)
            pinMode(_dm_h, OUTPUT);     // Подключаем верхнее плечо (D-) как выход
            pinMode(_dm_l, OUTPUT);     // Подключаем нижнее плечо (D-) как выход
            _5vOnly = false;            // Поддерживается не только 5 вольт	
            return QC_GEN2;             // Адаптер разъединил (D+) и (D-) после согласования, значит он поддерживает QC2.0 или QC3.0
        }

        // После попытки согласования (D+) и (D-) все еще замкнуты, это обычный 5В 2А адаптер
        _set_dp(SET_HIZ);               // Переводим D+ в Hi-Z, больше он не понадобится
        _set_dm(SET_HIZ);               // Переводим D- в Hi-Z, больше он не понадобится
        _5vOnly = true;                 // Поддерживается только 5 вольт
        return QC_GEN1;                 // Возвращаем тип адаптера - QC1.0
    }

    /* Метод установки режима выходного напряжения адаптера */
    void setMode(uint8_t mode) {
        /*
        | (D+) | (D-) |   Mode   |
        | 0.6V |  0V  |   5V     |
        | 3.3V | 0.6V |   9V     |
        | 0.6V | 0.6V |   12V    |
        | 3.3V | 3.3V |   20V    |
        | 0.6V | 3.3V | VARIABLE |
        */
        switch (mode) {               	// Выбираем нужный режим
        case QC_5V:               	    // 5V - по умолчанию
            _set_dp(SET_600MV);       	// (D+) 0.6V
            _set_dm(SET_0MV);         	// (D-) 0V
            _mv = 5000;           	    // Сохраняем текущее напряжение
            break;
        case QC_9V:               	    // 9V
            if (_5vOnly) break;			// Напряжение не поддерживается адаптером
            _set_dp(SET_3300MV);      	// (D+) 3.3V
            _set_dm(SET_600MV);       	// (D-) 0.6V
            _mv = 9000;           	    // Сохраняем текущее напряжение
            break;
        case QC_12V:              	    // 12V
            if (_5vOnly) break;			// Напряжение не поддерживается адаптером
            _set_dp(SET_600MV);       	// (D+) 0.6V
            _set_dm(SET_600MV);       	// (D-) 0.6V
            _mv = 12000;          	    // Сохраняем текущее напряжение
            break;
        case QC_20V:              	    // 20V
            if (_5vOnly || !_class) break;	// Напряжение не поддерживается адаптером
            _set_dp(SET_3300MV);      	// (D+) 3.3V
            _set_dm(SET_3300MV);      	// (D-) 3.3V
            _mv = 20000;          	    // Сохраняем текущее напряжение
            break;
        case QC_VAR:         	        // Настраиваемое напряжение
            if (_5vOnly) break;			// Напряжение не поддерживается адаптером
            _set_dp(SET_600MV);       	// (D+) 0.6V
            _set_dm(SET_3300MV);      	// (D-) 3.3V
            break;
        }
        delay(GLITCH_VOUT_CHANGE);  	// Ждем изменения
    }

    // установка выходного напряжения адаптера в режиме QC_VAR
    void set(int16_t mv) {
        if(_5vOnly) return;					            // Режим не поддерживается адаптером
        int16_t error = constrain(mv, QC_MV_MIN, _class ? QCB_MV_MAX : QCA_MV_MAX) - _mv; // Считаем, сколько накрутить
        bool dir = true;                                // Направление - больше / меньше
        if (error < 0) error = -error, dir = false;     // Если ошибка отрицательная, меняем направление, убираем минус
        for (uint8_t i = 0; i < error / 200; i++) {     // Крутим нужное количество раз
            if (dir) inc();                             // Надо увеличить - инкрементируем
            else dec();                                 // Надо уменьшить - декрементируем
        }
    }

    // инкремент выходного напряжения адаптера в режиме QC_VAR
    void inc() {
        if(_5vOnly) return;		    // Режим не поддерживается адаптером
        _set_dp(SET_3300MV);        // Подаем восходящий фронт на (D+)
        delay(GLITCH_CONT_CHANGE);  // Ждем
        _set_dp(SET_600MV);         // Возвращаем (D+) на прежний уровень
        delay(GLITCH_CONT_CHANGE);  // Ждем
        _mv = constrain(_mv + 200, QC_MV_MIN, _class ? QCB_MV_MAX : QCA_MV_MAX);
    }

    // декремент выходного напряжения адаптера в режиме QC_VAR
    void dec() {
        if(_5vOnly) return;		    // Режим не поддерживается адаптером
        _set_dm(SET_600MV);         // Подаем нисходящий фронт на (D-)
        delay(GLITCH_CONT_CHANGE);  // Ждем
        _set_dm(SET_3300MV);        // Возвращаем (D-) на прежний уровень
        delay(GLITCH_CONT_CHANGE);  // Ждем
        _mv = constrain(_mv - 200, QC_MV_MIN, _class ? QCB_MV_MAX : QCA_MV_MAX);
    }
    
    // Текущее напряжение (mV)
    int16_t voltage() {
        return _mv;
    }

private:
    // Служебные функции для установки логических уровней (0V, 0.6V, 3.3V) на D+ и D-
    void _set_dp(uint8_t state) {
        switch (state) {
        case SET_HIZ:    pinMode(_dp_h, INPUT), pinMode(_dp_l, INPUT);                // Hi-Z: INPUT + LOW
        case SET_0MV:    digitalWrite(_dp_h, LOW), digitalWrite(_dp_l, LOW);   break; // 0 mV: Оба плеча LOW
        case SET_600MV:  digitalWrite(_dp_h, HIGH), digitalWrite(_dp_l, LOW);  break; // 600 mV: Работает делитель
        case SET_3300MV: digitalWrite(_dp_h, HIGH), digitalWrite(_dp_l, HIGH); break; // 3300 mV: Оба плеча HIGH
        }
    }

    void _set_dm(uint8_t state) {
        switch (state) {
        case SET_HIZ:    pinMode(_dm_h, INPUT), pinMode(_dm_l, INPUT);                // Hi-Z: INPUT + LOW
        case SET_0MV:    digitalWrite(_dm_h, LOW), digitalWrite(_dm_l, LOW);   break; // 0 mV: Оба плеча LOW
        case SET_600MV:  digitalWrite(_dm_h, HIGH), digitalWrite(_dm_l, LOW);  break; // 600 mV: Работает делитель
        case SET_3300MV: digitalWrite(_dm_h, HIGH), digitalWrite(_dm_l, HIGH); break; // 3300 mV: Оба плеча HIGH
        }
    }
    
    int16_t _mv = 5000;
    
    // пины
    const uint8_t _dp_h = 0;
    const uint8_t _dp_l = 0;
    const uint8_t _dm_h = 0;
    const uint8_t _dm_l = 0;
    bool _5vOnly = true;
    bool _class = false;
};
#endif