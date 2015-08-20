## iRacing-Arduino-Pit-Board

Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque sed vehicula urna. Fusce eu elit sed felis auctor malesuada nec et enim. Cras maximus risus ac vehicula ullamcorper. Cras arcu sapien, tristique pharetra convallis nec, congue sed turpis. Maecenas egestas, lorem et eleifend vestibulum, ante metus pretium nibh, quis blandit mi est at odio. Morbi bibendum mauris a pulvinar dignissim. Donec efficitur sit amet diam nec dictum. Nulla accumsan feugiat ipsum, nec rhoncus elit molestie non. Pellentesque et dui porttitor nisl sagittis suscipit ac eu odio. Nam maximus, purus at sagittis maximus, mi dui posuere dui, quis condimentum risus enim a neque. Praesent faucibus eros consectetur, ornare nibh nec, ornare tortor. Quisque efficitur consectetur felis nec vulputate. Vestibulum id tincidunt nunc, sit amet euismod ante.

### Software Install Procedure

#### Python Side
1. [Python 3.4.x](https://www.python.org/)
2. [PyYaml 3.11](http://pyyaml.org/wiki/PyYAML)
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
4. Run the python file from a command prompt
  1. python "iRacing Arduino Pit Board TFT Back End.py"
5. Load iRacing Simulator
