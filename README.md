## iRacing Arduino Pit Board

The iRacing Arduino Pit Board is an Arduino based device that can help with pit strategies by giving you live up to date information about your fuel requirements in any session online or offline, including those races that require one or more pit stops. An executable program (based on python) runs in the background and reads the live telemetry data coming out of any iRacing session and calculates when you should and/or need to stop and how much fuel you need to add to finish. The application then sends updates to the Arduino Uno via the USB serial port and displays the information on a small touch screen. It is designed to work on a 2.8 inch (320 x 240) TFT or LCD screen or shield that is supported by the Adafruit graphics library and works out to be around $12 to $13 in hardware costs.

#### Features:
* session laps / time
* laps completed
* laps / time remaining 
* fuel remaining
* laps until you run out of fuel
* how much fuel you will need to add at the next stop
* imperial and metric fuel calculations
* 5 lap average fuel burn
* race fuel burn
* which lap you should pit on
* pit windows to make it to the end with only one more stop
* weather and session information
* information on when your last stop was
* shows the amount that should be in your tank when you leave the pits
* target burn rate if you need a splash and dash for less than 1 lap worth of fuel
* Warning flashes to help inform the driver that something critical has happened

**Note:** The calculations for required fuel automatically includes an extra half of a lap to ensure you can cross the finish line. The half lap fuel addition is calculated using half of the current 5 lap average fuel burn rate and is automatically included in the required fuel figure.

#### Concept: 
<img src="http://i.imgur.com/MtPBUsV.png">

If you have a feature request or an problem with the code, please [create an entry in the issue section](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/issues)

#### Demonstration of v0.9

<a href="http://www.youtube.com/watch?feature=player_embedded&v=jrubmpEYgn8
" target="_blank"><img src="http://img.youtube.com/vi/jrubmpEYgn8/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

Click the above image to watch the video on youtube

#### Hardware
* [Arduino Uno or Uno Clone](https://www.arduino.cc/en/Main/arduinoBoardUno)
* [2.8" TFT Shield via eBay](http://www.ebay.com.au/itm/381238351575?_trksid=p2060353.m2749.l2648&ssPageName=STRK%3AMEBIDX%3AIT) <-- This is the actual TFT I purchased on eBay Australia.
  * [The underside of the TFT screen looks like this](http://i.imgur.com/zYKCSf8.jpg)
  * Screens smaller than 2.8 inches will not work correctly and will crop significant amounts of Pit Board information.

#### Mounting Options
A number of people are coming up with creative ways to mount the device including using 3D printers. The [**Mounting Options Page**](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Mounting%20Options.md) lists out the options as well as a link to the 3D printer object files.

![](http://i1201.photobucket.com/albums/bb350/jdrobbie41/24081267-400a-43a6-a625-b680936a3782_zpsetkzyjey.jpg)![](http://i.imgur.com/HT7e6C7m.jpg)
___

### Changelog: ([Full Changelog](https://github.com/Grimzentide/iRacing-Arduino-Pit-Board/blob/master/Changelog.md))

#### Front End (Arduino)
##### Version 1.0 - 2015-11-15

**WARNING: The latest front end update is not optional for this release.**  *If you use v1.4 of the backend without updating the Arduino front end to v1.0, it will most likely crash when green information messages are received.*

###### Added
- [x] Green text information messages can now be used
- [x] Issue 19: Make the screen flash to grab your attention when the BOX BOX BOX message comes up
  - [x] Screen lines now flash red and orange to indicate a critical (aka red) information message
  - [x] Works for all critical (aka red) messages

###### Changed
- [x] 'FUEL REQUIRED' cell title has now been changed to 'FUEL TO ADD'


#### Back End
##### Version 1.4 - 2015-11-15
###### Fixed 
- [x] Fixed an issue where a log file would not be created when going from qualify to race
- [x] Removed a duplicate "On Pit Road" information message
- [x] Issue 23: Fuel required in a timed race was incorrect
  - [x] It was possible for a negative burn rate to be used causing all average calculations to be incorrect
- [x] Issue 14: Earliest stop number in pit window often in the past when first calculated 
  - [x] The earliest lap to stop increments each lap if you do not stop for fuel

###### Added
- [x] Issue 20 - Show the amount that should be in your tank when you leave the pits (added as information message)
  - [x] An information message will be displayed once you enter pit lane.
- [x] Target burn rate added if you require a stop to add less than 1 lap worth of fuel
  - [x] Simple implementation which checks if the required fuel is less than the 5 Lap AVG burn rate
  - [x] Information message will show your target burn rate when you go over the start/finish line
  - [x] It will stop updating if you have conserved enough or you pit for fuel
  - [x] Information message shows your last lap burn rate and the green text indicates you are under your target burn
  - [x] Information message shows your last lap burn rate and the yellow text indicates you are over your target burn
 - [x] Issue 24 created on the git hub site for the following request mentioned on the iRacing forums
   - [x] Make it optional to show the amount that should be in your tank when you leave the pits

###### Changed
- [x] Enhanced logging to help with analysis

___
