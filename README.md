## iRacing-Arduino-Pit-Board

Video of v0.3 in action. **Note:** Older version shown

<a href="http://www.youtube.com/watch?feature=player_embedded&v=dhHBOhDyfVM
" target="_blank"><img src="http://img.youtube.com/vi/dhHBOhDyfVM/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>




### Software Install Procedure

#### Python Side
1. Install [Python 3.4.x](https://www.python.org/)
2. Install [PyYaml 3.11](http://pyyaml.org/wiki/PyYAML)
3. add python directory to your PATH environment variable
4. Install [Python iRacing SDK](https://github.com/kutu/pyirsdk) by [Mihail Latyshov (aka Kutu)](https://github.com/kutu)
  * pip3 install pyirsk
5. Download the iRacing Arduino Pit Board Back End python file to your computer

#### Arduino Side
1. Install [Arduino IDE](https://www.arduino.cc)
2. Install [Adafruit GFX graphics core library](https://github.com/adafruit/Adafruit-GFX-Library)
3. Install [Adafruit 2.8" TFT display library](https://github.com/adafruit/TFTLCD-Library)
  * Not all TFT's are supported and it may take some work on your behalf to identify and get the above libraries working
4. Download the iRacing Arduino Pit Board TFT Front End.ino sketch to your computer and open it in the Arduino IDE
5. Ensure line 58 in the Arduino front end uses the correct identifier for your TFT Screen.  The test sketches in the Adafruit core library will assist with this identification.
  * eg: uint16_t identifier = 0x9325;
5. Upload the iRacing Arduino Pit Board TFT sketch onto your Arduino Uno

### How to use the iRacing Arduino Pit Board
1. Plug in your Arduino with TFT screen into an available USB port
2. Determine which COM port your Arduino Uno is connected to
3. Edit the first section of the iRacing Arduino Pit Board Back End python file to ensure the COM port and operating system is correct
4. Run the python file from a command prompt or set it to auto load from a launcher (eg [iAppStarter](http://www.fulhack.org/iappstarter/))
  1. python "iRacing Arduino Pit Board TFT Back End.py"
5. Load iRacing Simulator
