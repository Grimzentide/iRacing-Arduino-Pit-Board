## iRacing Arduino Pit Board

The iRacing Arduino Pit Board is an Arduino based device that can help with pit strategies by giving you live up to date information about your fuel requirements in any session online or offline, including those races that require one or more pit stops. An executable program (based on python) runs in the background and reads the live telemetry data coming out of any iRacing session and calculates when you should and/or need to stop and how much fuel you need to add to finish. The application then sends updates to the Arduino Uno via the USB serial port and displays the information on a small touch screen. It is designed to work on a 2.8 inch (320 x 240) TFT or LCD screen or shield that is supported by the Adafruit graphics library and works out to be around $12 to $13 in hardware costs.

Features:
* session laps / time
* laps completed
* laps / time remaining 
* fuel remaining
* laps until you run out of fuel
* how much fuel you will need to add at the next stop
* 5 lap average fuel burn
* race fuel burn
* which lap you should pit on
* pit windows to make it to the end with only one more stop
* weather and session information
* Information on when your last stop was

<img src="http://i.imgur.com/MtPBUsV.png">

If you have a feature request or an problem with the code, please [create an entry in the issue section](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/issues)

#### Demonstration of v0.9

<a href="http://www.youtube.com/watch?feature=player_embedded&v=jrubmpEYgn8
" target="_blank"><img src="http://img.youtube.com/vi/jrubmpEYgn8/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

Click the above image to watch the video on youtube

## Hardware
* [Arduino Uno or Uno Clone](https://www.arduino.cc/en/Main/arduinoBoardUno)
* [2.8" TFT Shield via eBay](http://www.ebay.com.au/itm/381238351575?_trksid=p2060353.m2749.l2648&ssPageName=STRK%3AMEBIDX%3AIT) <-- This is the actual TFT I purchased on eBay Australia.
  * [The underside of the TFT screen looks like this](http://i.imgur.com/zYKCSf8.jpg)
  * Screens smaller than 2.8 inches will not work correctly and will crop significant amounts of Pit Board information.

## Mounting Options
A number of people are coming up with creative ways to mount the device including using 3D printers. The [**Mounting Options Page**](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Mounting%20Options.md) lists out the options as well as a link to the 3D printer object files.

![](http://i1201.photobucket.com/albums/bb350/jdrobbie41/24081267-400a-43a6-a625-b680936a3782_zpsetkzyjey.jpg)
___

#### Changelog: ([Full Changelog](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Changelog.md))
#### Version 0.9 - 2015-09-24
##### Front End (Arduino)
###### Changed
- [x] 'Pit on Lap' changed to 'Pit Window' to show the earliest and latest you can now pit

###### Fixed
- [x] Graphical errors when the pit window data updates

##### Back End (Python)
###### Added
- [x] Pit window functionality showing the earliest and latest you can stop if you can make it to the end. 
  - [x] If multiple stops are requried to get to the end, only the latest you can stop is shown.

###### Fixed  
- [x] Pit window is cleared if you conserve enough fuel to remove the need to stop
- [x] Fuel requried is cleared if you conserve enough fuel to remove the need to stop
- [x] Removed a double up of delays when writing information messages to the device
