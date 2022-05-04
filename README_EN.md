# QuickCharge
Library for managing adapters supporting QuickCharge 2.0/3.0 technology
- QC 2.0: fixed voltage 5V, 9V, 12V (class A/B) and 20V (class B)
- QC 3.0: fixed (as in QC 2.0) + adjustable voltage in 200mV steps: 3.6-12V (class A/B) or 3.6-20V (class B)
- Determination of the QC version from the program
- Supports hot plugging
- MK supply voltage: 3.3-3.6V or 5V (see diagrams)
- Requires 4 digital pins to work

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Connection](#wiring)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **QuickCharge** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/QuickCharge/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include Library/Add .ZIP Library…* and specifydownloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="wiring"></a>
## Connection
- The first circuit - MK is powered by an external source 3.3V
- The second circuit - the MK is powered by an external 5V source (for example, from a nearby USB port)
- The third circuit - the MK is powered from the same QC port with high voltage through the stabilizer

![scheme](/doc/scheme.png)

<a id="init"></a>
## Initialization
```cpp
QuickCharge adapter(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L); // specify pins
QuickCharge adapter(uint8_t DP_H, uint8_t DP_L, uint8_t DM_H, uint8_t DM_L, bool CLASS); // + class
```

<a id="usage"></a>
## Usage
```cpp
uint8_tbegin(); // Negotiate with adapter, return adapter version:
  - QC_NA (0): does not support fast charging
  - QC_GEN1 (1): (QC 1.0) supports high current fast charging - 5V 2A
  - QC_GEN2 (2): (QC 2.0, QC 3.0) supports high voltage fast charging
  
void setClass(class); // Set adapter class (QC_CLASS_A by default). class:
  - QC_CLASS_A: max. voltage 12V
  - QC_CLASS_B: max. voltage 20V
  
void setMode(mode); // Set fixed voltage or custom voltage mode. mode:
  - QC_5V, QC_9V, QC_12V, QC_20V - fixed voltage (QC 2.0/3.0)
  - QC_VAR - adjustable voltage (QC 3.0)
  
void set(int); // Set output voltage in millivolts (requires QC_VAR and QC3.0 mode)
void inc(); // Increase output voltage by 200mV (requires QC_VAR and QC3.0 mode)
void dec(); // Decrease output voltage by 200mV (requires QC_VAR and QC3.0 mode)
intvoltage(); // Get the current voltage of the adapter
```

Additionally:
- You can exit the **QC_VAR** mode only by switching to the **QC_5V** mode
- Entering the modem **QC_VAR** retains the already set voltage (for example, when switching from QC_9V)
- It is not possible to determine the difference between QC 2.0 and QC 3.0 from the program, the adapter version is indicated on the case. Accordingly, the QC_VAR mode simply will not work on adapters with a QC version less than 3.0
- Not all adapters can deliver voltage below 5V, see the sticker on the case

<a id="example"></a>
## Example
```cpp
#define DP_H_PIN 2
#define DP_L_PIN 3
#define DM_H_PIN A4
#define DM_L_PIN A5

/* Connect the lib and create an object */
#include <QuickCharge.h>
QuickCharge QC(DP_H_PIN, DP_L_PIN, DM_H_PIN, DM_L_PIN, QC_CLASS_B);

void setup() {
  // QC.setClass(QC_CLASS_A); // If desired, the adapter class can be changed
  
  Serial.begin(9600); // Open Serial port
  Serial.print(F("Charger type: ")); // Display a message about the adapter type

  int type = QC.begin(); // Be sure to call begin, it will return the adapter type
  switch (type) { // Display the type of adapter found
    case QC_NA: Serial.println(F("QC is not available")); break;
    case QC_GEN1: Serial.println(F("QC1.0 - (5V 2A)")); break;
    case QC_GEN2: Serial.println(F("QC2.0 or QC3.0")); break;
  }
  
  // FIXED VOLTAGE
  QC.setMode(QC_9V); // Get 9V
  Serial.print("Voltage: "); // Output the current voltage
  Serial.println(QC.voltage());
  delay(3000); // We wait

  QC.setMode(QC_12V); // Get 12V
  Serial.print("Voltage: "); // Output the current voltage
  Serial.println(QC.voltage());
  delay(3000); // We wait
  
  QC.setMode(QC_VAR); // set custom voltage mode
}

void loop() {
  /* Control the output voltage with the Serial Monitor, sending the voltage in whole millivolts! */
  if (Serial.available() > 1) { // A message has arrived via Serial
    int mv = Serial.parseInt(); // Parse voltage in millivolts
    QC set(mv);// Set the desired voltage
    Serial.print(F("Set voltage to: ")); // Output the current voltage
    Serial.print(QC.voltage());
    Serial.println(F("mV"));
    /*
      Also, the voltage can be increased / decreased manually:
      QC.inc(); // Increase voltage by 200 mV
      QC.dec(); // Decrease voltage by 200 mV
    */
  }
}
```

<a id="versions"></a>
## Versions
- v1.0

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!