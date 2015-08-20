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
3. Install [Adafruit 2.8" TFT display library](https://github.com/adafruit/TFTLCD-Library)\
4. Download the iRacing Arduino Pit Board TFT Front End.ino sketch to your computer
4. Upload the iRacing Arduino Pit Board TFT sketch onto your Arduino Uno

### How to use the iRacing Arduino Pit Board
1. Plug in your Arduino with TFT screen into an available USB port
2. Determine which COM port your Arduino Uno is connected to
3. Edit the first section of the iRacing Arduino Pit Board Back End python file to ensure the COM port and operating system is correct
4. Run the python file from a command prompt or set it to auto load from a launcher (eg [iAppStarter](http://www.fulhack.org/iappstarter/))
  1. python "iRacing Arduino Pit Board TFT Back End.py"
5. Load iRacing Simulator
