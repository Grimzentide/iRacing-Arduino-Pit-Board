### Changelog:
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
