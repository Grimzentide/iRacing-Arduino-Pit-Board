#!/usr/bin/env python3.3
#!python3

# Variables that can be modified
#####################################################################################
isLinux = 0                                                                         # 0 = Windows; 1 = Linux
arduinoSerialPort = 'COM7'                                                          # Arduino serial port
arduinoSerialSpeed = 250000                                                         # Arduino connection speed (must match speed in Arduino code)
arduinoSerialTimeout = 0                                                            # Timeout length when trying to establish serial connection
waitAfterSerialWrite = .3                                                           # Time delay to ensure the data is written to the Arduino - Suggest not going below 0.2
fuelMultiplier = 1                                                                  # 1 = metric (litres); 0.264172 = imperial (gallons)
#####################################################################################


# Nothing below this line should require modification
#####################################################################################
import time                                                                         # Required for time.sleep function
import os                                                                           # Required to clear the console screen
import irsdk                                                                        # iRacing SDK - https://github.com/kutu/pyirsdk
import serial                                                                       # Required for the serial connection to the Arduino

ir = irsdk.IRSDK()
ser = serial.Serial(arduinoSerialPort, arduinoSerialSpeed, timeout=arduinoSerialTimeout) # set up the serial port
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
# Clear the console screen function for Windows and *nix in offline testing mode
#####################################################################################
def clearScreen(int):
    if int == 0:
        os.system('cls')
    else:
        os.system('clear')


# Check if the iRacing service is running
#####################################################################################
while True:
	clearScreen(int = isLinux)
	count = 5
	if ir.startup():
		print("Live...")
		time.sleep(2)
		break
	else:
		while (count > 0):
			print ("iRacing is currently not running. Retrying connection in " + str(count) + " seconds")
			time.sleep(1)
			count -= 1
			clearScreen(int = isLinux)

	
# Send to serial port (ie. Ardunio)
#####################################################################################
def sendViaSerial(str):                                                             # Function to send data to the Arduino
        ser.write(bytes(str.encode('ascii')))                                       # Send the string to the Arduino 1 byte at a time.
        time.sleep(waitAfterSerialWrite)                                            # A delay after sending the string via serial to the Arduino prevents the arduino from getting confused

		
# Setup information message and send to serial port (ie. Ardunio)
#####################################################################################
def sendInfoMessage(str):                                                           #
    # 1st char in str (set elswhere) defines the colour on the Arduino end          # @ = White; # = Yellow; $ = Red
    infoMessageVar = ('-' + str[:26] + '!')                                         # '-' tells the Arduino that it is an info message, '!' tells the arduino that its the end of the message
    sendViaSerial(str = infoMessageVar);                                            # Send the string to the Ardunio using the sendViaSerial function

	
# Ok, lets start this... 
#####################################################################################	
clearScreen(int = isLinux)                                                          # Clear the console screen
fuelBurn = []                                                                       # Fuel burn array used for 5 lap and race fuel burn average
fuelRemaining = ir['FuelLevel']                                                     # Read the current fuel level
lastFuelRemaining = ir['FuelLevel']                                                 # Set the last fuel reading to the current level
remainingLap = ir['SessionLapsRemain']                                              # Set the remaining laps in the current session
currentDistance = ir['LapDistPct']                                                  # Set the current location of the car on the track (10%, 20%, 30%, etc)
currentLap = ir['Lap']                                                              # Set the current lap you are on
SessionLaps = ir['SessionLaps']                                                     # Set the total number of laps in the session

# Get the full event information and send the details to the Ardunio	
trackDisplayName = ir['WeekendInfo']['TrackDisplayName']                        # Track Name
eventType = (ir['WeekendInfo']['EventType'])                                    # Event Type = Race, Practice, Qualify, Offline Testing
trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])                             # Current track temperature
trackWeatherType = (ir['WeekendInfo']['TrackWeatherType'])                      # Realistic or Constant weather
trackSkies = (ir['WeekendInfo']['TrackSkies'])                                  # Current cloud cover

sendInfoMessage("@Track Temp: " + trackTemp)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Sky: " + trackSkies)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Weather: " + trackWeatherType)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@Session: " + eventType)
time.sleep(waitAfterSerialWrite)
sendInfoMessage("@" + trackDisplayName)
time.sleep(waitAfterSerialWrite)


while True:
	if ir.startup():	
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
			if onPitRoadFlag == 0 and currentLap >= 1:                             # If I have already sent the pit lane message once, ignore that I am on pit road
				sendInfoMessage("#" + "On Pit Road: Lap " + str(currentLap))       # Display an info message on the arduino in Yellow
				onPitRoadFlag = 1                                                  # Change the flag status to prevent spamming of the info messages

		if (ir['WeekendInfo']['EventType'] != eventType):                          # If the session changes, print the updated info on the arduino
			eventType = (ir['WeekendInfo']['EventType'])                           # Re-set the eventType variable
			del fuelBurn[:]                                                        # Erase current fuel usage data
			boxThisLap = 0                                                         # Remove the box this lap flag
			sendViaSerial(str = "?!")                                              # Reset the Arduino screen
			sendInfoMessage("@Session:" + eventType)                               # If the session goes from practice to qualify, update the info box on the arduino

		if (trackTemp != (ir['WeekendInfo']['TrackSurfaceTemp'])):                 # Code in place for dynamic track temp changes in the future
			trackTemp = (ir['WeekendInfo']['TrackSurfaceTemp'])                    #
			sendInfoMessage("@Track Temp: " + trackTemp)                           # If the track temp changes mid race, update the info box on the arduino            	

		if (trackSkies != (ir['WeekendInfo']['TrackSkies'])):                      # Code in place for dynamic weather changes in the future 
			trackSkies = (ir['WeekendInfo']['TrackSkies'])                         #
			sendInfoMessage("@Sky: " + trackSkies)                                 # If the cloud cover changes mid race, update the info box on the arduino            

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

		if remainingLap < 10000:
			sendViaSerial(str = remainingLapVar);
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
				fuelTankCapacity = ((ir['FuelLevel'] / ir['FuelLevelPct']))
				if fuelRequiredAtPitstop > fuelTankCapacity:
					fuelRequiredAtPitstopVar = ('^' + str(format(fuelTankCapacity*fuelMultiplier,'.2f')) + '!')
				sendViaSerial(str = pitOnLapVar);
				sendViaSerial(str = fuelRequiredAtPitstopVar);

			if (fuelRemaining < float(averageFuelBurn5Lap) * 2):
				if boxThisLap == 0:
					sendInfoMessage("$Box Box Box (Lap " + str(format(currentLap, '.0f')) + ")")
					boxThisLap = 1
