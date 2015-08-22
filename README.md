## iRacing Arduino Pit Board

The iRacing Arduino Pit Board is a small Arduino based device designed to give you live up to date information about your fuel requirements in any session online or offline including races that requires one or more pit stops.  It works with both timed and lap based sessions up to 24 hours in length. It is designed to work on a 2.8 inch (320 x 240) TFT or LCD screen or shield that is supported by the Adafruit graphics library and works out to be around $12 to $13 in hardware costs.


#### Preview of v0.5 (UNRELEASED)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=FFvC9X74Tfs
" target="_blank"><img src="http://img.youtube.com/vi/FFvC9X74Tfs/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

## Hardware
* [Arduino Uno or Uno Clone](https://www.arduino.cc/en/Main/arduinoBoardUno)
* [2.8" TFT Shield via eBay](http://www.ebay.com.au/itm/381238351575?_trksid=p2060353.m2749.l2648&ssPageName=STRK%3AMEBIDX%3AIT) <-- This is the actual TFT I purchased on eBay Australia.
  * [The underside of the TFT screen looks like this](http://i.imgur.com/zYKCSf8.jpg)
  * Screens smaller than 2.8 inches will not work correctly and will crop significant amounts of Pit Board information.

___

#### Changelog: ([Full Changelog](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Changelog.md))
##### Version 0.4 - 2015-08-22

Version 0.4 in action.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=LYWg47O0CII
" target="_blank"><img src="http://img.youtube.com/vi/LYWg47O0CII/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

##### Front End (Arduino)
###### Added
- [x] Session Laps title changes to Session Time when in a timed session
- [x] Session Time displayed for timed sessions (previously blank)
- [x] Remaining Time displayed in timed sessions (previously blank)
- [x] "Unlimited" shown as session time in offline testing mode

###### Fixed
- [x] Various bug fixes

###### Changed
- [x] GUI alignment

##### Back End (Python)
###### Added

###### Fixed
- [x] Various bug fixes

###### Changed
- [x] Fuel required now shows the maximum tank size when the required fuel exceeds amount you can fit in

