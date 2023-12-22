This is an automatic translation, may be incorrect in some places. See sources and examples!

# QuickChaRge
Library for managing adapters with support for QUICCHARGE 2.0/3.0 technology
- QC 2.0: fixed voltage 5V, 9V, 12V (class A/B) and 20V (class B)
-QC 3.0: fixed (as in QC 2.0) + tuned voltage with a step 200MV: 3.6-12V (class A/B) or 3.6-20V (class B)
- Determination of the version of QC from the program
- supports "hot" connection
- supply voltage MK: 3.3-3.6V or 5V (see schemes)
- for work, you need 4 digital pins

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [connection] (#wiring)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** QuickChaRge ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/quickcharge/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="wiring"> </a>
## connection
- the first scheme - MK is powered by an external source 3.3V
- The second scheme - MK is powered by an external source 5V (for example from a neighboring USB port)
- the third scheme - MK is powered by the same QC port with high voltage through a stabilizer

! [Scheme] (/doc/scheme.png)

<a id="init"> </a>
## initialization
`` `CPP
Quickcharge adapter (uint8_t dp_h, uint8_t dp_l, uint8_t dm_h, uint8_t dm_l);// Indication of Pinov
Quickcharge adapter (uint8_t dp_h, uint8_t dp_l, uint8_t dm_h, uint8_t dm_l, bool class);// + class
`` `

<a id="usage"> </a>
## Usage
`` `CPP
uint8_t begin ();// Perform approval with the adapter, will return the version of the adapter:
  - qc_na (0): does not support fast charging
  - Qc_gen1 (1): (Qc 1.0) supports fast charging with large current - 5V 2A
  - Qc_gen2 (2): (Qc 2.0, Qc 3.0) supports fast charging with high voltage
  
Void setclass (class);// Install the adapter class (by default qc_class_a).Class:
  - Qc_class_a: Max.Voltage 12V
  - Qc_class_b: Max.voltage 20V
  
VOID setmode (mode);//set a fixed voltage or a adjustable voltage mode.Mode:
  - qc_5v, qc_9v, qc_12v, qc_20v - fixed voltage (QC 2.0/3.0)
  - qc_var - tuned voltage (QC 3.0)
  
VOID set (int);// Set the output voltage in millivals (you need the QC_Var and Qc3.0 mode)
VOID Inc ();// Increase the output voltage by 200 m (requires the QC_Var and Qc3.0 mode)
Void Dec ();// Reduce the output voltage by 200 m (requires the QC_Var and Qc3.0 mode)
int voltage ();// Get the current voltage of the adapter
`` `

Additionally:
- get out of the ** qc_var ** mode only by switching to the ** qc_5v ** mode
- Entrance to the ** qc_var ** mode retains the already set voltage (for example, when transition from Qc_9V)
- It is impossible to determine the difference between QC 2.0 and QC 3.0 from the program, the version of the adapter is indicated on the case.Accordingly, the QC_Var mode will simply not work on adapters with the QC version less than 3.0
- Not all adapters can issue a voltage below 5V, see the sticker on the case

<a id="EXAMPLE"> </a>
## Example
`` `CPP
#define dp_h_pin 2
#define dp_l_pin 3
#define dm_h_pin a4
#define dm_l_pin a5

/ * Connect the LIBA and create an object */
#include <QuickChaRge.h>
Quickcharge qc (dp_h_pin, dp_l_pin, dm_h_pin, dm_l_pin, qc_class_b);

VOID setup () {
  // Qc.setclass (qc_class_a);// If desired, the class class can be changed
  
  Serial.Begin (9600);// Open Serial Port
  Serial.print ("Charger Type:"));// display a message about the type of adapter

  int type = qc.begin ();// We must call Begin, return the type of adapter
  Switch (type) {// Display the type of adapter detected
    CASE QC_NA: serial.println (F ("Qc IS Not Available")));Break;
    CASE QC_gen1: serial.println (F ("QC1.0 - (5V 2A)"));Break;
    Case Qc_gen2: Serial.println (F ("Qc2.0 or Qc3.0"));Break;
  }
  
  // fixed voltage
  Qc.Setmode (QC_9V);// Get 9V
  Serial.print ("Voltage:");// We remove the current voltage
  Serial.println (Qc.voltage ());
  Delay (3000);// We wait

  Qc.Setmode (QC_12V);// We get 12V
  Serial.print ("Voltage:");// We remove the current voltage
  Serial.println (Qc.voltage ());
  Delay (3000);// We wait
  
  Qc.Setmode (Qc_var);// set the adjustable voltage mode
}

VOID loop () {
  /* We control the output voltage using a Serial monitor, sending voltage to as many as millivals!*/
  if (serial.available ()> 1) {// Came a message on Serial
    int mv = serial.parseint ();// Parsim tension in millivals
    Qc.Set (MV);// set the desired voltage
    Serial.print ("set voltage to:"));// We remove the current voltage
    Serial.print (Qc.voltage ());
    Serial.println (f ("mv"));
    /*
      Also, the voltage can be increased / reduced manually:
      QC.inc ();// increase voltage by 200 mV
      Qc.Dec ();// Reduce voltage by 200 mV
    */
  }
}
`` `

<a id="versions"> </a>
## versions
- V1.0

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code