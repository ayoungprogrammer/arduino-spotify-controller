#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Joystick
#define Neutral 0
#define Press 1
#define Up 2
#define Down 3
#define Right 4
#define Left 5

// Animation scroll 
#define SCROLL 20

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

int16_t x = 0;
unsigned long time = 0;
int lastCmd = -1;

int CheckJoystick()
{
  int joystickState = analogRead(3);
  
  if (joystickState < 50) return Left;
  if (joystickState < 150) return Down;
  if (joystickState < 250) return Press;
  if (joystickState < 500) return Right;
  if (joystickState < 650) return Up;
  return Neutral;
}

void setup() {
  Serial.begin(9600);
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print("Spotify\nController");
  
}

String data = "";
String artist = "";
String song = "";

void printArtist(uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.print(artist);
}

void printSong(uint16_t color) {
  tft.setCursor(x, 50);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.print(song);
}

void loop() {
  
   int curCmd = CheckJoystick();
   if(curCmd != lastCmd){
     Serial.println(curCmd);
     lastCmd = curCmd;
   }
  
  if(Serial.available() > 0){
     
     data = Serial.readString();
     int comma = data.indexOf(',');
     if(comma != -1){
       if(artist != data.substring(0, comma) || song != data.substring(comma+1)){
         tft.fillScreen(ST7735_BLACK);
         x = SCROLL;
         artist = data.substring(0, comma);
         song = data.substring(comma+1);
         printArtist(ST7735_BLUE);
       }
     }
  }
  if(time + 500 < millis()) {
    time = millis();
    printSong(ST7735_BLACK);
    x -= SCROLL;
    if(x < (-12 * (int)song.length() + 128)){
      x = SCROLL;
    }
    printSong(ST7735_BLUE);
  }
}
