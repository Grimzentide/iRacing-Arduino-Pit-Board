### Software Install Procedure


### UPDATE IN PROGRESS

#### Hardware
1. Clip TFT screen into Arduino Uno
2. Insert USB cable into device and computer

#### Arduino Side
1. Install the [Arduino IDE](https://www.arduino.cc) software
2. Install [Adafruit GFX graphics core library](https://github.com/adafruit/Adafruit-GFX-Library) to \Arduino\Sketchbook\libraries
3. Install [Adafruit 2.8" TFT display library](https://github.com/adafruit/TFTLCD-Library) to \Arduino\Sketchbook\libraries
  * Not all TFT's are supported and it may take some work on your behalf to identify and get the above libraries working
4. Launch Arduino IDE program
5. Open the Adafruit TFT example sketch
  * File > Examples > TFTLCD > GraphicsTest
5. Ensure the COM port is correct for the Arduino
6. Upload Adafruit TFTLCD sketch to your Arduino
7. Open the Arduino serial monitor
  * Tools > Serial Monitor
8. Identify LCD Driver Chip
  * If the serial monitor says Unknown LCD driver chip: 0, you will manually need to test each driver chip identifier
9. Download the iRacing Arduino Pit Board TFT Front End.ino sketch to your computer and open it in the Arduino IDE
10. Ensure line 58 in the Arduino front end uses the correct identifier for your TFT Screen from step 8.  
  * eg: uint16_t identifier = 0x9325;
  * If Unknown lcd chip, test using each of the following until the sketch displays correctly
  * 0x9325, 0x9328, 0x7575, 0x9341 or 0x8357
11. Upload the iRacing Arduino Pit Board TFT sketch onto your Arduino Uno
12. If successful, you should see the basic layout of the device (lines, headings, etc) without any numbers

#### Backend (Based on Python)
1. Download the current executable file from the [releases section](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/releases)
2. Unzip the file to a location of your choice
3. In Windows devices and printers, confirm the COM port number for your Arduino
4. Open a command prompt and run the exe file with the com port argument
  * eg: arduinopitboard_v0.8.exe com7
  * eg: arduinopitboard_v0.8.exe com7 -gallons
5. Launch iRacing
