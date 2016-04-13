### Changelog:
#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End
##### Version 2.5 - 2016-04-13

###### Minimum Requirements
- Front End v1.1

###### Fixed 
- [x] Fixed a crash bug when entering pit lane.

###### Known Issues
- [x] Wind direction is incorrect in sessions where dynamic weather is enabled

___
#### Front End (Arduino)

v1.1 of the front end has been released which is a mandatory upgrade for v2.4 of the back end.  It contains the new pit information screens when you enter pit lane after completing more than 1 lap.

###### Added 
- [x] Added new screen when entering pit lane
- [x] Added laps on tires
- [x] Added last lap you stopped
- [x] Added optional repairs left when leaving the pits
- [x] Added fuel added last stop
- [x] Added tire wear as a percentage
- [x] Added tire temperatures as colors (blue, green, orange, red)
- [x] Added fuel stint burn rates
- [x] Added last pitted under flag status
- [x] Added fuel to add indicator
- [x] Added fuel to leave with indicator
- [x] Added tire change indicators
- [x] Added quick repair indicator

![](https://camo.githubusercontent.com/546341224edc5caf9741fac26ef90906d7f79f6d/687474703a2f2f696d672e796f75747562652e636f6d2f76692f4c785464695a7a413764512f687164656661756c742e6a7067)

#### Back End
##### Version 2.4 - 2016-04-08

###### Minimum Requirements
- Front End v1.1

###### Added 
- [x] Issue 41: Added circuit layout to log files
- [x] Issue 40: Removed C/F on temperatures in log and create separate entry for measurement
- [x] Issue 35: When on pit road, change to a secondary screen that shows the current stop settings (**requires v1.1 of front end**)
- [x] Re-added the upload burn data to cloud option.  Use -c to disable. Upload occur after each session has ended.

###### Fixed 
- [x] Issue 38: New log file is not created when the session changes

###### Known Issues
- [ ] Wind direction is incorrect in sessions where dynamic weather is enabled

___
#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End
##### Version 2.2 - 2016-01-15

###### Minimum Requirements
- Front End v1.0

###### Added 
- [x] A completely new way to capture the fuel and distance telemetry data out of iRacing has been written and implemented allowing for more accurate fuel usage and estimation calculations

###### Fixed 
- [x] The new addition in this release fixes an issue where it would be possible to go faster and further than the back end would calculate causing incorrect fuel burn data to be displayed.

###### Known Issues
- [x] Wind direction is incorrect in sessions where dynamic weather is enabled
- [x] I broke the code where changing from a qualifying to a race session no longer creates a new log file. The data is in the practice prior sessions log file.

___

#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End
##### Version 2.2 Beta 2 - Pre-Release

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Fixed issue where time remaining would not clear a previous value correctly
- [x] Fixed issue where laps completed would not clear a previous value correctly

###### Added 
- [x] McLaren MP4-30 added to the list of cars that uses weight instead of volume for fuel

###### Known Issues
- [x] Wind direction is incorrect in sessions where dynamic weather is enabled

___

#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End
##### Version 2.1 - 2016-01-01

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Timed sessions now calculate the requried fuel by total time + 1 lap
- [x] COM ports 10 and greater no longer cause an error on the back end 

###### Known Issues
- [x] Wind direction is incorrect in sessions where dynamic weather is enabled

___

#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End
##### Version 2.0 - 2015-12-18

###### Minimum Requirements
- Front End v1.0

###### Added 
- [x] New SessionLapsRemainEx parameter is now used and more accurately reflects the laps remaining in the current session.
- [x] New TrackCleanup parameter is now logged indicating if the track is cleaned between sessions 
- [x] New TrackDynamicTrack parameter is now logged indicating if the track uses the dynamic surface model 
- [x] New SessionTrackRubberState parameter is now logged indicating if the track uses the dynamic surface mode
- [x] Track rubber state is now shown in the information message box on device load
- [x] Wind information is now shown in the information message box on device load

###### Removed 
- [x] All uploading to the cloud features have been removed until the performance issues are resolved
- [x] Removed the track name from the information message box on device load
- [x] Removed the session type from the information message box on device load

###### Known Issues
- [x] Wind direction is incorrect in sessions where dynamic weather is enabled

___

#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End (Python)
##### Version 1.8 - 2015-11-28

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Issue #28 - Suppress the yellow "in pit lane" messages
 - [x] Use the `-spl` argument to supress pit lane messages
- [x] Issue #27 - The device sometimes thinks you are using the pace car in a team session

___


#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End (Python)
##### Version 1.7 - 2015-11-28

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Issue where the uploading of logs at the end of a session would crash the backend

___

#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you...

#### Back End (Python)
##### Version 1.6 - 2015-11-27

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Incorrectly waved checkered flag whilst in pit lane

___


#### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

#### Back End (Python)
##### Version 1.5 - 2015-11-26

Note: The argument for imperial has changed from -gallons to -g . 

###### Minimum Requirements
- Front End v1.0

###### Fixed 
- [x] Issue #12: Those cars that require fuel in KG's still uses Litres/Gallons
  - [x] KGs or lbs now used for calculations on the HPD and F1 cars

###### Added   
- [x] Flag state added to log file
- [x] Logs can now be pushed to the cloud for fuel burn data modelling across all Pit Board users
  - [x] use the -c argument to disable this feature
  - [ ] Still yet to implement the data modelling side in the cloud
- [x] Added optional arguments to turn off and on certain features
  - `optional arguments:`
  - `-h, --help  show this help message and exit`
  - `-g          This option will use imperial measurements for weight, temperature and volume`
  - `-c          This option will disable the uploading of logs to the cloud`
  - `-sfr        This option will disable the show fuel required on exit informational messages`
  - `--version   show program's version number and exit`
  
___

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


#### Back End (Python)
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

#### Version 1.3 - 2015-11-01
##### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

##### Back End (Python)
###### Fixed 
- [x] Issue #21: Backend crashes when trying to display special characters eg. Autódromo José Carlos Pace

###### Added

___

#### Version 1.2 - 2015-10-08
##### Front End (Arduino)

There are no updates to the front end Arduino code in this release which means you do not need to re-upload the code to the arduino device.  Only the back end files in the release section need to be updated by you.

##### Back End (Python)
###### Fixed 
- [x] Issue #18:The reset car button in iRacing now resets the current fuel stats.
- [x] Information messages when running the backend without arguments now reflects .exe release
- [x] Delay after writing to screen has been reduced from .3 to .25
- [x] Issue #17: Fixed issue which causes a hard crash when going from practice directly to a race session
- [x] Fixed issue where car details and class were picking up the wrong car 
- [x] Issue #15: Incorrect data when going from a timed to lap session

###### Added
- [x] Logging of all statistics to a unique local file for each session
- [x] Information message is displayed on the screen when connection is lost to iRacing
- [x] Backend will automatically retry re-connection when iRacing closes or forces you into a new race session

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
