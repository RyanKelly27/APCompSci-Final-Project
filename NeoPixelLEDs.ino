#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 60
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
boolean randomUsed = false;
short scrollState = 0;
uint32_t rainbowColors[7] = {0xCC99A2, 0x800080, 0x00DDDD, 0x0000FF, 0x00FF00, 0xFD7F00, 0xFF0000};
uint32_t ussrColors[2] = {0xFFFF00, 0xFF0000}; 
uint32_t funColors[8] = {0xFF0000, 0xCc44CC, 0x2f4286, 0x336699, 0x996633, 0x66bb99, 0xd4b200, 0x448822};

short delayTick = 0;
short delayLength = 0;
short scrollStates = 0;
int testChanges = 10;
int spectrumCounter = 0;
int delayBlink = 200;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  strip.setBrightness(255);
  if(testChanges == 0)
    setEntireStripColor(0xB118C2);
  else if(testChanges == 1) 
    setPartialStrip(42,18,0x0000FF);
  else if(testChanges == 2)
  {
    if(randomUsed == false)
    {
      randomColors();
      randomUsed = true;
    }
  }   
  else if(testChanges == 3)
    entireSpectrum(10);
  else if(testChanges == 4)
    wave(funColors, 8, 7, true, 0, 60, 100);
  else if(testChanges == 5)
    wave(rainbowColors, 7, 6, true, 0, 60, 100);
  else if(testChanges == 6)
    entireSpectrumWave(40);
  else if(testChanges == 7)
    singleSpectrum(25, 16);
  else if(testChanges == 8)
    blinkLED(0xFF00AA, 16, 15);
  else if(testChanges == 9)
    blinkLEDsIncreasingFast(0xFF0000);
  else if(testChanges == 10)
    rainbow(30);
  else if(testChanges == 11)
    sineWave(40);
    
  strip.show();
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
void wave(uint32_t colors[], int arrayLength, int groupLength, boolean forward, int start, int goTo, int speedOfWave)
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
  delay(speedOfWave);
}
void entireSpectrum(int speedOfCycle)
{
  int rgbColor[3];
  rgbColor[0] = 255;
  rgbColor[1] = 0;
  rgbColor[2] = 0;  
  for (int decColor = 0; decColor < 3; decColor += 1) {
    int incColor = decColor == 2 ? 0 : decColor + 1;
    for(int i = 0; i < 255; i += 1) {
      rgbColor[decColor] -= 1;
      rgbColor[incColor] += 1;
      setEntireStrip(rgbColor[0], rgbColor[1], rgbColor[2]);
      strip.show();
      delay(speedOfCycle);
    }
  }
}
void entireSpectrumWave(int speedOfCycle)
{
  int rgbColor[3];
  rgbColor[0] = 255;
  rgbColor[1] = 0;
  rgbColor[2] = 0;  
  for (int decColor = 0; decColor < 3; decColor += 1) {
    int incColor = decColor == 2 ? 0 : decColor + 1;
    for(int i = 0; i < 255; i += 1) {
      rgbColor[decColor] -= 1;
      rgbColor[incColor] += 1;
      if(spectrumCounter >= 60)
        spectrumCounter = 0;
      strip.setPixelColor(spectrumCounter, rgbColor[0], rgbColor[1], rgbColor[2]);
      strip.show();
      delay(speedOfCycle);
      spectrumCounter++;
    }
  }
}
void singleSpectrum(int speedOfCycle, int pixelLocation)
{
  int rgbColor[3];
  rgbColor[0] = 255;
  rgbColor[1] = 0;
  rgbColor[2] = 0;  
  for (int decColor = 0; decColor < 3; decColor += 1) 
  {
    int incColor = decColor == 2 ? 0 : decColor + 1;
    for(int i = 0; i < 255; i += 1) 
    {
      rgbColor[decColor] -= 1;
      rgbColor[incColor] += 1;
      strip.setPixelColor(pixelLocation, rgbColor[0], rgbColor[1], rgbColor[2]);
      strip.show();
      delay(speedOfCycle);
    }
  }
}
void blinkLEDsIncreasingFast(uint32_t color)
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(delayBlink);
  for(int i=0; i<NUMPIXELS; i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  delay(delayBlink);
  delayBlink = delayBlink - 7;
}
void blinkLED(uint32_t color, int num ,int speedOfCycle)
{
  for(int i=0; i<NUMPIXELS; i++)
  {
  strip.setPixelColor(i, color);
  
  }
  strip.show();
  delay(speedOfCycle);
  for(int i=0; i<NUMPIXELS; i++)
  {
  strip.setPixelColor(i, 0, 0, 0);
 
  }
   strip.show();
  delay(speedOfCycle);
}
void rainbow(uint8_t wait) {//for the 2nd mode this is from the neopixel libray example
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {//for the 2nd mode this is from the neopixel libray example
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
double j = 0;
void sineWave(int delayTime)
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    strip.setPixelColor(i, 255*sin(j), 255*sin(j+85), 255*sin(j+170));
  }
  j= j + .1;
  delay(delayTime);
}

