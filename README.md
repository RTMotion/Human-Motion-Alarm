# Human-Motion-Alarm

The purpose of this project is to make a motion sensor alarm that goes off when it detects another person trying to move the object that the sensor is attached to. Heres the twist the only way to disarm the alarm is a RFID key tag that only you have. You are the key.

### Basic Idea 
 - The alarm will go off when the sensor detects motion.
 - When you present your key it will disarm the alarm. and will re-arm after 5 seconds has passed.
 - If another key is presented that it does not know it will also sound off the alarm.

Credit goes to deloarts for her code on the RFID-RC522 is the basis of my project.

### Components 
- 12VDC 76dB Peizo Buzzer
- Ardunino UNO
- Blue LED
- Red LED
- Tilt Ball Sensor 
- RFID RC 522 Scanner
- RFID key tag
- 2 220 ohm Resistors
- 10 Kohm Resistor

### Connection For RC522

Arduino Pin |	RC522 Pin
------------|----------
9	|RST
10|	NSS
11|	MOSI
12|	MISO
13|	SCK
3V3|	3V3
GND|	GND

### Full integration Schematic
For a visual aid visit my wordpress for the schematic and what it actually looks like wired up.
https://rtmotionsecurityalarm.wordpress.com/2017/02/21/putting-it-all-together/
