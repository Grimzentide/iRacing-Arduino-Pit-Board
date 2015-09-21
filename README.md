## iRacing Arduino Pit Board

The iRacing Arduino Pit Board is a small Arduino based device designed to give you live up to date information about your fuel requirements in any session online or offline including races that require one or more pit stops.  It works with both timed and lap based sessions up to 24 hours in length. It is designed to work on a 2.8 inch (320 x 240) TFT or LCD screen or shield that is supported by the Adafruit graphics library and works out to be around $12 to $13 in hardware costs.

<img src="http://i.imgur.com/UEEd8sn.png">

If you have a feature request or an problem with the code, please [create an entry in the issue section](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/issues)

#### Demonstration of v0.5

<a href="http://www.youtube.com/watch?feature=player_embedded&v=FFvC9X74Tfs
" target="_blank"><img src="http://img.youtube.com/vi/FFvC9X74Tfs/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

Click the above image to watch the video on youtube

## Hardware
* [Arduino Uno or Uno Clone](https://www.arduino.cc/en/Main/arduinoBoardUno)
* [2.8" TFT Shield via eBay](http://www.ebay.com.au/itm/381238351575?_trksid=p2060353.m2749.l2648&ssPageName=STRK%3AMEBIDX%3AIT) <-- This is the actual TFT I purchased on eBay Australia.
  * [The underside of the TFT screen looks like this](http://i.imgur.com/zYKCSf8.jpg)
  * Screens smaller than 2.8 inches will not work correctly and will crop significant amounts of Pit Board information.

___

#### Changelog: ([Full Changelog](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Changelog.md))
#### Version 0.8 - 2015-09-21
##### Front End (Arduino)
###### Added
- [x] Auto centering text for all fields

###### Fixed
- [x] Issue #2 - Fuel Required sometimes has a 3rd decimal point
- [x] Issue #6 - Character display issues when going from a double digit + decimal to a single digit + decimal
 
##### Back End (Python)
###### Fixed
- [x] Issue #8 - Telemetry property 'CarClassMaxFuel' is renamed to 'CarClassMaxFuelPct'.
- [x] Issue #9 - You must be in the car or the app crashes (Fuel required error)

###### Added
- [x] .exe file now used to run the program removing the need to install python and the requried libraries

##### Other
###### Updated
- [x] Installation Guide
