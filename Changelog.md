### Changelog:
#### Version 1.2 - UNRELEASED
##### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

##### Back End (Python)
###### Fixed 
- [x] The reset car button in iRacing now resets the current fuel stats.
- [x] Information messages when running the backend without arguments now reflects .exe release
- [x] Delay after writing to screen has been reduced from .3 to .25
- [x] Backend will automatically retry re-connection when iRacing resets to a new session
- [x] Logging of all statistics to a unique local file fo each session is now enabled by default

___

#### Version 1.1 - 2015-10-03
##### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

##### Back End (Python)
###### Fixed 
- [x] Fields correctly reset when going from one session to another eg. Qualify to Race
- [x] Minor bug fixes

___

#### Version 1.0 - 2015-10-01
##### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

##### Back End (Python)
###### Fixed 
- [x] Issue #13 - "Unlimited" changed to "Infinite" to remove overlapping into other cells
- [x] Issue #11 - Track temperature not shown in Fahrenheit
  - Use -gallons argument to enable gallons and fahrenheit temperatures
- [x] Issue #7 - Not all figures are reset when the session changes type
- [x] Lap percentage not resetting after tow causing fuel burn averages to be incorrect
- [x] 5 Lap AVG now correctly reset after a tow
- [x] Race AVG now correctly reset after a tow
- [x] Time remaining now shows as "Infinite" instead of 168:00 if in an offline test session 
- [x] Remaining laps in offline testing mode now correctly calculated given the unlimited/infinite time for the session
- [x] Fixed a call that continually refreshed the screen when in the garage

___

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

___

#### Version 0.8 - 2015-09-21
##### Front End (Arduino)
###### Fixed
- [x] Issue #2 - Fuel Required sometimes has a 3rd decimal point
- [x] Issue #6 - Character display issues when going from a double digit + decimal to a single digit + decimal
 
###### Added
- [x] Auto centering text for all fields
- [x] Issue #10 - Auto centering text for headers

##### Back End (Python)
###### Fixed
- [x] Issue #8 - Telemetry property 'CarClassMaxFuel' is renamed to 'CarClassMaxFuelPct'.
- [x] Issue #9 - You must be in the car or the app crashes (Fuel required error)

###### Added
- [x] .exe file now used to run the program removing the need to install python and the requried libraries

##### Other
###### Updated
- [x] Installation Guide

___

#### Version 0.7 - 2015-09-14
##### Front End (Arduino)
- [ ] Nil

##### Back End (Python)
###### Added
- [x] .exe file now used to run the program removing the need to install python and the requried libraries

###### Fixed
- [x] Issue #8 - Telemetry property 'CarClassMaxFuel' is renamed to 'CarClassMaxFuelPct'.

___

#### Version 0.6 - 2015-08-29
##### Front End (Arduino)
- [ ] Nil

##### Back End (Python)
###### Added
- [x] Fuel Required now uses tank capacity and the maximum fuel percentage for the vehicle class
- [x] Version Number is now displayed on the screen during load

___

#### Version 0.5 - 2015-08-23
##### Front End (Arduino)
- [ ] Nil

##### Back End (Python)
###### Added
- [x] Command line argument now used to define Arduino COM port to allow the creation of an .exe in a future version
- [x] Optional command line argument now used to define the switch to gallons (default\no argument is litres)
  - [x] ```Python backend.py COM7 -gallons```
- [x] Fuel required in a timed session now based on the drivers expected lap time and the time left in the session

###### Deprecated
- [x] Linux support (all this basically means is that the console screen will not clear)

___

#### Version 0.4 - 2015-08-22
##### Front End (Arduino)
###### Added
- [x] Session Laps title changes to Session Time when in a timed session
- [x] Session Time displayed for timed sessions (previously blank)
- [x] Remaining Time displayed in timed sessions (previously blank)
- [x] "Unlimited" shown as session time in offline testing mode

###### Fixed
- [x] Various bug fixes

##### Changed
- [x] GUI alignment

##### Back End (Python)
###### Added
- [ ] Nil

###### Fixed
- [x] Various bug fixes

###### Changed
- [x] Fuel required now shows the maximum tank size when the required fuel exceeds amount you can fit in
