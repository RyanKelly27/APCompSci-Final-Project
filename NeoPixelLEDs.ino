#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 60

//HOPEFULLY SOMEONE CAN SEE THIS

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

short pin = 4; 
unsigned long duration;
   
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
    pinMode(pin, INPUT);
  // put your setup code here, to run once:

}
short displayID = 100;  // Stores the last displayedID to increase some effeciency
byte teamID = 0; // Saves the last inputted team color
short scrollState = 0; //Used by various funtions for animation
int delayTick = 0; //Used as a tick for delays.... don't touch this
int delayLength = 0; // Used to control speeds of animations. You can touch this
double tick = NUMPIXELS/2;

uint32_t rwbColors[3] = {0x00FF00, 0x0000FF, 0xFFFFFF};
uint32_t valColors[4] = {0xC0FFCB, 0x4CFF4C, 0x35B235, 0x00FF00};
uint32_t rainbowColors[7] = {0x99CCa2, 0x008080, 0xDD00DD, 0x0000FF, 0xFF0000, 0x7ffd00, 0x00FF00};
uint32_t gearColors[2] = {0x5033FF, 0xAFCC00};
uint32_t blueColors[5] = {0x0000FF, 0x00AAFF, 0xAA00FF, 0x0000FF, 0xAAAAFF};
uint32_t redColors[4] = {0x00FF00, 0x00CC00, 0x20FF00, 0x90FF00};
uint32_t teamColors[2] = {0xAAFF00, 0x010101};
uint32_t coffmanColors[4] = {0xFF0000, 0x606060, 0xAA0000, 0xFFFFFF}; 

void loop() {
  boolean newID =  displayID != round(duration/20.0);
  strip.setBrightness(255);
  displayID = 4;
//displayID = round(duration/20.0);
  if(displayID == 0) //default colors when started meet two colors evenly
  {
    delayLength = 0;
    tick = NUMPIXELS/2; 
   // changingTeam(0x00FF00, 0x0000FF, 0x000000, false);
    fullScrollStrip(newID, rainbowColors, 7, 10);
 //   blinkColor2(0xFFFFFF,0xFFFFFF);
//    strobe(0xFFFFFF);
  }
  else if(displayID == 1)//strip to red display as alliance color
  {
    // Stores the teamID for use later
    teamID = 0;
    delayLength = 0;
    // Sets the whole strip to the red team color
    setStripColor(0x00FF00);
  }
  else if(displayID == 2) // strip to blue as alliance color
  {
    // Stores the teamID for use later
    teamID = 1;
    delayLength = 0;
    // Sets the whole strip to the blue team color
    setStripColor(0x0000FF);
  }
  else if(displayID == 3) //strip badRobotYellow show team color
  {
    delayLength = 0;
//  Sets the whole strip to the BadRobot Yellow
    setStripColor(0x77C700);
  }
  else if(displayID == 4) //strip rainbow colors
  {
    delayLength = 0;
    // Scrolls the LED's in a rainbow fashion
  //  fullScrollStrip(newID, rainbowColors, 7, 4);
    blinkColor2(0x00FF00,0x000000);
  }
  else if(displayID == 5) //blinks lights for low battery or other error
  {
    delayLength = 0;
    // Blinks the LED's rapidly red
    blinkColor2(0x00FF00,0x000000);
  }
  else if(displayID == 6) //switching to blue team beginning of match
  {
    delayLength = 0;
    changingTeam(0x00FF00, 0x0000FF, 0x0000FF, false);
    tick = tick - .5;
    if(tick < NUMPIXELS-NUMPIXELS-10)
    {
      setStripColor(0x0000FF);
        if(tick < NUMPIXELS-NUMPIXELS-11)
        {
          tick = tick + .5;
        }
    }
  }
  else if(displayID == 7) //switching to red team beginning of match
  {
    delayLength = 0;
    changingTeam(0x00FF00, 0x0000FF, 0x00FF00, false);
    tick = tick + .5;
    if(tick > NUMPIXELS+10)
    {
      setStripColor(0x00FF00);
      if(tick > NUMPIXELS+11)
      {
        tick = tick - .5;
      }
    }
  }
  else if(displayID == 8) //shooting green meeting
  {
    delayLength = 0;
    changingTeam(0xFF0000, 0xFF0000, 0xFF0000, false);
   }
  else if(displayID == 9) //america
  { 
    delayLength = 0;  
    fullScrollStrip(newID, rwbColors, 3, 10);
  }
  else if(displayID == 10) //loading a gear blue and yellow
  { 
    delayLength = 0;
    fullScrollStrip(newID, gearColors, 2 , 10);
  }
  else if(displayID == 11) //climbing white
  { 
    delayLength = 0;
    setStripColor(0xFFFFFF);
  }
  else if(displayID == 12) //driving blue team
  { 
    delayLength = 0;
    fullScrollStrip(newID, blueColors, 5, 4); 
  }
  else if(displayID == 13) //driving red team
  { 
    delayLength = 0;
    fullScrollStrip(newID, redColors, 4, 4); 
   }
  strip.show();
  delay(50);
}

void setStripColor(uint32_t color)
{
 if((delayTick = delayTick + 1) < delayLength)
    return;
  delayTick = 0;
  for(short i = 0; i < NUMPIXELS; i++)
      strip.setPixelColor(i, color);
}
void fullScrollStrip(boolean firstRun, uint32_t colors[], int arrayLength, int groupLength)
{
  if((delayTick = delayTick + 1) < delayLength)
    return;
  delayTick = 0;
  boolean flag = false;
  for(int i = 0; i < arrayLength; i++)
  {
    if(strip.getPixelColor(0) == colors[i])
    {
      scrollState = i;
      flag = true;
    }
  }

  if(!flag)
    scrollState = 0;

  int runningLength = 1;
  if(firstRun)
  {
    for(short i = 0; i < NUMPIXELS; i++)
    {
      if(runningLength > groupLength)
      {
        runningLength = 1;
        if((scrollState = scrollState + 1) >= arrayLength)
          scrollState = 0;
      }
      runningLength = runningLength + 1;
      strip.setPixelColor(i, colors[scrollState]);
    }
  }
  else
  {
    if((scrollState = scrollState - 1) < 0)
      scrollState = arrayLength - 1;
    for(short i = NUMPIXELS; i > 0; i--)
      strip.setPixelColor(i, strip.getPixelColor(i-1));

    flag = true;
    for(int i = 2; i <= groupLength; i++)
      if(strip.getPixelColor(1) != strip.getPixelColor(i))
        flag = false;
        
    if(flag)
      strip.setPixelColor(0, colors[scrollState]);
    else
      strip.setPixelColor(0, strip.getPixelColor(1));
  }
}
void changingTeam(uint32_t color, uint32_t color2, uint32_t finalColor, boolean forward)
{
  
  delayTick = delayTick + 1;
    if(delayTick < delayLength)
       return;
    delayTick = 0;
    short nextState = scrollState + (forward ? 1 : -1);
    if(forward && nextState > 9)
        nextState = 0;
    else if(!forward && nextState < 0)
        nextState = 9;
      
    for(short i = 0; i < NUMPIXELS; i++)
    {            
          if(i < tick && i % 10 == 9 - nextState)
        {
            strip.setPixelColor(i, color);
            if(i==NUMPIXELS/2-1)
            {
              
            }
            else if(i==NUMPIXELS/2-2)
            {
              strip.setPixelColor(i+1, 0);
            }
            else if(i==NUMPIXELS/2-3)
            {
              strip.setPixelColor(i+1, 0);
              strip.setPixelColor(i+2, 0);
            }
            else if(i==NUMPIXELS/2-4)
            {
              strip.setPixelColor(i+1, 0);
              strip.setPixelColor(i+2, 0);
              strip.setPixelColor(i+3, 0);
            }
            else if(i==NUMPIXELS/2-5)
            {
              strip.setPixelColor(i+1, 0);
              strip.setPixelColor(i+2, 0);
              strip.setPixelColor(i+3, 0);
              strip.setPixelColor(i+4, 0);
            }
            else if(i==NUMPIXELS/2-6)
            {
              strip.setPixelColor(i+1, 0);
              strip.setPixelColor(i+2, 0);
              strip.setPixelColor(i+3, 0);
              strip.setPixelColor(i+4, 0);
              strip.setPixelColor(i+5, 0);
            }
            else
            {
              strip.setPixelColor(i+1, 0);
              strip.setPixelColor(i+2, 0);
              strip.setPixelColor(i+3, 0);
              strip.setPixelColor(i+4, 0);
              strip.setPixelColor(i+5, 0);
              strip.setPixelColor(i+6, 0);
            }
        }    
      else if(i > tick-1 && i % 10 == nextState)
      {
          strip.setPixelColor(i, color2);
          if(i==NUMPIXELS/2)
            {
              
            }
            else if(i==NUMPIXELS/2+1)
            {
              strip.setPixelColor(i-1, 0);
            }
            else if(i==NUMPIXELS/2+2)
            {
              strip.setPixelColor(i-1, 0);
              strip.setPixelColor(i-2, 0);
            }
            else if(i==NUMPIXELS/2+3)
            {
              strip.setPixelColor(i-1, 0);
              strip.setPixelColor(i-2, 0);
              strip.setPixelColor(i-3, 0);
            }
            else if(i==NUMPIXELS/2+4)
            {
              strip.setPixelColor(i-1, 0);
              strip.setPixelColor(i-2, 0);
              strip.setPixelColor(i-3, 0);
              strip.setPixelColor(i-4, 0);
            }
            else if(i==NUMPIXELS/2+5)
            {
              strip.setPixelColor(i-1, 0);
              strip.setPixelColor(i-2, 0);
              strip.setPixelColor(i-3, 0);
              strip.setPixelColor(i-4, 0);
              strip.setPixelColor(i-5, 0);
            }
            else
            {
              strip.setPixelColor(i-1, 0);
              strip.setPixelColor(i-2, 0);
              strip.setPixelColor(i-3, 0);
              strip.setPixelColor(i-4, 0);
              strip.setPixelColor(i-5, 0);
              strip.setPixelColor(i-6, 0);
            }
    }
    }
    scrollState = scrollState + (forward ? 1 : -1);
    if(forward && scrollState > 9)
      scrollState = 0;
    else if(!forward && scrollState < 0)
       scrollState = 9; 
}
void sideScrollStripColor(uint32_t color, boolean forward)
{
  delayTick = delayTick + 1;
  if(delayTick < delayLength)
    return;
  delayTick = 0;
  short nextState = scrollState + (forward ? 1 : -1);
  if(forward && nextState > 9)
      nextState = 0;
  else if(!forward && nextState < 0)
      nextState = 9;
      
  for(short i = 0; i < NUMPIXELS; i++)
  {
      strip.setPixelColor(i, 0);
      if(i < 20 && i % 10 == 9 - nextState)
          strip.setPixelColor(i, color);
      else if(i > 31 && i % 10 == nextState)
          strip.setPixelColor(i, color);
  //    else if(i >= 20 && i <= 31)
  //        strip.setPixelColor(i, color);
   }

    scrollState = scrollState + (forward ? 1 : -1);
    if(forward && scrollState > 9)
      scrollState = 0;
    else if(!forward && scrollState < 0)
      scrollState = 9;
}
void blinkColor2(uint32_t color, uint32_t color2)
{
  if((delayTick = delayTick + 1) < delayLength)
    return;
  delayTick = 0;
   if((scrollState = scrollState + 1) > 1)
     scrollState = 0;
   for(short i = 0; i < NUMPIXELS; i++)
   {
      if(i % 2 == 1 - scrollState)
        strip.setPixelColor(i, color);
      else
        strip.setPixelColor(i, color2);
    }
}
void strobe(uint32_t color)
{
  if((delayTick = delayTick + 1) < delayLength)
    return;
  delayTick = 0;
   if((scrollState = scrollState + 1) > 1)
     scrollState = 0;
  for(short i = 0; i< NUMPIXELS; i++)
  {
    if(i%2 == 1 - scrollState)
        setStripColor(color);
    else
        setStripColor(0x000000);       
  }
}
