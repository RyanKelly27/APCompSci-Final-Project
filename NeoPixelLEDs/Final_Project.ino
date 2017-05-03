#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 60
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
boolean randomUsed = false;
short scrollState = 0;
uint32_t rainbowColors[7] = {0xCC99A2, 0x800080, 0x00DDDD, 0x0000FF, 0x00FF00, 0xFD7F00, 0xFF0000};
uint32_t ussrColors[2] = {0xFFFF00, 0xFF0000}; 
short delayTick = 0;
short delayLength = 0;
short scrollStates = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  strip.setBrightness(255);
//  setEntireStripColor(0xFF0000);
 // setPartialStrip(42,18,0x0000FF);
  if(randomUsed == false)
  {
//    randomColors();
//    randomUsed = true;
  } 
  //fullScrollStrip(ussrColors, 2, 6, true, 0, 19);
  //fullScrollStrip(ussrColors, 2, 6, true, 20, 39);
  //fullScrollStrip(ussrColors, 2, 6, true, 40, 60);
//  fullScrollStrip(rainbowColors, 7, 6, true, 60, 0);
//  sineWave(50);
  spectrum(50);
  strip.show();
  delay(100);
}

void setEntireStripColor(uint32_t color)
{
  for(int i=0; i<NUMPIXELS; i++)
    strip.setPixelColor(i, color);
}
void setEntireStrip(int red, int green, int blue)
{
  for(int i=0; i<NUMPIXELS; i++)
    strip.setPixelColor(i, red, green, blue);
}
void setPartialStrip(int start, int goTo, uint32_t color)
{
  if(start<0)
    start=0;
  if(goTo>NUMPIXELS)
    goTo = NUMPIXELS;
  if(start > goTo)
  {
    int temp = start;
    start = goTo;
    goTo = temp;
  }
  for(int i=start; i<goTo; i++)
    strip.setPixelColor(i, color);
}
void randomColors()
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    int rColor = random(0,256) + 3;
    int gColor = random(0, 256) + 3;
    int bColor = random(0,256) + 3;
    rColor = (int) ((pow((float) rColor / 255.0, 3.0)) * (float) 255);
    gColor = (int) ((pow((float) gColor / 255.0, 3.0)) * (float) 255);
    bColor = (int) ((pow((float) bColor / 255.0, 3.0)) * (float) 255);
    if(rColor > 255)
      rColor = 255;
    if(gColor > 255)
      gColor = 255;
    if(bColor > 255)
      bColor = 255;
    strip.setPixelColor(i, rColor, gColor, bColor);
  }
  delay(100);
}

void fullScrollStrip(uint32_t colors[], int arrayLength, int groupLength, boolean forward, int start, int goTo)
{
  if(start<0)
    start=0;
  if(goTo>NUMPIXELS)
    goTo = NUMPIXELS;
  if(start > goTo)
  {
    int temp = start;
    start = goTo;
    goTo = temp;
  }
  if((delayTick = delayTick + 1) < delayLength)
    return;
  delayTick = 0;
  boolean flag = false;
  int runningLength = 1;
  
  if(forward == true)
  {
    if((scrollState = scrollState - 1) < 0)
      scrollState = arrayLength - 1;
    for(short i = goTo; i > start; i--)
      strip.setPixelColor(i, strip.getPixelColor(i-1));

    flag = true;
    for(int i = start + 2; i <= groupLength + start; i++)
      if(strip.getPixelColor(start+1) != strip.getPixelColor(i))
        flag = false;
        
    if(flag)
      strip.setPixelColor(start, colors[scrollState]);
    else
      strip.setPixelColor(start, strip.getPixelColor(start+1));
  }
  else if(forward == false)
  {
    if((scrollStates = scrollStates - 1) < 0)
      scrollStates = arrayLength - 1;
    for(short i = start; i < goTo; i++)
      strip.setPixelColor(i, strip.getPixelColor(i+1));

    flag = true;
    for(int i = goTo-3; i > goTo-1-groupLength; i--)
      if(strip.getPixelColor(goTo-2) != strip.getPixelColor(i))
        flag = false;
        
    if(flag)
      strip.setPixelColor(goTo-1, colors[scrollStates]);
    else
      strip.setPixelColor(goTo-1, strip.getPixelColor(goTo-2));
  }
}
void sineWave(int speedOfCycles)
{
  int red;
  int green;
  int blue;
/*  for(int i=0; i<256; i++)
  {
    redColor++;
    greenColor++;
    blueColor++;
    if(redColor >= 255)
      redColor = redColor - 255;
    if(greenColor >= 255)
      greenColor = greenColor - 255;
    if(blueColor >= 255)
      blueColor = blueColor - 255;  
    setEntireStrip(redColor, blueColor, greenColor);
    strip.show();
    delay(speedOfCycles);
  }
 */ 
  for (red = 255; red >= 90; red--) {
    for (green = 90; green <= 255; green++) {
        for (blue = 255; blue >= 90; blue--) {
           setEntireStrip(red, blue, green);
           strip.show();
           delay(speedOfCycles);
        }
    }
}
}
void spectrum(int speedOfCycle)
{
  int rgbColour[3];
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      setEntireStrip(rgbColour[0], rgbColour[1], rgbColour[2]);
      strip.show();
      delay(speedOfCycle);
    }
  }
}

