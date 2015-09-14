#!/usr/bin/env python3.3
#!python3

import time                                                                         # Required for time.sleep function
import os                                                                           # Required to clear the console screen
import irsdk                                                                        # iRacing SDK - https://github.com/kutu/pyirsdk
import serial                                                                       # Required for the serial connection to the Arduino
import datetime																		# Required to convert session times in human readable form
import sys																			# Requried for command line arguments and to clear the console screen


#####################################################################################
versionNumber = 0.7
#####################################################################################


arduinoSerialSpeed = 250000                                                         # Arduino connection speed (must match speed in Arduino code)
arduinoSerialTimeout = 0                                                            # Timeout length when trying to establish serial connection
waitAfterSerialWrite = .3                                                           # Time delay to ensure the data is written to the Arduino - Suggest not going below 0.2
fuelMultiplier = 1																	# Use litres by default.  Changing this to 0.264172 will make Gallons the default and remove the need for the -gallons argument

ir = irsdk.IRSDK()

try:
	if (str(sys.argv[1])):															# Has a commandline argument been supplied?
		comPort = str(sys.argv[1])
		if (comPort[:3].upper() != "COM"):											# Are the first 3 letters of the argument 'COM'
			os.system('cls')
			print ("Incorrect Arguments")
			print()
			print ("Usage: python ArduinoPitBoard.py <COM Port> [-gallons]")
			print()
			print ("Example 1: python ArduinoPitBoard.py COM7")
			print("or")
			print ("Example 2: python ArduinoPitBoard.py COM7 -gallons")
			sys.exit()			
		print("Attempting connection to the Arduino on",comPort)
		ser = serial.Serial(comPort, arduinoSerialSpeed, timeout=arduinoSerialTimeout) # set up the serial port
except IndexError:
	os.system('cls')
	print ("Insufficient Arguments")
	print()
	print ("Usage: python ArduinoPitBoard.py <COM Port> [-gallons]")
	print()
	print ("Example 1: python ArduinoPitBoard.py COM7")
	print("or")
	print ("Example 2: python ArduinoPitBoard.py COM7 -gallons")
	sys.exit()

try:
	if (str(sys.argv[2])):
		if (str(sys.argv[2]) == "-gallons"):
			fuelMultiplier = 0.264172
except IndexError:
	os.system('cls')

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
onPitRoadFlag = 0
isTimedSession = 0

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
    sendViaSerial(str = infoMessageVar);                                            # Send the string to the Ardunio using the sendViaSerial function

	
clearScreen()                                                          				# Clear the console screen
fuelBurn = []                                                                       # Fuel burn array used for 5 lap and race fuel burn average
fuelRemaining = ir['FuelLevel']   													# Read the current fuel level
carClassMaxFuel = float(str(ir['DriverInfo']['Drivers'][0]['CarClassMaxFuelPct'])[:-2])# Get the class maximum fuel percentage
fuelTankCapacity = ((ir['FuelLevel'] / ir['FuelLevelPct'])*carClassMaxFuel)         # Calculate the fuel tank capacity
lastFuelRemaining = ir['FuelLevel']                                                 # Set the last fuel reading to the current level
remainingLap = ir['SessionLapsRemain']                                              # Set the remaining laps in the current session
currentDistance = ir['LapDistPct']                                                  # Set the current location of the car on the track (10%, 20%, 30%, etc)
currentLap = ir['Lap']                                                              # Set the current lap you are on
SessionLaps = ir['SessionLaps']                                                     # Set the total number of laps in the session

# Get the full event information and send the details to the Ardunio	
trackDisplayName = (ir['WeekendInfo']['TrackDisplayName'])                       	# Track Name
sessionNum = ir['SessionNum']														# Current session number
sessionType = (ir['SessionInfo']['Sessions'][sessionNum]['SessionType'])        	# Session Type = Race, Practice, Qualify, Offline Testing
trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])                             	# Current track temperature
trackWeatherType = (ir['WeekendInfo']['TrackWeatherType'])                      	# Realistic or Constant weather
trackSkies = (ir['WeekendInfo']['TrackSkies'])                                 	 	# Current cloud cover

sendInfoMessage("@     By Brock Cremer")									
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@")									
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@      Version: " + str(versionNumber))									
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@iRacing Arduino Pit Board")									
time.sleep(5)

sendInfoMessage("@Track Temp: " + trackTemp)										# Send the track temperature as an information message in white text
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Sky: " + trackSkies)												# Send the cloud cover as an information message in white text
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Weather: " + trackWeatherType)									# Send the Weather as an information message in white text (Constant or Realistic)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Session: " + sessionType)											# Send the current session as an information message in white text (Offline Testing, Practice, Qualify, Race)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@" + trackDisplayName)     										# Send the track name as an information message in white text
time.sleep(waitAfterSerialWrite)		
	
while True:
	if ir.startup():	
				
		sessionNum = ir['SessionNum']		
		currentDistance = ir['LapDistPct']
		fuelRemaining = ir['FuelLevel']
		fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))
		sendViaSerial(str = fuelRemainingVar)
		
		currentLap = ir['Lap']
		currentLapVar = ('#' + str(format(currentLap, '.0f') + '!'))
		sendViaSerial(str = currentLapVar)		
	
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

		if (ir['IsInGarage'] == 0 and ir['IsOnTrack'] == 0):
			del fuelBurn[:]
			boxThisLap = 0
			sendViaSerial(str = "?!")

		if (ir['OnPitRoad'] == 1 and ir['IsOnTrack'] == 1):
			if onPitRoadFlag == 0 and currentLap >= 1:                                  # If I have already sent the pit lane message once, ignore that I am on pit road
				sendInfoMessage("#" + "On Pit Road: Lap " + str(currentLap))       		# Display an info message on the arduino in Yellow
				onPitRoadFlag = 1                                                  		# Change the flag status to prevent spamming of the info messages

		if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionType']) != sessionType): # If the session changes, print the updated info on the arduino
			sessionType = ((ir['SessionInfo']['Sessions'][sessionNum]['SessionType']))  # Re-set the sessionType variable
			del fuelBurn[:]                                                        		# Erase current fuel usage data
			boxThisLap = 0                                                        		# Remove the box this lap flag
			sendViaSerial(str = "?!")                                             		# Reset the Arduino screen
			sendInfoMessage("@Session: " + sessionType)                         	    # If the session goes from practice to qualify, update the info box on the arduino
		
		if (trackTemp != (ir['WeekendInfo']['TrackSurfaceTemp'])):             		    # Code in place for dynamic track temp changes in the future
			trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])               		    #
			sendInfoMessage("@Track Temp: " + trackTemp)                      		    # If the track temp changes mid race, update the info box on the arduino            	

		if (trackSkies != (ir['WeekendInfo']['TrackSkies'])):                		    # Code in place for dynamic weather changes in the future 
			trackSkies = (ir['WeekendInfo']['TrackSkies'])                   		    #
			sendInfoMessage("@Sky: " + trackSkies)                           		    # If the cloud cover changes mid race, update the info box on the arduino            

		if ir['IsInGarage'] == 1:
			del fuelBurn[:]
			remainingLap = ir['SessionLapsRemain']
			SessionLaps = ir['SessionLaps']
			fuelRemaining = ir['FuelLevel']
			lastFuelRemaining = ir['FuelLevel']
			sendViaSerial(str = "?!");

			SessionLapsVar = ('@' + str(SessionLaps) + '!')
			currentLapVar = ('#' + str(format(currentLap, '.0f') + '!'))
			remainingLapVar = ('$' + str(format(remainingLap, '.0f') + '!'))
			fuelRemainingVar = ('*' + str(format(fuelRemaining*fuelMultiplier, '.2f') + '!'))

			sendViaSerial(str = currentLapVar);
			sendViaSerial(str = fuelRemainingVar);
	
	
		if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionLaps']) == "unlimited"):				# Unlimited laps?
	
			isTimedSession = 1

			if ((ir['SessionInfo']['Sessions'][sessionNum]['SessionTime']) == "unlimited"):			# Unlimted time?  
				sessionTime = 604800
				m, s = divmod(sessionTime, 60)
				h, m = divmod(m, 60)
				
				sessionLapsVar =('@Unlimited!')
				sendViaSerial(str = sessionLapsVar);
				time.sleep(waitAfterSerialWrite)
			else:
				sessionTime = (ir['SessionInfo']['Sessions'][sessionNum]['SessionTime'])         	# Get the amount of time in seconds for this session
				sessionTime = float(sessionTime[:-4])
				m, s = divmod(sessionTime, 60)
				h, m = divmod(m, 60)
				
				sessionLapsVar = ('@' + "%d:%02d" % (h, m) + '!')
				sendViaSerial(str = sessionLapsVar);
				time.sleep(waitAfterSerialWrite)				
			
			if ((ir['SessionTimeRemain']) == "unlimited"):										 	# Unlimted session time?  
				sessionTimeRemainVar = ('$Unlimited!')
				sendViaSerial(str = sessionTimeRemainVar);
				time.sleep(waitAfterSerialWrite)
			else:
				sessionTimeRemain = int(ir['SessionTimeRemain'])         						 	# Get the amount of time in seconds for this session time remaining
				m, s = divmod(sessionTimeRemain, 60)
				h, m = divmod(m, 60)
				
				sessionTimeRemainVar = ('$' + "%d:%02d" % (h, m) + '!')
				sendViaSerial(str = sessionTimeRemainVar);
				time.sleep(waitAfterSerialWrite)			
				
				remainingLap = (sessionTimeRemain / ir['DriverInfo']['DriverCarEstLapTime'])
		else:
			remainingLap = ir['SessionLapsRemain']
			remainingLapVar = ('$' + str(format(remainingLap, '.0f') + '!'))
			sendViaSerial(str = remainingLapVar);
			
			SessionLaps = ir['SessionLaps']
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
				pitOnLapVar = ('%' + str(int((currentLap + estimatedLaps) - 1)) + '!')				
				if fuelRequiredAtPitstop > fuelTankCapacity:
					fuelRequiredAtPitstopVar = ('^' + str(format(fuelTankCapacity*fuelMultiplier,'.2f')) + '!')
				else:
					fuelRequiredAtPitstopVar = ('^' + str(format(fuelRequiredAtPitstop*fuelMultiplier,'.2f')) + '!')
				sendViaSerial(str = pitOnLapVar);
				sendViaSerial(str = fuelRequiredAtPitstopVar);

			if (fuelRemaining < float(averageFuelBurn5Lap) * 2):
				if boxThisLap == 0:
					sendInfoMessage("$Box Box Box (Lap " + str(format(currentLap, '.0f')) + ")")
					boxThisLap = 1
