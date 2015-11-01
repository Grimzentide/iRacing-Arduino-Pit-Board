#!/usr/bin/env python3.3
#!python3
 
import time                                                                         # Required for time.sleep function
import os                                                                           # Required to clear the console screen
import irsdk                                                                        # iRacing SDK - https://github.com/kutu/pyirsdk
import serial                                                                       # Required for the serial connection to the Arduino
import datetime                                                                     # Required to convert session times in human readable form
import sys                                                                          # Requried for command line arguments and to clear the console screen
 
 
#####################################################################################
versionNumber = 1.3
#####################################################################################
 
now = datetime.datetime.now()
 
arduinoSerialSpeed = 250000                                                         # Arduino connection speed (must match speed in Arduino code)
arduinoSerialTimeout = 0                                                            # Timeout length when trying to establish serial connection
waitAfterSerialWrite = .3                                                           # Time delay to ensure the data is written to the Arduino - Suggest not going below 0.2
fuelMultiplier = 1                                                                  # Use litres by default.  Changing this to 0.264172 will make Gallons the default and remove the need for the -gallons argument
logFileName = None
driverID = 0

ir = irsdk.IRSDK()
 
try:
    if (str(sys.argv[1])):                                                          # Has a commandline argument been supplied?
        comPort = str(sys.argv[1])
        if (comPort[:3].upper() != "COM"):                                          # Are the first 3 letters of the argument 'COM'
            os.system('cls')
            print ("Incorrect Arguments")
            print()
            print ("Usage: arduinopitboard_v" + str(versionNumber) + " <COM Port> [-gallons]")
            print()
            print ("Example 1: arduinopitboard_v" + str(versionNumber) + " COM7")
            print("or")
            print ("Example 2: arduinopitboard_v" + str(versionNumber) + " COM7 -gallons")
            sys.exit()          
        print("Attempting connection to the Arduino on",comPort)
        ser = serial.Serial(comPort, arduinoSerialSpeed, timeout=arduinoSerialTimeout) # set up the serial port
except IndexError:
    os.system('cls')
    print ("Insufficient Arguments")
    print()
    print ("Usage: arduinopitboard_v" + str(versionNumber) + " <COM Port> [-gallons]")
    print()
    print ("Example 1: arduinopitboard_v" + str(versionNumber) + " COM7")
    print("or")
    print ("Example 2: arduinopitboard_v" + str(versionNumber) + " COM7 -gallons")
    sys.exit()
 
try:
    if (str(sys.argv[2])):
        if (str(sys.argv[2]) == "-gallons"):
            fuelMultiplier = 0.264172
except IndexError:
    os.system('cls')
 


# Clear the command prompt \ console screen function in Windows
#####################################################################################
def clearScreen():
    os.system('cls')
 
 
# Check if the iRacing service is running
#####################################################################################
while True:
    clearScreen()
    count = 5
    if ir.startup():
        print("Loading iRacing Arduino Pit Board...")
        time.sleep(2)
        break
    else:
        while (count > 0):
            print ("iRacing is currently not running. Retrying connection in " + str(count) + " seconds")
            time.sleep(1)
            count -= 1
            clearScreen()
 
     
# Send to serial port (ie. Ardunio)
#####################################################################################
def sendViaSerial(str):                                                             # Function to send data to the Arduino
        ser.write(bytes(str.encode('ascii')))                                       # Send the string to the Arduino 1 byte at a time.
        time.sleep(waitAfterSerialWrite)                                            # A delay after sending the string via serial to the Arduino prevents the arduino from getting confused
 
         
# Setup information message and send to serial port (ie. Ardunio)
#####################################################################################
def sendInfoMessage(str):                                                           # Function to construct an informational message and limit the characters to 26
    # 1st char in str (set elswhere) defines the colour on the Arduino end          # @ = White; # = Yellow; $ = Red
    infoMessageVar = ('-' + str[:26] + '!')                                         # '-' tells the Arduino that it is an info message, '!' tells the arduino that its the end of the message
    time.sleep(0.3)                                                # A delay after sending the string via serial to the Arduino prevents the arduino from getting confused
    sendViaSerial(str = infoMessageVar);                                            # Send the string to the Ardunio using the sendViaSerial function

def createLogFile():
	sessionNum = (ir['SessionNum'])
	if (ir['WeekendInfo']['SessionID'] == 0):
		logFileName = str(str(now.year) + str(now.month) + str(now.day) + '-' + str(now.hour) + str(now.minute) + ' - ' + (ir['DriverInfo']['Drivers'][driverID]['CarScreenName']) + ' at ' + (ir['WeekendInfo']['TrackDisplayName']) + ' - ' + (ir['SessionInfo']['Sessions'][sessionNum]['SessionType']) + '.log')
	else:
		logFileName = str(str(ir['WeekendInfo']['SessionID']) + ' - ' + ir['DriverInfo']['Drivers'][driverID]['CarScreenName'] + ' at ' + (ir['WeekendInfo']['TrackDisplayName']) + ' - ' + ir['SessionInfo']['Sessions'][sessionNum]['SessionType'] + '.log')
	return logFileName
	
def writeToLog(logFileName ,str):
	with open(logFileName,"a+") as f:
		f.write(str + "\n");

def writeSessionInfoToLog():
	writeToLog (logFileName, "iRacing Arduino Pit Board v" + str(versionNumber))
	writeToLog (logFileName, "Track: " + (ir['WeekendInfo']['TrackDisplayName']))
	writeToLog (logFileName, "Car: " + (ir['DriverInfo']['Drivers'][driverID]['CarScreenName']))
	writeToLog (logFileName, "Session Number: " + str(ir['SessionNum']))
	writeToLog (logFileName, "Session: " + (ir['SessionInfo']['Sessions'][sessionNum]['SessionType'])) 
	writeToLog (logFileName, "Weather: " + (ir['WeekendInfo']['TrackWeatherType']) )
	writeToLog (logFileName, "Sky: " + (ir['WeekendInfo']['TrackSkies']))
	writeToLog (logFileName, "Track Temp: " + (ir['WeekendInfo']['TrackSurfaceTemp']))  
	writeToLog (logFileName, "Driver ID: " + str(ir['DriverInfo']['DriverCarIdx']))
	writeToLog (logFileName, "Starting Fuel: " + str(ir['FuelLevel']))
	writeToLog (logFileName, "Fuel Capacity: " + (str(ir['DriverInfo']['Drivers'][driverID]['CarClassMaxFuelPct'])[:-2]))
	writeToLog (logFileName, "----------------------------------------------------------")
		
def welcomeScreen():	
	clearScreen()                                                                       # Clear the console screen
	
	sessionNum = ir['SessionNum']                                                       # Current session number
	currentSurface = ir['CarIdxTrackSurface'][0]                                        # Fuel burn array used for 5 lap and race fuel burn average
	currentCar = ir['DriverInfo']['Drivers'][driverID]['CarScreenName']

	carClassMaxFuel = float(str(ir['DriverInfo']['Drivers'][driverID]['CarClassMaxFuelPct'])[:-2])# Get the class maximum fuel percentage
	lastFuelRemaining = ir['FuelLevel']   												# Set the last fuel reading to the current level

	# Get the full event information and send the details to the Ardunio    
	rawTrackDisplayName = (ir['WeekendInfo']['TrackDisplayName'])                          # Track Name
	trackDisplayName = str(rawTrackDisplayName.encode('utf-8').decode('ascii', 'ignore'))
	sessionNum = ir['SessionNum']                                                       # Current session number
	sessionType = (ir['SessionInfo']['Sessions'][sessionNum]['SessionType'])            # Session Type = Race, Practice, Qualify, Offline Testing
	trackWeatherType = (ir['WeekendInfo']['TrackWeatherType'])                          # Realistic or Constant weather
	trackSkies = (ir['WeekendInfo']['TrackSkies'])                                      # Current cloud cover
	
	sendInfoMessage("@")
	sendInfoMessage("@     By Brock Cremer")                                    
	sendInfoMessage("@")                                    
	sendInfoMessage("@      Version: " + str(versionNumber))                                    
	sendInfoMessage("@iRacing Arduino Pit Board")                                   
	time.sleep(5)
	 
	sendInfoMessage("@Track Temp: " + trackTemp)                                        # Send the track temperature as an information message in white text
	sendInfoMessage("@Sky: " + trackSkies)                                              # Send the cloud cover as an information message in white text
	sendInfoMessage("@Weather: " + trackWeatherType)                                    # Send the Weather as an information message in white text (Constant or Realistic)
	sendInfoMessage("@Session: " + sessionType)                                         # Send the current session as an information message in white text (Offline Testing, Practice, Qualify, Race)
	sendInfoMessage("@" + trackDisplayName)                                             # Send the track name as an information message in white text

estimatedLaps = 0
flagNewLap = 0
flag10pct = 0
flag20pct = 0
flag30pct = 0
flag40pct = 0
flag50pct = 0
flag60pct = 0
flag70pct = 0
flag80pct = 0
flag90pct = 0
boxThisLap = 0
pitWindowOpen = 0
onPitRoadFlag = 0
isTimedSession = 0
sessionExitFlag = 0
driverID = ir['DriverInfo']['DriverCarIdx']
currentSurface = ir['CarIdxTrackSurface'][0]
sessionNum = ir['SessionNum'] 
carClassMaxFuel = float(str(ir['DriverInfo']['Drivers'][driverID]['CarClassMaxFuelPct'])[:-2])
lastFuelRemaining = ir['FuelLevel']
sessionType = (ir['SessionInfo']['Sessions'][sessionNum]['SessionType'])
fuelBurn = []


try:
    if (str(sys.argv[2])):
        if (str(sys.argv[2]) == "-gallons"):  
            trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])                                 # Current track temperature
            trackTemp = trackTemp[:-2]
            trackTemp = (float(trackTemp) * 9/5) + 32
            trackTemp = "%.2f" % round(trackTemp,2) + " F"			
except IndexError:
    trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])                                 # Current track temperature

logFileName = createLogFile()
writeSessionInfoToLog()
welcomeScreen()                                         
	
while True:    
    if ir.startup():        
		
        if sessionExitFlag == 1:
            sessionExitFlag = 0
            logFileName = createLogFile()
            welcomeScreen()
		
        sessionNum = ir['SessionNum']
        currentDistance = ir['LapDistPct']
        fuelRemaining = ir['FuelLevel']
        fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))
        sendViaSerial(str = fuelRemainingVar)
        lastSurface = currentSurface
        currentSurface = ir['CarIdxTrackSurface'][0]         
        currentLap = ir['Lap']
        currentLapVar = ('#' + str(format(currentLap, '.0f') + '!'))
        sendViaSerial(str = currentLapVar)  

        writeToLog (logFileName, "Lap: " + str(currentLap))
        writeToLog (logFileName, "Fuel Remaining: " + str(format(fuelRemaining*fuelMultiplier, '.2f')))
        
        if(ir['IsOnTrack'] == 1):
            fuelTankCapacity = ((ir['FuelLevel'] / ir['FuelLevelPct'])*carClassMaxFuel)         # Calculate the fuel tank capacity 
     
        if 0.00 <= currentDistance <= 0.100:
            if flagNewLap == 0 and flag90pct == 1:
                fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 1     
                flag10pct = 0
                flag20pct = 0
                flag30pct = 0
                flag40pct = 0
                flag50pct = 0
                flag60pct = 0
                flag70pct = 0
                flag80pct = 0
                flag90pct = 0
        if 0.10 <= currentDistance <= 0.20:
            if flag10pct == 0:
                flag10pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.20 <= currentDistance <= 0.30:
            if flag20pct == 0 and flag10pct == 1:
                flag20pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.30 <= currentDistance <= 0.40:
            if flag30pct == 0 and flag20pct == 1:
                flag30pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.40 <= currentDistance <= 0.50:
            if flag40pct == 0 and flag30pct == 1:
                flag40pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.50 <= currentDistance <= 0.60:
            if flag50pct == 0 and flag40pct == 1:
                flag50pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
                onPitRoadFlag = 0
        if 0.60 <= currentDistance <= 0.70:
            if flag60pct == 0 and flag50pct == 1:
                flag60pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.70 <= currentDistance <= 0.80:
            if flag70pct == 0 and flag60pct == 1:
                flag70pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.80 <= currentDistance <= 0.90:
            if flag80pct == 0 and flag70pct == 1:
                flag80pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0
        if 0.90 <= currentDistance <= 1.000:
            if flag90pct == 0 and flag80pct == 1:
                flag90pct = 1
                if lastFuelRemaining - fuelRemaining > 0:
                    fuelBurn.append(lastFuelRemaining - fuelRemaining)
                lastFuelRemaining = fuelRemaining
                flagNewLap = 0     
 
 
        if (currentSurface == 1) and ((lastSurface > 2) or (lastSurface == 0)):	 # reset button used
            writeToLog (logFileName, "Car Reset")
            del fuelBurn[:]
            boxThisLap = 0                                                              
            pitWindowOpen = 0
            estimatedLaps = 0
            flagNewLap = 0
            flag10pct = 0
            flag20pct = 0
            flag30pct = 0
            flag40pct = 0
            flag50pct = 0
            flag60pct = 0
            flag70pct = 0
            flag80pct = 0
            flag90pct = 0
            boxThisLap = 0
            pitWindowOpen = 0
            onPitRoadFlag = 0
            fuelRemaining = ir['FuelLevel']
            lastFuelRemaining = ir['FuelLevel']
            fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))
            sendViaSerial(str = currentLapVar);
            sendViaSerial(str = fuelRemainingVar);     
            sendViaSerial('(       !')
            sendViaSerial(')       !')
            sendViaSerial('%       !')
            sendViaSerial('^       !')
            sendViaSerial('&       !')
 
        if (ir['IsInGarage'] == 0 and ir['IsOnTrack'] == 0):
            #writeToLog (logFileName, "Tow")
            del fuelBurn[:]
            boxThisLap = 0                                                             
            pitWindowOpen = 0
            estimatedLaps = 0
            flagNewLap = 0
            flag10pct = 0
            flag20pct = 0
            flag30pct = 0
            flag40pct = 0
            flag50pct = 0
            flag60pct = 0
            flag70pct = 0
            flag80pct = 0
            flag90pct = 0
            boxThisLap = 0
            pitWindowOpen = 0
            onPitRoadFlag = 0
            fuelRemaining = ir['FuelLevel']
            lastFuelRemaining = ir['FuelLevel']
            fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))
            sendViaSerial(str = currentLapVar);
            sendViaSerial(str = fuelRemainingVar);     
            sendViaSerial('(       !')
            sendViaSerial(')       !')
            sendViaSerial('%       !')
            sendViaSerial('^       !')
            sendViaSerial('&       !')
 
        if (ir['OnPitRoad'] == 1 and ir['IsOnTrack'] == 1):
            if onPitRoadFlag == 0 and currentLap >= 1:                                  # If I have already sent the pit lane message once, ignore that I am on pit road
                sendInfoMessage("#" + "On Pit Road: Lap " + str(currentLap))            # Display an info message on the arduino in Yellow
                writeToLog (logFileName, "On Pit Road: Lap " + str(currentLap))
                onPitRoadFlag = 1                                                       # Change the flag status to prevent spamming of the info messages
 
        if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionType']) != sessionType): # If the session changes, print the updated info on the arduino
            sessionType = ((ir['SessionInfo']['Sessions'][sessionNum]['SessionType']))  # Re-set the sessionType variable
            writeToLog (logFileName, "Session Type Changed To " + sessionType)
            createLogFile()
            del fuelBurn[:]                                                             # Erase current fuel usage data
            boxThisLap = 0                                                              # Remove the box this lap flag
            pitWindowOpen = 0
            estimatedLaps = 0
            flagNewLap = 0
            flag10pct = 0
            flag20pct = 0
            flag30pct = 0
            flag40pct = 0
            flag50pct = 0
            flag60pct = 0
            flag70pct = 0
            flag80pct = 0
            flag90pct = 0
            boxThisLap = 0
            pitWindowOpen = 0
            onPitRoadFlag = 0
            isTimedSession = 0
            sendViaSerial('@       !') # session laps
            sendViaSerial('#       !') # completed laps
            sendViaSerial('$       !') # remaining laps
            sendViaSerial('^       !') # fuel required
            sendViaSerial('%       !') # pit window
            sendViaSerial('&       !') # laps until empty
            sendViaSerial('(       !') # 5 lap avg
            sendViaSerial(')       !') # race avg
            sendInfoMessage("@Session: " + sessionType)                                 # If the session goes from practice to qualify, update the info box on the arduino  
			
        if ir['IsInGarage'] == 1:
            writeToLog (logFileName, "In Garage")
            del fuelBurn[:]
            boxThisLap = 0                                                              # Remove the box this lap flag
            pitWindowOpen = 0
            estimatedLaps = 0
            flagNewLap = 0
            flag10pct = 0
            flag20pct = 0
            flag30pct = 0
            flag40pct = 0
            flag50pct = 0
            flag60pct = 0
            flag70pct = 0
            flag80pct = 0
            flag90pct = 0
            boxThisLap = 0
            pitWindowOpen = 0
            onPitRoadFlag = 0
            fuelRemaining = ir['FuelLevel']
            lastFuelRemaining = ir['FuelLevel']
            fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))
            sendViaSerial(str = currentLapVar);
            sendViaSerial(str = fuelRemainingVar);     
            sendViaSerial('(       !')
            sendViaSerial(')       !')
            sendViaSerial('%       !')
            sendViaSerial('^       !')
            sendViaSerial('&       !')
     
        if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionLaps']) == "unlimited"):             # Unlimited laps?
            
            isTimedSession = 1
            sessionTimeRemain = int(ir['SessionTimeRemain'])                                    # Get the amount of time in seconds for this session time remaining
            m, s = divmod(sessionTimeRemain, 60)
            h, m = divmod(m, 60)
 
            if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionTime']) == "unlimited"):         # Unlimted time?  
                sessionTime = 604800
                m, s = divmod(sessionTime, 60)
                h, m = divmod(m, 60)                 
                sessionLapsVar =('@Infinite!')
                sendViaSerial(str = sessionLapsVar);                
            else:
                sessionTime = (ir['SessionInfo']['Sessions'][sessionNum]['SessionTime'])            # Get the amount of time in seconds for this session
                sessionTime = float(sessionTime[:-4])
                m, s = divmod(sessionTime, 60)
                h, m = divmod(m, 60)                 
                sessionLapsVar = ('@' + "%d:%02d" % (h, m) + '!')
                sendViaSerial(str = sessionLapsVar); 
                
            if (sessionTimeRemain == 604800): 
                sessionTimeRemainVar = ('$Infinite!')
                sendViaSerial(str = sessionTimeRemainVar);
                remainingLap = (sessionTimeRemain / ir['DriverInfo']['DriverCarEstLapTime'])				
            else:
                sessionTimeRemain = int(ir['SessionTimeRemain'])                                    # Get the amount of time in seconds for this session time remaining
                m, s = divmod(sessionTimeRemain, 60)
                h, m = divmod(m, 60)                 
                sessionTimeRemainVar = ('$' + "%d:%02d" % (h, m) + '!')
                sendViaSerial(str = sessionTimeRemainVar);
                remainingLap = (sessionTimeRemain / ir['DriverInfo']['DriverCarEstLapTime'])
        else:
            remainingLap = ir['SessionLapsRemain']
            remainingLapVar = ('$' + str(format(remainingLap, '.0f') + '!'))
            sendViaSerial(str = remainingLapVar);
             
            SessionLaps = (ir['SessionInfo']['Sessions'][sessionNum]['SessionLaps'])
            SessionLapsVar = ('@' + str(SessionLaps) + '!')
            sendViaSerial(str = SessionLapsVar);
 
             
        if len(fuelBurn) >= 2:            
            averageFuelBurnRace = (sum(fuelBurn)/len(fuelBurn)*10)
            averageFuelBurn5Lap = (sum(fuelBurn[-50:])/len(fuelBurn[-50:])*10)
            estimatedLaps = (fuelRemaining / averageFuelBurn5Lap)               
 
            averageFuelBurn5LapVar = ('(' + str(format(averageFuelBurn5Lap*fuelMultiplier, '.2f') + '!'))
            averageFuelBurnRaceVar = (')' + str(format(averageFuelBurnRace*fuelMultiplier, '.2f') + '!'))
            estimatedLapsVar = ('&' + str(format(estimatedLaps, '.2f') + '!'))
 
            sendViaSerial(str = averageFuelBurn5LapVar);
            sendViaSerial(str = averageFuelBurnRaceVar);
            sendViaSerial(str = estimatedLapsVar);
			
             
            if (estimatedLaps <= remainingLap):
 
                fuelRequiredAtPitstop = (((remainingLap * float(averageFuelBurn5Lap))-fuelRemaining) + (float(averageFuelBurn5Lap) /2))     
 
                writeToLog (logFileName, "5 Lap Burn: " + str(averageFuelBurn5Lap))
                writeToLog (logFileName, "Race Burn: " + str(averageFuelBurnRace))
                writeToLog (logFileName, "Estimated Laps: " + str(estimatedLaps))	
                 
                if fuelRequiredAtPitstop > fuelTankCapacity:
                    fuelRequiredAtPitstopVar = ('^' + str(format(fuelTankCapacity*fuelMultiplier,'.2f')) + '!')
                    writeToLog (logFileName, "Fuel Required at Pit Stop: " + str(format(fuelTankCapacity*fuelMultiplier,'.2f')))
                else:
                    fuelRequiredAtPitstopVar = ('^' + str(format(fuelRequiredAtPitstop*fuelMultiplier,'.2f')) + '!')                
                    writeToLog (logFileName, "Fuel Required at Pit Stop: " + str(format(fuelRequiredAtPitstop*fuelMultiplier,'.2f')))
					
                pitEarlyOnLap = int(((fuelRequiredAtPitstop + fuelRemaining - fuelTankCapacity) / averageFuelBurn5Lap) + currentLap + 1) 
				
                if (pitEarlyOnLap < 0):
                    pitEarlyOnLap = 0
                     
                if (pitEarlyOnLap == currentLap):
                    if (pitWindowOpen == 0):
                        sendInfoMessage("#Pit Window Open (Lap " + str(format(currentLap, '.0f')) + ")")
                        writeToLog (logFileName, "Pit Window Open (Lap " + str(format(currentLap, '.0f')))
                        pitWindowOpen = 1
                 
                if (ir['CarIdxTrackSurface'][0] == 1):
                    pitWindowOpen = 0
                 
                pitLateOnLap = int((currentLap + estimatedLaps) - 1)            
                 
                if (pitEarlyOnLap > pitLateOnLap):
					# Can NOT make it to the end with only 1 more stop... Show the latest you can stop
                    pitOnLapVar = ('%' + str(int((currentLap + estimatedLaps) - 1)) + '!')
                    writeToLog (logFileName, "Pit on Lap: " + str(int((currentLap + estimatedLaps) - 1)))
                else:
					# Can make it to the end with only one more stop, show the earliest and latest you can stop.
                    pitOnLapVar = ('%' + str(pitEarlyOnLap) + '-' + str(pitLateOnLap) + '!')
                    writeToLog (logFileName, "Pit Window: " + str(pitEarlyOnLap) + '-' + str(pitLateOnLap))
                 
                sendViaSerial(str = pitOnLapVar);
                sendViaSerial(str = fuelRequiredAtPitstopVar);
            else:
                # Clear Pit Window Field
                sendViaSerial('%       !')
                 
                # Clear Required Fuel Field
                sendViaSerial('^       !')
             
            if (fuelRemaining < float(averageFuelBurn5Lap) * 2):
                if boxThisLap == 0:
                    sendInfoMessage("$Box Box Box (Lap " + str(format(currentLap, '.0f')) + ")")
                    writeToLog (logFileName, "Box Box Box (Lap " + str(format(currentLap, '.0f')) + ")")
                    boxThisLap = 1
						
    else:
        count = 5
        sessionExitFlag = 1
        sendInfoMessage("@Connection Lost: Retrying")
        writeToLog (logFileName, "Connection Lost: Retrying")
        while (count > 0):
            print ("iRacing is currently not running. Retrying connection in " + str(count) + " seconds")
            time.sleep(1)
            count -= 1
            clearScreen()
