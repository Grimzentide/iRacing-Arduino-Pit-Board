// This switch provides compilation modes for 320x240 and 480x320 screens
// Enable the HDGFX switch if you are compiling for 480x320

#define HDGFX true

#include <Adafruit_GFX.h>	// Core graphics library
#if HDGFX == true
	#include <MCUFRIEND_kbv.h>
#else
	#include <Adafruit_TFTLCD.h> // Hardware-specific library
#endif

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK           0x0000
#define DARKGREEN       0x03E0
#define LIGHTGREY       0xC618
#define DARKGREY        0x7BEF
#define BLUE            0x001F
#define GREEN           0x07E0
#define RED             0xF800
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define ORANGE          0xFD20

#if HDGFX == true
	#define ROW_1_HDR_TXT_YVAL			4
	#define ROW_1_INFO_TXT_YVAL			30
	#define ROW_2_HDR_TXT_YVAL			71
	#define ROW_2_INFO_TXT_YVAL			97
	#define ROW_4_HDR_TXT_YVAL			255
	#define ROW_4_INFO_TXT_YVAL			281
	#define INFO_TXT_SIZE			    	3
#else
	#define ROW_1_HDR_TXT_YVAL			2
	#define ROW_1_INFO_TXT_YVAL			19
	#define ROW_2_HDR_TXT_YVAL			55
	#define ROW_2_INFO_TXT_YVAL			68
	#define ROW_4_HDR_TXT_YVAL			192
	#define ROW_4_INFO_TXT_YVAL			209
	#define INFO_TXT_SIZE		    		2
#endif

#if HDGFX == true
	MCUFRIEND_kbv tft;
#else
	Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#endif

	String sessionLaps = "";       // SESSION LAPS
	String completedLaps = "";     // COMPLETED LAPS
	String remainingLaps = "";     // REMAINING LAPS
	String pitOnLap = "";          // PIT ON LAP
	String lastPitOnLap = "Default";      // LAST PIT ON LAP
	String fuelRequired = "";      // FUEL REQUIRED
	String lapsUntilEmpty = "";    // LAPS UNTIL EMPTY
	String fuelRemaining = "";     // FUEL REMAINING
	String fiveLapAvg = "";        // 5 LAP AVG
	String raceAVG = "";           // RACE AVG
	String str = "";               // INBOUND SERIAL STRING

	String infoMessage[5];
	int clearFuelRemainingTag = 0;
	int clearFuelRequiredTag = 0;
	int clearLapsUntilEmptyTag = 0;
	int clearFiveLapAvgTag = 0;
	int clearRaceAvgTag = 0;
	int updateTitleSessionTime = 0;
	int textSize = 2;
	int fieldLimitLeft;
	int fieldLimitRight;

// PIT STOP SCREEN VARIABLES

	String lastPitStopOnLap = "";
	String optRepairLeft = "";
	String pittedUnderFlag = "";
	String lapsOnTires = "";
	String fuelRequiredAtPitstopVarPitScreen = "";
	String averageFuelBurnStint = "";
	String fuelToLeaveWith = "";
	String fuelAdded = "";
	String fastRepair = "";
	String lfTireChange = "";
	String rfTireChange = "";
	String lrTireChange = "";
	String rrTireChange = "";
	String wear = "";

void setup(void)
{
	// OPEN SERIAL CONNECTION
	Serial.begin(250000);
	tft.reset();
	#if HDGFX == true
		uint16_t identifier = 0x9486;
	#else
		uint16_t identifier = 0x9325;
	#endif
	tft.begin(identifier);
	#if HDGFX
		tft.invertDisplay(true);
		tft.setRotation(1);
	#else
		tft.setRotation(3);
	#endif
	tft.setTextColor(WHITE, BLACK);
	resetScreen();
}

void loop(void)
{
	while (Serial.available())
	{
		String content = "";			// INBOUND SERIAL STRING
		str = Serial.readStringUntil('!');
		content.concat(str);

		switch(str.charAt(0))
		{
		case '@':
			str.remove(0, 1);
			sessionLaps = str;			// SESSION LAPS
			updateTitleSessionTime = updateSessionLaps(sessionLaps, updateTitleSessionTime);
			break;
		case '#':
			str.remove(0, 1);
			completedLaps = str;		// COMPLETED LAPS
			updateCompletedLaps(completedLaps);
			break;
		case '$':
			str.remove(0, 1);
			remainingLaps = str;		// REMAINING LAPS
			updateRemainingLaps(remainingLaps);
			break;
		case '%':
			str.remove(0, 1);
			pitOnLap = str;				// PIT ON LAP
			lastPitOnLap = updatePitOnLap(pitOnLap, lastPitOnLap);
			break;
		case '^':
			str.remove(0, 1);
			fuelRequired = str;			// FUEL REQUIRED
			updateFuelRequired(fuelRequired);
			break;
		case '&':
			str.remove(0, 1);
			lapsUntilEmpty = str;		// LAPS UNTIL EMPTY
			clearLapsUntilEmptyTag = updateLapsUntilEmpty(lapsUntilEmpty, clearLapsUntilEmptyTag);
			break;
		case '*':
			str.remove(0, 1);
			fuelRemaining = str;		// FUEL REMAINING
			clearFuelRemainingTag = updateFuelRemaining(fuelRemaining, fiveLapAvg.toInt(), clearFuelRemainingTag);
			break;
		case '(':
			str.remove(0, 1);
			fiveLapAvg = str;			// 5 LAP AVG
			updateFiveLapAvg(fiveLapAvg);
			break;
		case ')':
			str.remove(0, 1);
			raceAVG = str;				// RACE AVG
			updateRaceAVG(raceAVG);
			break;
		case '?':
			resetScreen();
			break;
		case '~':
			pitLaneScreen();
			break;
		case '-':
			str.remove(0, 1);
			infoMessage[4] = infoMessage[3];
			infoMessage[3] = infoMessage[2];
			infoMessage[2] = infoMessage[1];
			infoMessage[1] = infoMessage[0];
			infoMessage[0] = str;
			updateInfoMessage(infoMessage[0], infoMessage[1], infoMessage[2], infoMessage[3], infoMessage[4]);
			break;
		case 'A':
			str.remove(0, 1);
			lastPitStopOnLap = str;
			updateLastPitStopOnLap(lastPitStopOnLap);
			break;
		case 'B':
			str.remove(0, 1);
			optRepairLeft = str;
			updateOptRepairLeft(optRepairLeft);
			break;
		case 'C':
			str.remove(0, 1);
			pittedUnderFlag = str;
			updatePittedUnderFlag(pittedUnderFlag);
			break;
		case 'D':
			str.remove(0, 1);
			lapsOnTires = str;
			updateLapsOnTires(lapsOnTires);
			break;
		case 'E':
			str.remove(0, 1);
			fuelRequiredAtPitstopVarPitScreen = str;
			updateFuelRequiredAtPitstopVarPitScreen(fuelRequiredAtPitstopVarPitScreen);
			break;
		case 'F':
			str.remove(0, 1);
			averageFuelBurnStint = str;
			updateAverageFuelBurnStint(averageFuelBurnStint);
			break;
		case 'G':
			str.remove(0, 1);
			fuelToLeaveWith = str;
			updateFuelToLeaveWith(fuelToLeaveWith);
			break;
		case 'H':
			str.remove(0, 1);
			fuelAdded = str;
			updateFuelAdded(fuelAdded);
			break;
		case 'I':
			str.remove(0, 1);
			fastRepair = str;
			updateFastRepair(fastRepair);
			break;
		case 'J':
			str.remove(0, 1);
			lfTireChange = str;
			updateLFTireChange(lfTireChange);
			break;
		case 'K':
			str.remove(0, 1);
			rfTireChange = str;
			updateRFTireChange(rfTireChange);
			break;
		case 'L':
			str.remove(0, 1);
			lrTireChange = str;
			updateLRTireChange(lrTireChange);
			break;
		case 'M':
			str.remove(0, 1);
			rrTireChange = str;
			updateRRTireChange(rrTireChange);
			break;
		case 'N':
			str.remove(0, 1);
			wear = str;
			updateWear(wear);
			break;
		}
	}
}

int updateSessionLaps(String sessionLaps, int updateTitleSessionTime)
{
	#if HDGFX == true
	fieldLimitLeft = 4;
	fieldLimitRight = 158;
	#else
	fieldLimitLeft = 4;
	fieldLimitRight = 100;
	#endif

	tft.setTextColor(WHITE, BLACK);

	// SESSION LAPS
	int isTimeNotLaps = sessionLaps.indexOf(':');
	if (isTimeNotLaps >= 1)								// ':' exists
	{
		if (updateTitleSessionTime == 0)				// Change default title from 'Session Laps' to 'Session Time'
		{
			tft.setTextSize(1);
			tft.setTextColor(LIGHTGREY, BLACK);
			#if HDGFX == true
			tft.setCursor(45, ROW_1_HDR_TXT_YVAL);
			#else
			tft.setCursor(16, ROW_1_HDR_TXT_YVAL);
			#endif
			tft.println("SESSION TIME");
			updateTitleSessionTime = 1;
			tft.setTextColor(WHITE, BLACK);
		}
	}
	else
	//if (updateTitleSessionTime == 1)					// Restore default title from 'Session Time' to 'Session Laps'
	{
		tft.setTextSize(1);
		tft.setTextColor(LIGHTGREY, BLACK);
		#if HDGFX == true
		tft.setCursor(45, ROW_1_HDR_TXT_YVAL);
		#else
		tft.setCursor(16, ROW_1_HDR_TXT_YVAL);
		#endif
		tft.println("SESSION LAPS");
		updateTitleSessionTime = 0;
		tft.setTextColor(WHITE, BLACK);
	}

	tft.setTextSize(textSize);

	#if HDGFX == true
	tft.setCursor(8, ROW_1_INFO_TXT_YVAL);			// clear text box, just in case
	#else
	tft.setCursor(28, ROW_1_INFO_TXT_YVAL);
	#endif
	tft.println("        ");

	tft.setCursor(calculateStringStartPosition(sessionLaps, fieldLimitLeft, fieldLimitRight, textSize), ROW_1_INFO_TXT_YVAL);
	tft.println(sessionLaps);

	return updateTitleSessionTime;
}

void updateCompletedLaps(String completedLaps)
{
	// COMPLETED LAPS
	tft.setTextSize(textSize);
	#if HDGFX == true
	fieldLimitLeft = 162;
	fieldLimitRight = 317;
	#else
	fieldLimitLeft = 101;
	fieldLimitRight = 211;
	#endif
	tft.setTextColor(WHITE, BLACK);
	if (completedLaps.toInt() < 10 )					// clear to avoid overlap
	{
		#if HDGFX == true
		tft.setCursor(167, ROW_1_INFO_TXT_YVAL);
		#else
		tft.setCursor(129, ROW_1_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
	}
	tft.setCursor(calculateStringStartPosition(completedLaps, fieldLimitLeft, fieldLimitRight, textSize), ROW_1_INFO_TXT_YVAL);
	tft.println(completedLaps);
}

void updateRemainingLaps(String remainingLaps)
{
	tft.setTextSize(textSize);
	#if HDGFX == true
	fieldLimitLeft = 321;
	fieldLimitRight = 476;
	#else
	fieldLimitLeft = 212;
	fieldLimitRight = 318;
	#endif

	// REMAINING LAPS
	int isTimeNotLaps = remainingLaps.indexOf(':');
	if (isTimeNotLaps == -1)							// ':' does not exist in remaining laps. eg: Unlimited or an Int
	{
		if (remainingLaps.toInt() == 9)					// 9 laps remaining in the race
		{

			#if HDGFX == true
			tft.setCursor(326, ROW_1_INFO_TXT_YVAL);
			#else
			tft.setCursor(238, ROW_1_INFO_TXT_YVAL);
			#endif
			tft.println("        ");		// clear over the display so that the double-digit numbers can't be seen
		}
	}
	tft.setCursor(calculateStringStartPosition(remainingLaps, fieldLimitLeft, fieldLimitRight, textSize), ROW_1_INFO_TXT_YVAL);
	tft.println(remainingLaps);
}

String updatePitOnLap(String pitOnLap, String lastPitOnLap)
{
	tft.setTextColor(YELLOW, BLACK);

	tft.setTextSize(textSize);
	#if HDGFX == true
	fieldLimitLeft = 162;
	fieldLimitRight = 317;
	#else
	fieldLimitLeft = 101;
	fieldLimitRight = 211;
	#endif

	if (pitOnLap != lastPitOnLap)
	{
		#if HDGFX == true
		tft.setCursor(167, ROW_2_INFO_TXT_YVAL);
		#else
		tft.setCursor(129, ROW_2_INFO_TXT_YVAL);
		#endif
		tft.println("        ");			// clear display
	}

	tft.setCursor(calculateStringStartPosition(pitOnLap, fieldLimitLeft, fieldLimitRight, textSize), ROW_2_INFO_TXT_YVAL);
	tft.println(pitOnLap);
	tft.setTextColor(WHITE, BLACK);
	return pitOnLap;
}

void updateFuelRequired(String fuelRequired)
{
	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 4;
	fieldLimitRight = 156;
	#else
	fieldLimitLeft = 4;
	fieldLimitRight = 100;
	#endif

	if (fuelRequired.toInt() < 10 && clearFuelRequiredTag != 1)
	{

		#if HDGFX == true
		tft.setCursor(8, ROW_2_INFO_TXT_YVAL);
		#else
		tft.setCursor(28, ROW_2_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
		clearFuelRequiredTag = 1;
	}

	if (fuelRequired.toInt() >= 10 && clearFuelRequiredTag == 1)
	{
		clearFuelRequiredTag = 0;
	}

	tft.setCursor(calculateStringStartPosition(fuelRequired, fieldLimitLeft, fieldLimitRight, textSize), ROW_2_INFO_TXT_YVAL);
	tft.println(fuelRequired);
}

int updateLapsUntilEmpty(String lapsUntilEmpty, int clearlapsUntilEmptyTag)
{
	tft.setTextSize(textSize);
	#if HDGFX == true
	fieldLimitLeft = 321;
	fieldLimitRight = 476;
	#else
	fieldLimitLeft = 212;
	fieldLimitRight = 318;
	#endif
	if (lapsUntilEmpty.toInt() < 10 && clearlapsUntilEmptyTag != 1)
	{

		#if HDGFX == true
		tft.setCursor(326, ROW_2_INFO_TXT_YVAL);
		#else
		tft.setCursor(239, ROW_2_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
		clearLapsUntilEmptyTag = 1;
	}

	if (lapsUntilEmpty.toInt() >= 10 && clearlapsUntilEmptyTag == 1)
	{
		clearLapsUntilEmptyTag = 0;
	}

	if (lapsUntilEmpty.toInt() < 4)
	{
		tft.setTextColor(RED, BLACK);
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
	}

	// LAPS UNTIL EMPTY
	tft.setCursor(calculateStringStartPosition(lapsUntilEmpty, fieldLimitLeft, fieldLimitRight, textSize), ROW_2_INFO_TXT_YVAL);
	tft.println(lapsUntilEmpty);
	return clearLapsUntilEmptyTag;
}

int updateFuelRemaining(String fuelRemaining, int fiveLapAvg, int clearFuelRemainingTag)
{
	// TELEMETRY VALUE DEFAULTS

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 4;
	fieldLimitRight = 156;
	#else
	fieldLimitLeft = 4;
	fieldLimitRight = 100;
	#endif

	if (fuelRemaining.toInt() < 10 && clearFuelRemainingTag != 1)
	{

		#if HDGFX == true
		tft.setCursor(8, ROW_4_INFO_TXT_YVAL);
		#else
		tft.setCursor(28, ROW_4_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
		clearFuelRemainingTag = 1;
	}

	if (fuelRemaining.toInt() >= 10 && clearFuelRemainingTag == 1)
	{
		clearFuelRemainingTag = 0;
	}

	if (fuelRemaining.toInt() < (fiveLapAvg * 4))
	{
		tft.setTextColor(RED, BLACK);
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
	}

	tft.setCursor(calculateStringStartPosition(fuelRemaining, fieldLimitLeft, fieldLimitRight, textSize), ROW_4_INFO_TXT_YVAL);
	tft.println(fuelRemaining);

	return clearFuelRemainingTag;
}

void updateFiveLapAvg(String fiveLapAvg)
{
	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 164;
	fieldLimitRight = 316;
	#else
	fieldLimitLeft = 101;
	fieldLimitRight = 211;
	#endif

	if (fiveLapAvg.toInt() < 10 && clearFiveLapAvgTag != 1)
	{

		#if HDGFX == true
		tft.setCursor(167, ROW_4_INFO_TXT_YVAL);
		#else
		tft.setCursor(132, ROW_4_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
		clearFiveLapAvgTag = 1;
	}

	if (fiveLapAvg.toInt() >= 10 && clearFiveLapAvgTag == 1)
	{
		clearFiveLapAvgTag = 0;
	}

	tft.setCursor(calculateStringStartPosition(fiveLapAvg, fieldLimitLeft, fieldLimitRight, textSize), ROW_4_INFO_TXT_YVAL);
	tft.println(fiveLapAvg);
}

void updateRaceAVG(String raceAVG)
{
	// TELEMETRY VALUE DEFAULTS

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 321;
	fieldLimitRight = 476;
	#else
	fieldLimitLeft = 101;
	fieldLimitRight = 211;
	#endif
	if (raceAVG.toInt() < 10 && clearRaceAvgTag != 1)
	{

		#if HDGFX == true
		tft.setCursor(326, ROW_4_INFO_TXT_YVAL);
		#else
		tft.setCursor(241, ROW_4_INFO_TXT_YVAL);
		#endif
		tft.println("        ");
		clearRaceAvgTag = 1;
	}

	if (raceAVG.toInt() >= 10 && clearRaceAvgTag == 1)
	{
		clearRaceAvgTag = 0;
	}

	tft.setCursor(calculateStringStartPosition(raceAVG, fieldLimitLeft, fieldLimitRight, textSize), ROW_4_INFO_TXT_YVAL);
	tft.println(raceAVG);
}

void updateInfoMessage(String infoMessage1, String infoMessage2, String infoMessage3, String infoMessage4, String infoMessage5)
{
	// TELEMETRY VALUE DEFAULTS

	tft.setTextSize(2);
	#if HDGFX == true
	tft.fillRect(2, 138, 476, 113, BLACK);
	tft.drawRect(1, 137, 478, 114, DARKGREY);
	#else
	tft.fillRect(3, 105, 316, 83, BLACK);
	tft.drawRect(2, 104, 317, 84, DARKGREY);
	#endif

	// Process 1st infoMessage Colour
	#if HDGFX == true
	tft.setCursor(9, 142);
	#else
	tft.setCursor(5, 105);
	#endif
	if (infoMessage1.charAt(0) == '@')
	{
		tft.setTextColor(WHITE, BLACK);
		tft.print(infoMessage1.substring(1));
	}
	else if (infoMessage1.charAt(0) == '#')
	{
		tft.setTextColor(YELLOW, BLACK);
		tft.print(infoMessage1.substring(1));
	}
	else if (infoMessage1.charAt(0) == '$')
	{
		tft.setTextColor(RED, BLACK);
		tft.print(infoMessage1.substring(1));
	}
	else if (infoMessage1.charAt(0) == '%')
	{
		tft.setTextColor(DARKGREEN, BLACK);
		tft.print(infoMessage1.substring(1));
	}

	// Process 2nd infoMessage Colour
	#if HDGFX == true
	tft.setCursor(9, 163);
	#else
	tft.setCursor(5, 121);
	#endif
	if (infoMessage2.charAt(0) == '@')
	{
		tft.setTextColor(WHITE, BLACK);
		tft.print(infoMessage2.substring(1));
	}
	else if (infoMessage2.charAt(0) == '#')
	{
		tft.setTextColor(YELLOW, BLACK);
		tft.print(infoMessage2.substring(1));
	}
	else if (infoMessage2.charAt(0) == '$')
	{
		tft.setTextColor(RED, BLACK);
		tft.print(infoMessage2.substring(1));
	}
	else if (infoMessage2.charAt(0) == '%')
	{
		tft.setTextColor(DARKGREEN, BLACK);
		tft.print(infoMessage2.substring(1));
	}

	// Process 3rd infoMessage Colour
	#if HDGFX == true
	tft.setCursor(9, 184);
	#else
	tft.setCursor(5, 137);
	#endif
	if (infoMessage3.charAt(0) == '@')
	{
		tft.setTextColor(WHITE, BLACK);
		tft.print(infoMessage3.substring(1));
	}
	else if (infoMessage3.charAt(0) == '#')
	{
		tft.setTextColor(YELLOW, BLACK);
		tft.print(infoMessage3.substring(1));
	}
	else if (infoMessage3.charAt(0) == '$')
	{
		tft.setTextColor(RED, BLACK);
		tft.print(infoMessage3.substring(1));
	}
	else if (infoMessage3.charAt(0) == '%')
	{
		tft.setTextColor(DARKGREEN, BLACK);
		tft.print(infoMessage3.substring(1));
	}

	// Process 4th infoMessage Colour
	#if HDGFX == true
	tft.setCursor(9, 205);
	#else
	tft.setCursor(5, 153);
	#endif
	if (infoMessage4.charAt(0) == '@')
	{
		tft.setTextColor(WHITE, BLACK);
		tft.print(infoMessage4.substring(1));
	}
	else if (infoMessage4.charAt(0) == '#')
	{
		tft.setTextColor(YELLOW, BLACK);
		tft.print(infoMessage4.substring(1));
	}
	else if (infoMessage4.charAt(0) == '$')
	{
		tft.setTextColor(RED, BLACK);
		tft.print(infoMessage4.substring(1));
	}
	else if (infoMessage4.charAt(0) == '%')
	{
		tft.setTextColor(DARKGREEN, BLACK);
		tft.print(infoMessage4.substring(1));
	}

	// Process 5th infoMessage Colour
	#if HDGFX == true
	tft.setCursor(9, 226);
	#else
	tft.setCursor(5, 169);
	#endif
	if (infoMessage5.charAt(0) == '@')
	{
		tft.setTextColor(WHITE, BLACK);
		tft.print(infoMessage5.substring(1));
	}
	else if (infoMessage5.charAt(0) == '#')
	{
		tft.setTextColor(YELLOW, BLACK);
		tft.print(infoMessage5.substring(1));
	}
	else if (infoMessage5.charAt(0) == '$')
	{
		tft.setTextColor(RED, BLACK);
		tft.print(infoMessage5.substring(1));
	}
	else if (infoMessage5.charAt(0) == '%')
	{
		tft.setTextColor(DARKGREEN, BLACK);
		tft.print(infoMessage5.substring(1));
	}

	if (infoMessage1.charAt(0) == '$')
	{
		flashRed();
	}
}

void resetScreen()
{
	tft.setTextColor(LIGHTGREY, BLACK);
	tft.setTextSize(1);

	// BACKGROUND COLOUR
	tft.fillScreen(BLACK);

#if HDGFX == true

	// DRAW BOXES
	tft.drawRect(1, 1, 478, 67, DARKGREY);				//Box Row 1
	tft.drawFastVLine(160, 2, 65, DARKGREY);
	tft.drawFastVLine(319, 2, 65, DARKGREY);
	tft.drawRect(1, 69, 478, 67, DARKGREY);				//Box Row 2
	tft.drawFastVLine(160, 70, 65, DARKGREY);
	tft.drawFastVLine(319, 70, 65, DARKGREY);
	tft.drawRect(1, 137, 478, 114, DARKGREY);			//Box Row 3
	tft.drawRect(1, 252, 478, 67, DARKGREY);			//Box Row 4
	tft.drawFastVLine(160, 253, 65, DARKGREY);
	tft.drawFastVLine(319, 253, 65, DARKGREY);

	// SESSION LAPS
	tft.setCursor(45, ROW_1_HDR_TXT_YVAL);
	tft.println("SESSION LAPS");

	// COMPLETED
	tft.setCursor(213, ROW_1_HDR_TXT_YVAL);
	tft.println("COMPLETED");

	// REMAINING
	tft.setCursor(371, ROW_1_HDR_TXT_YVAL);
	tft.println("REMAINING");

	// FUEL REQUIRED
	tft.setCursor(42, ROW_2_HDR_TXT_YVAL);
	tft.println("FUEL REQUIRED");

	// PIT ON 210
	tft.setCursor(212, ROW_2_HDR_TXT_YVAL);
	tft.println("PIT WINDOW");

	// LAPS UNTIL EMPTY
	tft.setCursor(351, ROW_2_HDR_TXT_YVAL);
	tft.println("LAPS UNTIL EMPTY");

	// FUEL REMAINING
	tft.setCursor(39, ROW_4_HDR_TXT_YVAL);
	tft.println("FUEL REMAINING");

	// 5 LAP AVG
	tft.setCursor(213, ROW_4_HDR_TXT_YVAL);
	tft.println("5 LAP AVG");

	// RACE AVG
	tft.setCursor(375, ROW_4_HDR_TXT_YVAL);
	tft.println("RACE AVG");

#else

	// DRAW BOXES
	tft.drawRect(2, 0, 317, 50, DARKGREY);				//Box Row 1
	tft.drawFastVLine(101, 0, 50, DARKGREY);
	tft.drawFastVLine(207, 0, 50, DARKGREY);
	tft.drawRect(2, 52, 317, 50, DARKGREY);				//Box Row 2
	tft.drawFastVLine(101, 52, 50, DARKGREY);
	tft.drawFastVLine(207, 52, 50, DARKGREY);
	tft.drawRect(2, 104, 317, 84, DARKGREY);			//Box Row 3
	tft.drawRect(2, 190, 317, 50, DARKGREY);			//Box Row 4
	tft.drawFastVLine(101, 190, 50, DARKGREY);
	tft.drawFastVLine(207, 190, 50, DARKGREY);

	// SESSION LAPS
	tft.setCursor(16, ROW_1_HDR_TXT_YVAL);
	tft.println("SESSION LAPS");

	// COMPLETED
	tft.setCursor(129, ROW_1_HDR_TXT_YVAL);
	tft.println("COMPLETED");

	// REMAINING
	tft.setCursor(238, ROW_1_HDR_TXT_YVAL);
	tft.println("REMAINING");

	// FUEL REQUIRED
	tft.setCursor(19, ROW_2_HDR_TXT_YVAL);
	tft.println("FUEL TO ADD");

	// PIT ON LAP
	tft.setCursor(126, ROW_2_HDR_TXT_YVAL);
	tft.println("PIT WINDOW");

	// LAPS UNTIL EMPTY
	tft.setCursor(217, ROW_2_HDR_TXT_YVAL);
	tft.println("LAPS UNTIL EMPTY");

	// FUEL REMAINING
	tft.setCursor(10, ROW_4_HDR_TXT_YVAL);
	tft.println("FUEL REMAINING");

	// 5 LAP AVG
	tft.setCursor(129, ROW_4_HDR_TXT_YVAL);
	tft.println("5 LAP AVG");

	// RACE AVG
	tft.setCursor(241, ROW_4_HDR_TXT_YVAL);
	tft.println("RACE AVG");

	#endif
	tft.setTextSize(textSize);
	tft.setTextColor(WHITE, BLACK);
}

void flashRed()
{
	int flashThisManyTimes = 0;
	while(flashThisManyTimes < 10)
	{
		#if HDGFX == true
		tft.drawRect(1, 1, 478, 67, RED);
		tft.drawFastVLine(160, 2, 65, RED);
		tft.drawFastVLine(319, 2, 65, RED);
		tft.drawRect(1, 69, 478, 67, RED);
		tft.drawFastVLine(160, 70, 65, RED);
		tft.drawFastVLine(319, 70, 65, RED);
		tft.drawRect(1, 137, 478, 114, RED);
		tft.drawRect(1, 252, 478, 67, RED);
		tft.drawFastVLine(160, 253, 65, RED);
		tft.drawFastVLine(319, 253, 65, RED);
		delay(100);
		tft.drawRect(1, 1, 478, 67, ORANGE);
		tft.drawFastVLine(160, 2, 65, ORANGE);
		tft.drawFastVLine(319, 2, 65, ORANGE);
		tft.drawRect(1, 69, 478, 67, ORANGE);
		tft.drawFastVLine(160, 70, 65, ORANGE);
		tft.drawFastVLine(319, 70, 65, ORANGE);
		tft.drawRect(1, 137, 478, 114, ORANGE);
		tft.drawRect(1, 252, 478, 67, ORANGE);
		tft.drawFastVLine(160, 253, 65, ORANGE);
		tft.drawFastVLine(319, 253, 65, ORANGE);
		delay(100);
		#else
		tft.drawRect(2, 0, 317, 50, RED);			//Box Row 1
		tft.drawFastVLine(101, 0, 50, RED);
		tft.drawFastVLine(207, 0, 50, RED);
		tft.drawRect(2, 52, 317, 50, RED);			//Box Row 2
		tft.drawFastVLine(101, 52, 50, RED);
		tft.drawFastVLine(207, 52, 50, RED);
		tft.drawRect(2, 104, 317, 84, RED);			//Box Row 3
		tft.drawRect(2, 190, 317, 50, RED);			//Box Row 4
		tft.drawFastVLine(101, 190, 50, RED);
		tft.drawFastVLine(207, 190, 50, RED);
		delay(100);
		tft.drawRect(2, 0, 317, 50, ORANGE);			//Box Row 1
		tft.drawFastVLine(101, 0, 50, ORANGE);
		tft.drawFastVLine(207, 0, 50, ORANGE);
		tft.drawRect(2, 52, 317, 50, ORANGE);			//Box Row 2
		tft.drawFastVLine(101, 52, 50, ORANGE);
		tft.drawFastVLine(207, 52, 50, ORANGE);
		tft.drawRect(2, 104, 317, 84, ORANGE);			//Box Row 3
		tft.drawRect(2, 190, 317, 50, ORANGE);			//Box Row 4
		tft.drawFastVLine(101, 190, 50, ORANGE);
		tft.drawFastVLine(207, 190, 50, ORANGE);
		delay(100);
		#endif
		flashThisManyTimes++;
	}

	#if HDGFX == true
	tft.drawRect(1, 1, 478, 67, DARKGREY);
	tft.drawFastVLine(160, 1, 65, DARKGREY);
	tft.drawFastVLine(319, 1, 65, DARKGREY);
	tft.drawRect(1, 69, 478, 67, DARKGREY);
	tft.drawFastVLine(160, 69, 65, DARKGREY);
	tft.drawFastVLine(319, 69, 65, DARKGREY);
	tft.drawRect(1, 137, 478, 114, DARKGREY);
	tft.drawRect(1, 252, 478, 67, DARKGREY);
	tft.drawFastVLine(160, 253, 65, DARKGREY);
	tft.drawFastVLine(319, 253, 65, DARKGREY);
	#else
	tft.drawRect(2, 0, 317, 50, DARKGREY);				//Box Row 1
	tft.drawFastVLine(101, 0, 50, DARKGREY);
	tft.drawFastVLine(207, 0, 50, DARKGREY);
	tft.drawRect(2, 52, 317, 50, DARKGREY);				//Box Row 2
	tft.drawFastVLine(101, 52, 50, DARKGREY);
	tft.drawFastVLine(207, 52, 50, DARKGREY);
	tft.drawRect(2, 104, 317, 84, DARKGREY);			//Box Row 3
	tft.drawRect(2, 190, 317, 50, DARKGREY);			//Box Row 4
	tft.drawFastVLine(101, 190, 50, DARKGREY);
	tft.drawFastVLine(207, 190, 50, DARKGREY);
	#endif
}

void pitLaneScreen()
{
	tft.setTextColor(LIGHTGREY, BLACK);
	tft.setTextSize(1);

	// BACKGROUND COLOUR
	tft.fillScreen(BLACK);

#if HDGFX == true
	// DRAW BOXES
	tft.drawRect(1, 15, 318, 304, DARKGREY);			//Left Box
	tft.drawRect(320, 15, 158, 304, DARKGREY);			//Right Box

	// LAST STINT
	tft.setCursor(121, 2);
	tft.println("LAST PIT STOP");

	// STOPPED ON LAP
	tft.setCursor(48, 19);
	tft.println("STOPPED LAP");

	// OPT REPAIR LEFT
	tft.setCursor(48, 72);
	tft.println("FAST REPAIR");

	// PITTED UNDER
	tft.setCursor(45, 125);
	tft.println("PITTED UNDER");

	// FUEL ADDED
	tft.setCursor(209, 19);
	tft.println("FUEL ADDED");

	// LAPS ON TIRES
	tft.setCursor(200, 72);
	tft.println("LAPS ON TIRES");

	// STINT FUEL AVG
	tft.setCursor(197, 125);
	tft.println("STINT FUEL AVG");

	// WEAR
	tft.setCursor(133, 178);
	tft.println("TIRE WEAR");

	// THIS PIT STOP
	tft.setCursor(364, 2);
	tft.println("PIT SETTINGS");

	// FUEL REQUIRED
	tft.setCursor(364, 19);
	tft.println("FUEL TO DROP");

	// FUEL TO LEAVE WITH
	tft.setCursor(346, 72);
	tft.println("FUEL TO LEAVE WITH");

	// FAST REPAIR
	tft.setCursor(355, 125);
	tft.println("USE FAST REPAIR");

	// REPLACE TIRES
	tft.setCursor(361, 178);
	tft.println("REPLACE TIRES");

#else
	// DRAW BOXES
	tft.drawRect(1, 15, 196, 224, DARKGREY);			//Left Box
	tft.drawRect(198, 15, 122, 224, DARKGREY);			//Right Box

	// LAST STINT
	tft.setCursor(64, 2);
	tft.println("LAST PIT STOP");

	// STOPPED ON LAP
	tft.setCursor(23, 18);
	tft.println("STOPPED LAP");

	// OPT REPAIR LEFT
	tft.setCursor(23, 59);
	tft.println("FAST REPAIR");

	// PITTED UNDER
	tft.setCursor(20, 100);
	tft.println("PITTED UNDER");

	// FUEL ADDED
	tft.setCursor(120, 18);
	tft.println("FUEL ADDED");

	// LAPS ON TIRES
	tft.setCursor(112, 59);
	tft.println("LAPS ON TIRES");

	// STINT FUEL AVG
	tft.setCursor(110, 100);
	tft.println("STINT FUEL AVG");

	// WEAR
	tft.setCursor(75, 141);
	tft.println("TIRE WEAR");

	// THIS PIT STOP
	tft.setCursor(226, 2);
	tft.println("PIT SETTINGS");

	// FUEL REQUIRED
	tft.setCursor(226, 18);
	tft.println("FUEL TO DROP");

	// FUEL TO LEAVE WITH
	tft.setCursor(209, 59);
	tft.println("FUEL TO LEAVE WITH");

	// FAST REPAIR
	tft.setCursor(217, 100);
	tft.println("USE FAST REPAIR");

	// REPLACE TIRES
	tft.setCursor(223, 141);
	tft.println("REPLACE TIRES");

#endif

	tft.setTextSize(textSize);
	tft.setTextColor(WHITE, BLACK);
}

void updateLastPitStopOnLap(String lastPitStopOnLap)
{
	// LAST PIT STOP ON LAP VARIBLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	  fieldLimitLeft = 4;
	  fieldLimitRight = 158;
	  tft.setCursor(calculateStringStartPosition(lastPitStopOnLap, fieldLimitLeft, fieldLimitRight, textSize), 33);
	#else
	  fieldLimitLeft = 5;
	  fieldLimitRight = 100;
	  tft.setCursor(calculateStringStartPosition(lastPitStopOnLap, fieldLimitLeft, fieldLimitRight, textSize), 31);
	#endif
	tft.println(lastPitStopOnLap);
}

void updateOptRepairLeft(String optRepairLeft)
{
	// FAST REPAIR VARIABLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 4;
	fieldLimitRight = 158;
	tft.setCursor(calculateStringStartPosition(optRepairLeft, fieldLimitLeft, fieldLimitRight, textSize), 86);
	#else
	fieldLimitLeft = 5;
	fieldLimitRight = 100;
	tft.setCursor(calculateStringStartPosition(optRepairLeft, fieldLimitLeft, fieldLimitRight, textSize), 72);
	#endif
	tft.println(optRepairLeft);
}

void updatePittedUnderFlag(String pittedUnderFlag)
{
	// PITTED UNDER FLAG

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 4;
	fieldLimitRight = 158;
	tft.setCursor(5, 139);		//clear overlap
	tft.println("        ");
  tft.drawRect(4, 139, 154, 32, YELLOW);
	tft.setCursor(calculateStringStartPosition(pittedUnderFlag, fieldLimitLeft, fieldLimitRight, textSize), 139);
	#else
	fieldLimitLeft = 5;
	fieldLimitRight = 100;
	tft.setCursor(29, 111);		//clear overlap
	tft.println("        ");
	tft.setCursor(calculateStringStartPosition(pittedUnderFlag, fieldLimitLeft, fieldLimitRight, textSize), 112);
	#endif

	tft.println(pittedUnderFlag);
}

void updateLapsOnTires(String lapsOnTires)
{
	// LAPS ON TIRES VARIBLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 162;
	fieldLimitRight = 316;
	tft.setCursor(calculateStringStartPosition(lapsOnTires, fieldLimitLeft, fieldLimitRight, textSize), 86);
	#else
	fieldLimitLeft = 100;
	fieldLimitRight = 195;
	tft.setCursor(calculateStringStartPosition(lapsOnTires, fieldLimitLeft, fieldLimitRight, textSize), 31);
	#endif

	tft.println(lapsOnTires);
}

void updateFuelRequiredAtPitstopVarPitScreen(String fuelRequiredAtPitstopVarPitScreen)
{
	// FUEL TO DROP VARIBLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 324;
	fieldLimitRight = 476;
	tft.setCursor(calculateStringStartPosition(fuelRequiredAtPitstopVarPitScreen, fieldLimitLeft, fieldLimitRight, textSize), 33);
	#else
	fieldLimitLeft = 200;
	fieldLimitRight = 317;
	tft.setCursor(calculateStringStartPosition(fuelRequiredAtPitstopVarPitScreen, fieldLimitLeft, fieldLimitRight, textSize), 31);
	#endif

	tft.println(fuelRequiredAtPitstopVarPitScreen);
}

void updateAverageFuelBurnStint(String averageFuelBurnStint)
{
	// FUEL BURN AVG STINT

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 162;
	fieldLimitRight = 316;
	tft.setCursor(calculateStringStartPosition(averageFuelBurnStint, fieldLimitLeft, fieldLimitRight, textSize), 139);
	#else
	fieldLimitLeft = 100;
	fieldLimitRight = 195;
	tft.setCursor(calculateStringStartPosition(averageFuelBurnStint, fieldLimitLeft, fieldLimitRight, textSize), 112);
	#endif

	tft.println(averageFuelBurnStint);
}

void updateFuelToLeaveWith(String fuelToLeaveWith)
{
	// FUEL TO LEAVE WITH

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 324;
	fieldLimitRight = 476;
	tft.setCursor(calculateStringStartPosition(fuelToLeaveWith, fieldLimitLeft, fieldLimitRight, textSize), 86);
	#else
	fieldLimitLeft = 200;
	fieldLimitRight = 317;
	tft.setCursor(calculateStringStartPosition(fuelToLeaveWith, fieldLimitLeft, fieldLimitRight, textSize), 72);
	#endif

	tft.println(fuelToLeaveWith);
}

void updateFuelAdded(String fuelAdded)
{
	// FUEL ADDED VARIBLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 162;
	fieldLimitRight = 316;
	tft.setCursor(calculateStringStartPosition(fuelAdded, fieldLimitLeft, fieldLimitRight, textSize), 33);
	#else
	fieldLimitLeft = 100;
	fieldLimitRight = 195;
	tft.setCursor(calculateStringStartPosition(fuelAdded, fieldLimitLeft, fieldLimitRight, textSize), 31);
	#endif
	tft.println(fuelAdded);
}

void updateFastRepair(String fastRepair)
{
	// FAST REPAIR VARIABLE

	tft.setTextSize(textSize);

	#if HDGFX == true
	fieldLimitLeft = 324;
	fieldLimitRight = 476;
	tft.setCursor(calculateStringStartPosition(fastRepair, fieldLimitLeft, fieldLimitRight, textSize), 139);
	#else
	fieldLimitLeft = 200;
	fieldLimitRight = 318;
	tft.setCursor(calculateStringStartPosition(fastRepair, fieldLimitLeft, fieldLimitRight, textSize), 112);
	#endif

	tft.println(fastRepair);
}

void updateLFTireChange(String lfTireChange)
{
	// LF TEXT

	tft.setTextSize(textSize);
	if (lfTireChange == "YES")
	{
		tft.setTextColor(WHITE, DARKGREEN);
		#if HDGFX == true
		tft.fillRoundRect(323,191,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(202,153,56,41,8,DARKGREEN);
		#endif
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
		#if HDGFX == true
		tft.fillRoundRect(323,191,74,59,8,BLACK);
		tft.drawRoundRect(323,191,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(201,150,56,41,8,BLACK);
		tft.drawRoundRect(201,150,56,41,8,DARKGREEN);
		#endif
	}

	#if HDGFX == true
	fieldLimitLeft = 323;
	fieldLimitRight = 397;
	tft.setCursor(calculateStringStartPosition(lfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 210);
	#else
	fieldLimitLeft = 201;
	fieldLimitRight = 257;
	tft.setCursor(calculateStringStartPosition(lfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 165);
	#endif
	tft.println(lfTireChange);
}

void updateRFTireChange(String rfTireChange)
{
	// RF TEXT

	tft.setTextSize(textSize);
	if (rfTireChange == "YES")
	{
		tft.setTextColor(WHITE, DARKGREEN);
		#if HDGFX == true
		tft.fillRoundRect(400,191,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(259,150,56,41,8,DARKGREEN);
		#endif
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
		#if HDGFX == true
		tft.fillRoundRect(400,191,74,59,8,BLACK);
		tft.drawRoundRect(400,191,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(259,150,56,41,8,BLACK);
		tft.drawRoundRect(259,150,56,41,8,DARKGREEN);
		#endif
	}
	#if HDGFX == true
	fieldLimitLeft = 400;
	fieldLimitRight = 474;
	tft.setCursor(calculateStringStartPosition(rfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 210);
	#else
	fieldLimitLeft = 259;
	fieldLimitRight = 315;
	tft.setCursor(calculateStringStartPosition(rfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 162);
	#endif
	tft.println(rfTireChange);
}

void updateLRTireChange(String lrTireChange)
{
	// LR TEXT

	tft.setTextSize(textSize);
	if (lrTireChange == "YES")
	{
		tft.setTextColor(WHITE, DARKGREEN);
		#if HDGFX == true
		tft.fillRoundRect(323,255,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(201,194,56,41,8,DARKGREEN);
		#endif
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
		#if HDGFX == true
		tft.fillRoundRect(323,255,74,59,8,BLACK);
		tft.drawRoundRect(323,255,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(201,194,56,41,8,BLACK);
		tft.drawRoundRect(201,194,56,41,8,DARKGREEN);
		#endif
	}

	#if HDGFX == true
	fieldLimitLeft = 323;
	fieldLimitRight = 397;
	tft.setCursor(calculateStringStartPosition(lrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 274);
	#else
	fieldLimitLeft = 201;
	fieldLimitRight = 257;
	tft.setCursor(calculateStringStartPosition(lrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 207);
	#endif
	tft.println(lrTireChange);
}

void updateRRTireChange(String rrTireChange)
{
	// RR TEXT

	tft.setTextSize(textSize);
	if (rrTireChange == "YES")
	{
		tft.setTextColor(WHITE, DARKGREEN);
		#if HDGFX == true
		tft.fillRoundRect(400,255,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(260,194,56,41,8,DARKGREEN);
		#endif
	}
	else
	{
		tft.setTextColor(WHITE, BLACK);
		#if HDGFX == true
		tft.fillRoundRect(400,255,74,59,8,BLACK);
		tft.drawRoundRect(400,255,74,59,8,DARKGREEN);
		#else
		tft.fillRoundRect(260,194,56,41,8,BLACK);
		tft.drawRoundRect(260,194,56,41,8,DARKGREEN);
		#endif
	}

	#if HDGFX == true
	fieldLimitLeft = 400;
	fieldLimitRight = 474;
	tft.setCursor(calculateStringStartPosition(rrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 274);
	#else
	fieldLimitLeft = 260;
	fieldLimitRight = 317;
	tft.setCursor(calculateStringStartPosition(rrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 207);
	#endif

	tft.println(rrTireChange);
}

void updateWear(String wear)
{
	int wearWheel;
	int wearLeft;
	int wearMiddle;
	int wearRight;
	int tempLeft;
	int tempMiddle;
	int tempRight;

	int s1_x;
	int s2_x;
	int s3_x;
	int s13_w;
	int s2_w;
	int disp_y;
	int disp_h;
	int s1_tx;
	int s3_tx;
	int s2_tx;
	int text_y;

	#if HDGFX == true
	s13_w = 76;
	s2_w = 51;
	disp_h = 59;
	#else
	s13_w = 46;
	s2_w = 32;
	disp_h = 41;
	#endif

	tft.setTextSize(textSize);

	int colonIndex = wear.indexOf(':');
	int secondColonIndex = wear.indexOf(':', colonIndex+1);
	int thirdColonIndex = wear.indexOf(':', secondColonIndex+1);
	int fourthColonIndex = wear.indexOf(':', thirdColonIndex+1);
	int fifthColonIndex = wear.indexOf(':', fourthColonIndex+1);
	int sixthColonIndex = wear.indexOf(':', fifthColonIndex+1);

	wearWheel = wear.substring(0, colonIndex).toInt();
	wearLeft = wear.substring(colonIndex+1, secondColonIndex).toInt();
	wearMiddle = wear.substring(secondColonIndex+1,thirdColonIndex).toInt();
	wearRight = wear.substring(thirdColonIndex+1,fourthColonIndex).toInt();
	tempLeft = wear.substring(fourthColonIndex+1,fifthColonIndex).toInt();
	tempMiddle = wear.substring(fifthColonIndex+1,sixthColonIndex).toInt();
	tempRight = wear.substring(sixthColonIndex+1).toInt();

	switch(wearWheel)
	{
	#if HDGFX == true
		case 1:
			s1_x = 6;
			disp_y = 191;
			s3_x = 82;
			s2_x = 57;
			s1_tx = 14;
			s2_tx = 65;
			s3_tx = 116;
			text_y = 210;
			break;
		case 2:
			s1_x = 163;
			disp_y = 191;
			s3_x = 239;
			s2_x = 214;
			s1_tx = 171;
			s2_tx = 222;
			s3_tx = 273;
			text_y = 210;
			break;
		case 3:
			s1_x = 6;
			disp_y = 255;
			s3_x = 83;
			s2_x = 57;
			s1_tx = 14;
			s2_tx = 65;
			s3_tx = 116;
			text_y = 274;
			break;
		case 4:
			s1_x = 163;
			disp_y = 255;
			s3_x = 239;
			s2_x = 214;
			s1_tx = 171;
			s2_tx = 222;
			s3_tx = 273;
			text_y = 274;
			break;
	#else
		case 1:
			s1_x = 7;
			disp_y = 153;
			s3_x = 54;
			s2_x = 38;
			s1_tx = 15;
			s2_tx = 46;
			s3_tx = 77;
			text_y = 165;
			break;
		case 2:
			s1_x = 100;
			disp_y = 153;
			s3_x = 147;
			s2_x = 131;
			s1_tx = 108;
			s2_tx = 139;
			s3_tx = 170;
			text_y = 165;
			break;
		case 3:
			s1_x = 7;
			disp_y = 195;
			s3_x = 54;
			s2_x = 38;
			s1_tx = 15;
			s2_tx = 46;
			s3_tx = 77;
			text_y = 207;
			break;
		case 4:
			s1_x = 100;
			disp_y = 195;
			s3_x = 147;
			s2_x = 131;
			s1_tx = 108;
			s2_tx = 139;
			s3_tx = 170;
			text_y = 207;
			break;
	#endif
	}

	// Left
	#if HDGFX == true
	tft.fillRoundRect(s1_x,disp_y,s13_w,disp_h,8,tempColour(tempLeft));
	tft.setCursor(s1_tx, text_y);
	#else
	tft.fillRoundRect(s1_x,disp_y,s13_w,disp_h,8,tempColour(tempLeft));
	tft.setCursor(s1_tx, text_y);
	#endif

	tft.setTextColor(WHITE, tempColour(tempLeft));
	if (wearLeft == 100)
	{
		tft.println("00");
	}
	else
	{
		tft.println(wearLeft);
	}

	// Right
	tft.setTextColor(WHITE, tempColour(tempRight));
	tft.fillRoundRect(s3_x,disp_y,s13_w,disp_h,8,tempColour(tempRight));
	tft.setCursor(s3_tx, text_y);

	if (wearRight == 100)
	{
		tft.println("00");
	}
	else
	{
		tft.println(wearRight);
	}

	// Middle
	tft.setTextColor(WHITE, tempColour(tempMiddle));

	tft.fillRect(s2_x,disp_y,s2_w,disp_h,tempColour(tempMiddle));
	tft.setCursor(s2_tx, text_y);

	if (wearMiddle == 100)
	{
		tft.println("00");
	}
	else
	{
		tft.println(wearMiddle);
	}

	tft.setTextColor(WHITE, BLACK);
}

uint16_t tempColour (int temp)
{
	uint16_t tempColour;

	if (temp > 115)
	{
		tempColour = RED;
	}
	if (temp > 85 && temp < 115)
	{
		tempColour = ORANGE;
	}
	if (temp > 50 && temp < 85)
	{
		tempColour = DARKGREEN;
	}
	if (temp < 50)
	{
		tempColour = BLUE;
	}
	return tempColour;
}

int calculateStringStartPosition(String string, int left, int right, int textSize)
{
	int pixelsReqForString = (string.length() * (6 * textSize));
	int stringStartPos = (((right - left)/2) - (pixelsReqForString /2) + left);
	return stringStartPos;
}
