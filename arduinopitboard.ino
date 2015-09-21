#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

/* some RGB color definitions                                                 */
#define BLACK           0x0000      /*   0,   0,   0 */
#define NAVY            0x000F      /*   0,   0, 128 */
#define DARKGREEN       0x03E0      /*   0, 128,   0 */
#define DARKCYAN        0x03EF      /*   0, 128, 128 */
#define MAROON          0x7800      /* 128,   0,   0 */
#define PURPLE          0x780F      /* 128,   0, 128 */
#define OLIVE           0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY       0xC618      /* 192, 192, 192 */
#define DARKGREY        0x7BEF      /* 128, 128, 128 */
#define BLUE            0x001F      /*   0,   0, 255 */
#define GREEN           0x07E0      /*   0, 255,   0 */
#define CYAN            0x07FF      /*   0, 255, 255 */
#define RED             0xF800      /* 255,   0,   0 */
#define MAGENTA         0xF81F      /* 255,   0, 255 */
#define YELLOW          0xFFE0      /* 255, 255,   0 */
#define WHITE           0xFFFF      /* 255, 255, 255 */
#define ORANGE          0xFD20      /* 255, 165,   0 */
#define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
#define PINK            0xF81F

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

  String sessionLaps = "";       // SESSION LAPS
  String completedLaps = "";     // COMPLETED LAPS
  String remainingLaps = "";     // REMAINING LAPS
  String pitOnLap = "";          // PIT ON LAP     
  String fuelRequired = "";      // FUEL REQUIRED
  String lapsUntilEmpty = "";    // LAPS UNTIL EMPTY
  String fuelRemaining = "";     // FUEL REMAINING
  String fiveLapAvg = "";        // 5 LAP AVG
  String raceAVG = "";           // RACE AVG
  String str = "";               // INBOUND SERIAL STRING  
  String infoMessage[5];
  int clearFuelRemainingTag = 0;
  int clearLapsUntilEmptyTag = 0;
  int updateTitleSessionTime = 0;
  
void setup(void) 
{
  // OPEN SERIAL CONNECTION
  Serial.begin(250000);
  
  tft.reset();
  uint16_t identifier = 0x9325;
  tft.begin(identifier);
  tft.setRotation(3);
  resetScreen(); 
}

void loop(void) 
{
 while (Serial.available())
 {
    String content = "";         // INBOUND SERIAL STRING
    str = Serial.readStringUntil('!');
    content.concat(str);

    if (str.charAt(0) == '@')
    {
      str.remove(0, 1);
      sessionLaps = str;         // SESSION LAPS
      updateTitleSessionTime = updateSessionLaps(sessionLaps, updateTitleSessionTime);
    }

    if (str.charAt(0) == '#')
    {
      str.remove(0, 1);
      completedLaps = str;       // COMPLETED LAPS
      updateCompletedLaps(completedLaps, pitOnLap);
    }

    if (str.charAt(0) == '$')
    {
      str.remove(0, 1);
      remainingLaps = str;        // REMAINING LAPS
      updateRemainingLaps(remainingLaps);
    }
    
    if (str.charAt(0) == '%')
    {
      str.remove(0, 1);
      pitOnLap = str;            // PIT ON LAP  
      updatePitOnLap(pitOnLap);
    }
    
    if (str.charAt(0) == '^')
    {
      str.remove(0, 1);
      fuelRequired = str;      // FUEL REQUIRED
      updateFuelRequired(fuelRequired);
    }
    
    if (str.charAt(0) == '&')
    {
      str.remove(0, 1);
      lapsUntilEmpty = str;   // LAPS UNTIL EMPTY
      clearLapsUntilEmptyTag = updateLapsUntilEmpty(lapsUntilEmpty, clearLapsUntilEmptyTag);
    }
    
    if (str.charAt(0) == '*')
    {
      str.remove(0, 1);
      fuelRemaining = str;    // FUEL REMAINING
      clearFuelRemainingTag = updateFuelRemaining(fuelRemaining, fiveLapAvg.toFloat(), clearFuelRemainingTag);
    }
    
    if (str.charAt(0) == '(')
    {
      str.remove(0, 1);
      fiveLapAvg = str;       // 5 LAP AVG
      updateFiveLapAvg(fiveLapAvg);
    }
    
    if (str.charAt(0) == ')')
    {
      str.remove(0, 1);
      raceAVG = str;          // RACE AVG
      //updateRaceAVG(raceAVG.toFloat());
      updateRaceAVG(raceAVG);
    } 

    if (str.charAt(0) == '?')
    {
      resetScreen(); 
    } 

    if (str.charAt(0) == '-')
    {
      str.remove(0, 1);
      
      infoMessage[4] = infoMessage[3];
      infoMessage[3] = infoMessage[2];
      infoMessage[2] = infoMessage[1];
      infoMessage[1] = infoMessage[0];
      infoMessage[0] = str;
      updateInfoMessage(infoMessage[0], infoMessage[1], infoMessage[2], infoMessage[3], infoMessage[4]);
    } 
  }
}

int updateSessionLaps(String sessionLaps, int updateTitleSessionTime)
{
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);
  
  int fieldLimitLeft = 4;
  int fieldLimitRight = 100;

  int pixelsReqForString = (sessionLaps.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);

  if (fieldLimitLeft + stringStartPos < 5)
  {
    tft.setCursor(5, 19);
  }
  else
  {
    tft.setCursor(fieldLimitLeft + stringStartPos, 19);
  }
  
  // SESSION LAPS
  int isTimeNotLaps = sessionLaps.indexOf(':');
  if (isTimeNotLaps >= 1)                               // ':' exists
  {
    if (updateTitleSessionTime == 0)                    // Change default title from 'Session Laps' to 'Session Time'
    {
      tft.setTextColor(BLACK);
      tft.setTextSize(1);
      tft.setCursor(15, 2);
      tft.println("SESSION LAPS"); 
      tft.setCursor(15, 2);
      tft.setTextColor(LIGHTGREY);
      tft.println("SESSION TIME"); 
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(2);
      updateTitleSessionTime = 1;
    } 
  }   
      
  tft.println(sessionLaps);   
  return updateTitleSessionTime;    
}


void updateCompletedLaps(String completedLaps, String pitOnLap)
{
  // TELEMETRY VALUE DEFAULTS
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 101;
  int fieldLimitRight = 211;

  if (completedLaps.toInt() > pitOnLap.toInt())
  {
    int pixelsReqForString = (6 * (6 * textSize));  
    int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);

    tft.setCursor(fieldLimitLeft + stringStartPos, 19);
    tft.println("      ");
  }

  int pixelsReqForString = (completedLaps.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  // COMPLETED LAPS
  tft.setCursor(fieldLimitLeft + stringStartPos, 19);
  tft.println(completedLaps);
}


void updateRemainingLaps(String remainingLaps)
{
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 212;
  int fieldLimitRight = 318; 

 
  // REMAINING LAPS
  int isTimeNotLaps = remainingLaps.indexOf(':');
  if (isTimeNotLaps == -1)                            // ':' does not exist in remaining laps. eg: Unlimited or an Int
  {
    if (remainingLaps.toInt() == 9)                   // 9 laps remaining in the race
    {
    tft.setTextColor(WHITE, BLACK);
    
    int pixelsReqForString = (6 * (6 * textSize));  
    int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);

    tft.setCursor(fieldLimitLeft + stringStartPos, 19);
    tft.println("      ");
    } 
  }
  int pixelsReqForString = (remainingLaps.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 19);
  tft.println(remainingLaps);
}


void updatePitOnLap(String pitOnLap)
{
  tft.setTextColor(YELLOW, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);
  
  int fieldLimitLeft = 101;
  int fieldLimitRight = 211;
  
  int pixelsReqForString = (pitOnLap.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 72);
  tft.println(pitOnLap);
}

void updateFuelRequired(String fuelRequired)
{
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 4;
  int fieldLimitRight = 100;
  
  int pixelsReqForString = (fuelRequired.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 72);
  tft.println(fuelRequired);
}  


int updateLapsUntilEmpty(String lapsUntilEmpty, int clearlapsUntilEmptyTag)
{
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 209;
  int fieldLimitRight = 317;

  if (lapsUntilEmpty.toFloat() < 10 && clearlapsUntilEmptyTag != 1)
  {
    tft.setTextColor(WHITE, BLACK);
    
    int pixelsReqForString = (6 * (6 * textSize));  
    int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);

    tft.setCursor(fieldLimitLeft + stringStartPos, 72);
    tft.println("      ");
    clearLapsUntilEmptyTag = 1;

  }
  
  if (lapsUntilEmpty.toFloat() < 4)
  {
    tft.setTextColor(RED, BLACK);    
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);
  }

  int pixelsReqForString = (lapsUntilEmpty.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  // LAPS UNTIL EMPTY
  tft.setCursor(fieldLimitLeft + stringStartPos, 72);
  tft.println(lapsUntilEmpty);
  return clearLapsUntilEmptyTag;
}

  
int updateFuelRemaining(String fuelRemaining, float fiveLapAvg, int clearFuelRemainingTag)
{
  // TELEMETRY VALUE DEFAULTS
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 4;
  int fieldLimitRight = 100;
  
  if (fuelRemaining.toFloat() < 10 && clearFuelRemainingTag != 1)
  {
    tft.setTextColor(WHITE, BLACK);
    
    int pixelsReqForString = (6 * (6 * textSize));  
    int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);

    tft.setCursor(fieldLimitLeft + stringStartPos, 209);
    tft.println("      ");
    
    clearFuelRemainingTag = 1;
  }
 
  if (fuelRemaining.toFloat() < (fiveLapAvg * 4))
  {
    tft.setTextColor(RED, BLACK);
  }
  else
  {
    tft.setTextColor(WHITE, BLACK);
  }

  int pixelsReqForString = (fuelRemaining.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 209);
  tft.println(fuelRemaining);

  return clearFuelRemainingTag;
}  


void updateFiveLapAvg(String fiveLapAvg)
{
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);
  
  int fieldLimitLeft = 101;
  int fieldLimitRight = 211;

  int pixelsReqForString = (fiveLapAvg.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 209);
  tft.println(fiveLapAvg);
} 

void updateRaceAVG(String raceAVG)
{
  // TELEMETRY VALUE DEFAULTS
  tft.setTextColor(WHITE, BLACK);
  int textSize = 2;
  tft.setTextSize(textSize);

  int fieldLimitLeft = 212;
  int fieldLimitRight = 318;  

  int pixelsReqForString = (raceAVG.length() * (6 * textSize));  
  int stringStartPos = (((fieldLimitRight - fieldLimitLeft) - pixelsReqForString) /2);
  
  tft.setCursor(fieldLimitLeft + stringStartPos, 209);
  tft.println(raceAVG);
}

void updateInfoMessage(String infoMessage1, String infoMessage2, String infoMessage3, String infoMessage4, String infoMessage5)
{
  // TELEMETRY VALUE DEFAULTS
  tft.setTextColor(WHITE, BLACK);
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
}


void resetScreen()
{
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
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(15, 2);
  tft.println("SESSION LAPS"); 

  // COMPLETED
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(130, 2);
  tft.println("COMPLETED"); 

  // REMAINING
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(237, 2);
  tft.println("REMAINING"); 

  // FUEL REQUIRED
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(12, 55);
  tft.println("FUEL REQUIRED"); 

  // PIT ON LAP
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(127, 55);
  tft.println("PIT ON LAP"); 

  // LAPS UNTIL EMPTY
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(215, 55);
  tft.println("LAPS UNTIL EMPTY"); 

  // FUEL REMAINING
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(9, 192);
  tft.println("FUEL REMAINING"); 

  // 5 LAP AVG
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(125, 192);
  tft.println("5 LAP AVG"); 

  // RACE AVG
  tft.setTextColor(LIGHTGREY); 
  tft.setTextSize(1);
  tft.setCursor(239, 192);
  tft.println("RACE AVG");   
}
