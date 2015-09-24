## iRacing Arduino Pit Board

The iRacing Arduino Pit Board is a small Arduino based device designed to give you live up to date information about your fuel requirements in any session online or offline including races that require one or more pit stops.  It works with both timed and lap based sessions up to 24 hours in length. It is designed to work on a 2.8 inch (320 x 240) TFT or LCD screen or shield that is supported by the Adafruit graphics library and works out to be around $12 to $13 in hardware costs.

<img src="http://i.imgur.com/MFo0aRz.png">

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
