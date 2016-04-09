#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

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

//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

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
  //tft.reset();
  //uint16_t identifier = 0x9341;
  //tft.begin(identifier);
  tft.setRotation(3);
  tft.setTextColor(WHITE, BLACK);
  resetScreen(); 
}

void loop(void) 
{
 while (Serial.available())
 {
    String content = "";         // INBOUND SERIAL STRING
    str = Serial.readStringUntil('!');
    content.concat(str);

    switch(str.charAt(0))
    {
      case '@':
        str.remove(0, 1);
        sessionLaps = str;         // SESSION LAPS
        updateTitleSessionTime = updateSessionLaps(sessionLaps, updateTitleSessionTime);
        break;
      case '#':
        str.remove(0, 1);
        completedLaps = str;       // COMPLETED LAPS
        updateCompletedLaps(completedLaps);
        break;
      case '$':
        str.remove(0, 1);
        remainingLaps = str;        // REMAINING LAPS
        updateRemainingLaps(remainingLaps);
        break;
      case '%':
        str.remove(0, 1);
        pitOnLap = str;            // PIT ON LAP  
        lastPitOnLap = updatePitOnLap(pitOnLap, lastPitOnLap);
        break;
      case '^':
        str.remove(0, 1);
        fuelRequired = str;      // FUEL REQUIRED
        updateFuelRequired(fuelRequired);
        break;  
      case '&':
        str.remove(0, 1);
        lapsUntilEmpty = str;   // LAPS UNTIL EMPTY
        clearLapsUntilEmptyTag = updateLapsUntilEmpty(lapsUntilEmpty, clearLapsUntilEmptyTag);
        break;
      case '*':
        str.remove(0, 1);
        fuelRemaining = str;    // FUEL REMAINING
        clearFuelRemainingTag = updateFuelRemaining(fuelRemaining, fiveLapAvg.toInt(), clearFuelRemainingTag);
        break;
      case '(':
        str.remove(0, 1);
        fiveLapAvg = str;       // 5 LAP AVG
        updateFiveLapAvg(fiveLapAvg);
        break;
      case ')':
        str.remove(0, 1);
        raceAVG = str;          // RACE AVG
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
  fieldLimitLeft = 4;
  fieldLimitRight = 100;
  tft.setCursor(5, 19);
  tft.setTextColor(WHITE, BLACK);
  
  // SESSION LAPS
  int isTimeNotLaps = sessionLaps.indexOf(':');
  if (isTimeNotLaps >= 1)                               // ':' exists
  {
    if (updateTitleSessionTime == 0)                    // Change default title from 'Session Laps' to 'Session Time'
    {
      tft.setTextSize(1);
      tft.setTextColor(LIGHTGREY, BLACK);
      tft.setCursor(16, 2);
      tft.println("SESSION TIME");       
      updateTitleSessionTime = 1;  
      tft.setTextColor(WHITE, BLACK);
    } 
  }
  
  tft.setTextSize(textSize);
  tft.setCursor(calculateStringStartPosition(sessionLaps, fieldLimitLeft, fieldLimitRight, textSize), 19);  
  tft.println(sessionLaps);   
  return updateTitleSessionTime;    
}


void updateCompletedLaps(String completedLaps)
{
  // TELEMETRY VALUE DEFAULTS  
  
  tft.setTextSize(textSize);
  fieldLimitLeft = 101;
  fieldLimitRight = 211; 
  
  // COMPLETED LAPS
  tft.setCursor(calculateStringStartPosition(completedLaps, fieldLimitLeft, fieldLimitRight, textSize), 19);
  tft.println(completedLaps);
}


void updateRemainingLaps(String remainingLaps)
{  
  tft.setTextSize(textSize);
  fieldLimitLeft = 212;
  fieldLimitRight = 318; 
 
  // REMAINING LAPS
  int isTimeNotLaps = remainingLaps.indexOf(':');
  if (isTimeNotLaps == -1)                            // ':' does not exist in remaining laps. eg: Unlimited or an Int
  {
    if (remainingLaps.toInt() == 9)                   // 9 laps remaining in the race
    {
        
    tft.setCursor(241, 19);
    tft.println("      ");
    } 
  }
  tft.setCursor(calculateStringStartPosition(remainingLaps, fieldLimitLeft, fieldLimitRight, textSize), 19);
  tft.println(remainingLaps);
}


String updatePitOnLap(String pitOnLap, String lastPitOnLap)
{
  tft.setTextColor(YELLOW, BLACK);
  
  tft.setTextSize(textSize);    
  fieldLimitLeft = 101;
  fieldLimitRight = 211;
  
  if (pitOnLap != lastPitOnLap)
  {
    tft.setCursor(132, 72);
    tft.println("      ");
  }
  
  tft.setCursor(calculateStringStartPosition(pitOnLap, fieldLimitLeft, fieldLimitRight, textSize), 72);
  tft.println(pitOnLap);
  tft.setTextColor(WHITE, BLACK);
  return pitOnLap;
}

void updateFuelRequired(String fuelRequired)
{
  
  
  tft.setTextSize(textSize);

  fieldLimitLeft = 4;
  fieldLimitRight = 100;

  if (fuelRequired.toInt() < 10 && clearFuelRequiredTag != 1)
  {
    
    tft.setCursor(28, 72);
    tft.println("      ");
    clearFuelRequiredTag = 1;
  }

  if (fuelRequired.toInt() >= 10 && clearFuelRequiredTag == 1)
  {
    clearFuelRequiredTag = 0;
  }
  
  tft.setCursor(calculateStringStartPosition(fuelRequired, fieldLimitLeft, fieldLimitRight, textSize), 72);
  tft.println(fuelRequired);
}  


int updateLapsUntilEmpty(String lapsUntilEmpty, int clearlapsUntilEmptyTag)
{  
  tft.setTextSize(textSize);
  fieldLimitLeft = 209;
  fieldLimitRight = 317;
  if (lapsUntilEmpty.toInt() < 10 && clearlapsUntilEmptyTag != 1)
  {
    
    tft.setCursor(239, 72);
    tft.println("      ");
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
  tft.setCursor(calculateStringStartPosition(lapsUntilEmpty, fieldLimitLeft, fieldLimitRight, textSize), 72);
  tft.println(lapsUntilEmpty);
  return clearLapsUntilEmptyTag;
}

  
int updateFuelRemaining(String fuelRemaining, int fiveLapAvg, int clearFuelRemainingTag)
{
  // TELEMETRY VALUE DEFAULTS
  
  
  tft.setTextSize(textSize);

  fieldLimitLeft = 4;
  fieldLimitRight = 100;
  
  if (fuelRemaining.toInt() < 10 && clearFuelRemainingTag != 1)
  {
        
    tft.setCursor(28, 209);
    tft.println("      ");    
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

  tft.setCursor(calculateStringStartPosition(fuelRemaining, fieldLimitLeft, fieldLimitRight, textSize), 209);
  tft.println(fuelRemaining);

  return clearFuelRemainingTag;
}  


void updateFiveLapAvg(String fiveLapAvg)
{
  
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 101;
  fieldLimitRight = 211;

  if (fiveLapAvg.toInt() < 10 && clearFiveLapAvgTag != 1)
  {
    
    tft.setCursor(132, 209);
    tft.println("      ");    
    clearFiveLapAvgTag = 1;
  }

  if (fiveLapAvg.toInt() >= 10 && clearFiveLapAvgTag == 1)
  {
    clearFiveLapAvgTag = 0;
  }

  tft.setCursor(calculateStringStartPosition(fiveLapAvg, fieldLimitLeft, fieldLimitRight, textSize), 209);
  tft.println(fiveLapAvg);
} 

void updateRaceAVG(String raceAVG)
{
  // TELEMETRY VALUE DEFAULTS
  
  
  tft.setTextSize(textSize);

  fieldLimitLeft = 212;
  fieldLimitRight = 318;  

  if (raceAVG.toInt() < 10 && clearRaceAvgTag != 1)
  {
        
    tft.setCursor(241, 209);
    tft.println("      ");    
    clearRaceAvgTag = 1;
  }

  if (raceAVG.toInt() >= 10 && clearRaceAvgTag == 1)
  {
    clearRaceAvgTag = 0;
  }

  tft.setCursor(calculateStringStartPosition(raceAVG, fieldLimitLeft, fieldLimitRight, textSize), 209);
  tft.println(raceAVG);
}

void updateInfoMessage(String infoMessage1, String infoMessage2, String infoMessage3, String infoMessage4, String infoMessage5)
{
  // TELEMETRY VALUE DEFAULTS
  
  tft.setTextSize(2);
  tft.fillRect(3, 105, 316, 83, BLACK);
  tft.drawRect(2, 104, 317, 84, DARKGREY);
  
  // Process 1st infoMessage Colour
  tft.setCursor(5, 105); 
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
  tft.setCursor(5, 121);  
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
  tft.setCursor(5, 137);
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
  tft.setCursor(5, 153);
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
  tft.setCursor(5, 169);
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

  // DRAW BOXES
  tft.drawRect(2, 0, 317, 50, DARKGREY);              //Box Row 1
  tft.drawFastVLine(101, 0, 50, DARKGREY);
  tft.drawFastVLine(207, 0, 50, DARKGREY);
  tft.drawRect(2, 52, 317, 50, DARKGREY);             //Box Row 2
  tft.drawFastVLine(101, 52, 50, DARKGREY);
  tft.drawFastVLine(207, 52, 50, DARKGREY);
  tft.drawRect(2, 104, 317, 84, DARKGREY);            //Box Row 3
  tft.drawRect(2, 190, 317, 50, DARKGREY);            //Box Row 4
  tft.drawFastVLine(101, 190, 50, DARKGREY);
  tft.drawFastVLine(207, 190, 50, DARKGREY);

  // SESSION LAPS
  tft.setCursor(16, 2);
  tft.println("SESSION LAPS");   
  
  // COMPLETED
  tft.setCursor(129, 2);
  tft.println("COMPLETED");     

  // REMAINING
  tft.setCursor(238, 2);
  tft.println("REMAINING");  

  // FUEL REQUIRED
  tft.setCursor(19, 55);
  tft.println("FUEL TO ADD"); 

  // PIT ON LAP
  tft.setCursor(126, 55);
  tft.println("PIT WINDOW");  

  // LAPS UNTIL EMPTY
  tft.setCursor(217, 55);
  tft.println("LAPS UNTIL EMPTY");  

  // FUEL REMAINING
  tft.setCursor(10, 192);
  tft.println("FUEL REMAINING"); 

  // 5 LAP AVG
  tft.setCursor(129, 192);
  tft.println("5 LAP AVG");  

  // RACE AVG
  tft.setCursor(241, 192);
  tft.println("RACE AVG"); 

  tft.setTextSize(textSize);    
  tft.setTextColor(WHITE, BLACK);
}

void flashRed()
{
  int flashThisManyTimes = 0;
  while(flashThisManyTimes < 10)
  {
    tft.drawRect(2, 0, 317, 50, RED);              //Box Row 1
    tft.drawFastVLine(101, 0, 50, RED);
    tft.drawFastVLine(207, 0, 50, RED);
    tft.drawRect(2, 52, 317, 50, RED);             //Box Row 2
    tft.drawFastVLine(101, 52, 50, RED);
    tft.drawFastVLine(207, 52, 50, RED);
    tft.drawRect(2, 104, 317, 84, RED);            //Box Row 3
    tft.drawRect(2, 190, 317, 50, RED);            //Box Row 4
    tft.drawFastVLine(101, 190, 50, RED);
    tft.drawFastVLine(207, 190, 50, RED);
    delay(100);
    tft.drawRect(2, 0, 317, 50, ORANGE);              //Box Row 1
    tft.drawFastVLine(101, 0, 50, ORANGE);
    tft.drawFastVLine(207, 0, 50, ORANGE);
    tft.drawRect(2, 52, 317, 50, ORANGE);             //Box Row 2
    tft.drawFastVLine(101, 52, 50, ORANGE);
    tft.drawFastVLine(207, 52, 50, ORANGE);
    tft.drawRect(2, 104, 317, 84, ORANGE);            //Box Row 3
    tft.drawRect(2, 190, 317, 50, ORANGE);            //Box Row 4
    tft.drawFastVLine(101, 190, 50, ORANGE);
    tft.drawFastVLine(207, 190, 50, ORANGE);
    delay(100);
    flashThisManyTimes++;
  }
 
  tft.drawRect(2, 0, 317, 50, DARKGREY);              //Box Row 1
  tft.drawFastVLine(101, 0, 50, DARKGREY);
  tft.drawFastVLine(207, 0, 50, DARKGREY);
  tft.drawRect(2, 52, 317, 50, DARKGREY);             //Box Row 2
  tft.drawFastVLine(101, 52, 50, DARKGREY);
  tft.drawFastVLine(207, 52, 50, DARKGREY);
  tft.drawRect(2, 104, 317, 84, DARKGREY);            //Box Row 3
  tft.drawRect(2, 190, 317, 50, DARKGREY);            //Box Row 4
  tft.drawFastVLine(101, 190, 50, DARKGREY);
  tft.drawFastVLine(207, 190, 50, DARKGREY);
}
  
void pitLaneScreen()
{    
  tft.setTextColor(LIGHTGREY, BLACK);
  tft.setTextSize(1);
  
  // BACKGROUND COLOUR
  tft.fillScreen(BLACK);

  // DRAW BOXES
  tft.drawRect(3, 15, 194, 225, DARKGREY);              //Left Box
  tft.drawRect(198, 15, 122, 225, DARKGREY);            //Right Box

  // LAST STINT
  tft.setCursor(70, 3);
  tft.println("LAST STINT"); 

  // STOPPED ON LAP
  tft.setCursor(11, 17);
  tft.println("STOPPED ON LAP"); 

  // LAPS ON TIRES
  tft.setCursor(109, 17);
  tft.println("LAPS ON TIRES"); 
  
  // OPT REPAIR LEFT
  tft.setCursor(8, 60);
  tft.println("OPT REPAIR LEFT"); 
  
  // FUEL ADDED
  tft.setCursor(118, 60);
  tft.println("FUEL ADDED"); 

  // PITTED UNDER
  tft.setCursor(17, 99);
  tft.println("PITTED UNDER"); 

  // STINT FUEL AVG
  tft.setCursor(106, 99);
  tft.println("STINT FUEL AVG"); 
  
  // WEAR
  tft.setCursor(70, 137);
  tft.println("TIRE WEAR");

  // THIS PIT STOP
  tft.setCursor(223, 3);
  tft.println("PIT SETTINGS"); 

  // FUEL REQUIRED
  tft.setCursor(226, 17);
  tft.println("FUEL TO ADD"); 

  // FUEL TO LEAVE WITH
  tft.setCursor(205, 60);
  tft.println("FUEL TO LEAVE WITH"); 

  // FAST REPAIR
  tft.setCursor(214, 99);
  tft.println("USE FAST REPAIR"); 

  // REPLACE TIRES
  tft.setCursor(220, 137);
  tft.println("REPLACE TIRES");

  tft.setTextSize(textSize);
  tft.setTextColor(WHITE, BLACK);
}

void updateLastPitStopOnLap(String lastPitStopOnLap)
{
  // LAST PIT STOP ON LAP VARIBLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 5;
  fieldLimitRight = 100;  
  tft.setCursor(calculateStringStartPosition(lastPitStopOnLap, fieldLimitLeft, fieldLimitRight, textSize), 31);
  tft.println(lastPitStopOnLap);      
}

void updateOptRepairLeft(String optRepairLeft)
{  
  // FAST REPAIR VARIABLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 5;
  fieldLimitRight = 100;  
  tft.setCursor(calculateStringStartPosition(optRepairLeft, fieldLimitLeft, fieldLimitRight, textSize), 73);
  tft.println(optRepairLeft);  
}

void updatePittedUnderFlag(String pittedUnderFlag)
{
  // PITTED UNDER FLAG
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 5;
  fieldLimitRight = 100;  

  tft.setCursor(29, 111);
  tft.println("      ");

  tft.setCursor(calculateStringStartPosition(pittedUnderFlag, fieldLimitLeft, fieldLimitRight, textSize), 111);
  tft.println(pittedUnderFlag); 
}

void updateLapsOnTires(String lapsOnTires)
{  
  // LAPS ON TIRES VARIBLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 100;
  fieldLimitRight = 195;  

  tft.setCursor(calculateStringStartPosition(lapsOnTires, fieldLimitLeft, fieldLimitRight, textSize), 31);
  tft.println(lapsOnTires);  
}

void updateFuelRequiredAtPitstopVarPitScreen(String fuelRequiredAtPitstopVarPitScreen)
{  
  // FUEL REQUIRED VARIBLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 200;
  fieldLimitRight = 318;  

  tft.setCursor(calculateStringStartPosition(fuelRequiredAtPitstopVarPitScreen, fieldLimitLeft, fieldLimitRight, textSize), 31);
  tft.println(fuelRequiredAtPitstopVarPitScreen);  
}

void updateAverageFuelBurnStint(String averageFuelBurnStint)
{  
  // FUEL BURN AVG STINT
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 100;
  fieldLimitRight = 195;  

  tft.setCursor(calculateStringStartPosition(averageFuelBurnStint, fieldLimitLeft, fieldLimitRight, textSize), 111);
  tft.println(averageFuelBurnStint);  
}

void updateFuelToLeaveWith(String fuelToLeaveWith)
{  
  // FUEL TO LEAVE WITH
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 200;
  fieldLimitRight = 318;  

  tft.setCursor(calculateStringStartPosition(fuelToLeaveWith, fieldLimitLeft, fieldLimitRight, textSize), 73);
  tft.println(fuelToLeaveWith);  
}

void updateFuelAdded(String fuelAdded)
{  
  // FUEL ADDED VARIBLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 100;
  fieldLimitRight = 195;  

  tft.setCursor(calculateStringStartPosition(fuelAdded, fieldLimitLeft, fieldLimitRight, textSize), 73);
  tft.println(fuelAdded);  
}

void updateFastRepair(String fastRepair)
{  
  // FAST REPAIR VARIABLE
  
  tft.setTextSize(textSize);
  
  fieldLimitLeft = 200;
  fieldLimitRight = 318;

  tft.setCursor(calculateStringStartPosition(fastRepair, fieldLimitLeft, fieldLimitRight, textSize), 111);
  tft.println(fastRepair);  
}

void updateLFTireChange(String lfTireChange)
{  
  // LF TEXT
  
  tft.setTextSize(textSize);
  if (lfTireChange == "YES")
  {
    tft.setTextColor(WHITE, DARKGREEN);
    tft.fillRoundRect(201,150,57,43,8,DARKGREEN);
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);  
    tft.fillRoundRect(201,150,57,43,8,BLACK);
    tft.drawRoundRect(201,150,57,43,8,DARKGREEN);    
  }
  fieldLimitLeft = 201;
  fieldLimitRight = 258;

  tft.setCursor(calculateStringStartPosition(lfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 162);
  tft.println(lfTireChange);  
}

void updateRFTireChange(String rfTireChange)
{  
  // RF TEXT
  
  tft.setTextSize(textSize);
  if (rfTireChange == "YES")
  {
    tft.setTextColor(WHITE, DARKGREEN);
    tft.fillRoundRect(260,150,57,43,8,DARKGREEN);
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);  
    tft.fillRoundRect(260,150,57,43,8,BLACK);
    tft.drawRoundRect(260,150,57,43,8,DARKGREEN);    
  }
  fieldLimitLeft = 260;
  fieldLimitRight = 317;

  tft.setCursor(calculateStringStartPosition(rfTireChange, fieldLimitLeft, fieldLimitRight, textSize), 162);
  tft.println(rfTireChange);  
}

void updateLRTireChange(String lrTireChange)
{  
  // LR TEXT
  
  tft.setTextSize(textSize);
  if (lrTireChange == "YES")
  {
    tft.setTextColor(WHITE, DARKGREEN);
    tft.fillRoundRect(201,194,57,43,8,DARKGREEN);
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);    
    tft.fillRoundRect(201,194,57,43,8,BLACK);
    tft.drawRoundRect(201,194,57,43,8,DARKGREEN);  
  }
  fieldLimitLeft = 201;
  fieldLimitRight = 258;

  tft.setCursor(calculateStringStartPosition(lrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 209);
  tft.println(lrTireChange);  
}

void updateRRTireChange(String rrTireChange)
{  
  // RR TEXT
  
  tft.setTextSize(textSize);
  if (rrTireChange == "YES")
  {
    tft.setTextColor(WHITE, DARKGREEN);
    tft.fillRoundRect(260,194,57,43,8,DARKGREEN);
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);    
    tft.fillRoundRect(260,194,57,43,8,BLACK);  
    tft.drawRoundRect(260,194,57,43,8,DARKGREEN);
  }
  fieldLimitLeft = 260;
  fieldLimitRight = 317;

  tft.setCursor(calculateStringStartPosition(rrTireChange, fieldLimitLeft, fieldLimitRight, textSize), 209);
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
    case 1:
      // Left
      tft.fillRoundRect(6,150,46,43,8,tempColour(tempLeft));
      tft.setCursor(11, 162);
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
      tft.fillRoundRect(54,150,46,43,8,tempColour(tempRight));
      tft.setTextColor(WHITE, tempColour(tempRight));
      tft.setCursor(73, 162);
      if (wearRight == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearRight); 
      }
      
      // Middle
      tft.fillRect(35,150,32,43,tempColour(tempMiddle));
      tft.setTextColor(WHITE, tempColour(tempMiddle));
      tft.setCursor(41, 162);
      if (wearMiddle == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearMiddle); 
      }
      break;
    case 2:
      // Left
      tft.fillRoundRect(101,150,46,43,8,tempColour(tempLeft));
      tft.setCursor(107, 162);
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
      tft.fillRoundRect(150,150,46,43,8,tempColour(tempRight));
      tft.setTextColor(WHITE, tempColour(tempRight));
      tft.setCursor(169, 162);
      if (wearRight == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearRight); 
      }
      
      // Middle
      tft.fillRect(132,150,32,43,tempColour(tempMiddle));
      tft.setTextColor(WHITE, tempColour(tempMiddle));
      tft.setCursor(139, 162);
      if (wearMiddle == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearMiddle); 
      }
      break;
    case 3:
      // Left
      tft.fillRoundRect(6,194,46,43,8,tempColour(tempLeft));
      tft.setCursor(11, 206);
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
      tft.fillRoundRect(54,194,46,43,8,tempColour(tempRight));
      tft.setTextColor(WHITE, tempColour(tempRight));
      tft.setCursor(73, 206);
      if (wearRight == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearRight); 
      }
      
      // Middle
      tft.fillRect(35,194,32,43,tempColour(tempMiddle));
      tft.setTextColor(WHITE, tempColour(tempMiddle));
      tft.setCursor(41, 206);
      if (wearMiddle == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearMiddle); 
      }
      break;
    case 4:
      // Left
      tft.fillRoundRect(101,194,46,43,8,tempColour(tempLeft));
      tft.setCursor(107, 206);
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
      tft.fillRoundRect(150,194,46,43,8,tempColour(tempRight));
      tft.setTextColor(WHITE, tempColour(tempRight));
      tft.setCursor(169, 206);
      if (wearRight == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearRight); 
      }
      
      // Middle
      tft.fillRect(132,194,32,43,tempColour(tempMiddle));
      tft.setTextColor(WHITE, tempColour(tempMiddle));
      tft.setCursor(139, 206);
      if (wearMiddle == 100)
      {
        tft.println("00");
      }
      else
      {  
        tft.println(wearMiddle); 
      }
      break;
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
    tempColour = GREEN;
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
  int stringStartPos = (((right - left) - pixelsReqForString) /2) + left;
  return stringStartPos;
}
