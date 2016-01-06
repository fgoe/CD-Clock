# CD-Clock
Arduino clock using old CDROM Drives for display


Code:
---------------
Provided is the .ino File for the Arduino IDE.
Additionally needed are the following arduino libraries:
- DCF77 Library
- Time Library


Summary:
---------------
The CDROM clock is a huge clock which uses 28 old CDROM drives to display the time.
Those 28 drives emulate four 7-segment displays.
The time is adjusted by using a DCF77 receiver.


Hardware needed:
------------------
- Arduino. I used an Arduino Mega.
- 28 old CDROM drives
- Transistor Arrays like ULN2004 for switching purposes.
- DCF Receiver. I used the one from Pollin -> http://www.pollin.de/shop/dt/NTQ5OTgxOTk-/Bausaetze_Module/Module/DCF_Empfangsmodul_DCF1.html
- A lot of plywood for building a case.
- A least one old PC power supply for powering the drives and arduino.
