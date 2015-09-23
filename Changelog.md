### Changelog:
#### Version 0.9 - UNRELEASED
##### Front End (Arduino)
###### Updated
- [x] 'Pit on Lap' changed to 'Pit Window' to show the earliest and latest you can now pit

##### Back End (Python)
###### Added
- [x] Pit window functionality showing the earliest and latest you can stop and still make it to the end. 
  - [x] If the pit window is open from the start of the session, only the latest you can stop is shown

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


___
