#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// DEF OF TFT PINS for --> "ESP32 Board"
#define TFT_CS    15
#define TFT_DC     2
#define TFT_RST    4
#define TFT_SCK   18
#define TFT_MISO  19
#define TFT_MOSI  23

//RESOLUTION OF TFT DISPLAY 2.4" 240x320px
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  
  // Initialize TFT
  tft.init(TFT_WIDTH, TFT_HEIGHT);
  tft.invertDisplay(0);
  tft.setTextWrap(true);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setCursor(30, 60);
  tft.setTextSize(1);
  tft.print("Napis nejaky prikaz do Serial Monitoru ...");
  tft.print("Dostupné príkazy sú :");
  tft.print("fillScreen(<farba_sem>)");
  tft.print("PRíklad : fillScreen(ST77XX_RED)");
  tft.print("spolu setky su :");
 // Vypísanie textov s požadovanými farbami pozadia
  tft.setCursor(60, 90);
  tft.print("BLACK");
  
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.print(", ");
  
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.print("RED");
  
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.print(", ");
  
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.print("GREEN");
  
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.print(", ");
  
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.print("BLUE");
  
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.print(", ");
  
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("WHITE");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    
    if (command.startsWith("fillScreen")) {
      String color = command.substring(command.indexOf('(') + 1, command.indexOf(')'));
      if (color == "ST77XX_BLACK") {
        tft.fillScreen(ST77XX_BLACK);
      } else if (color == "ST77XX_RED") {
        tft.fillScreen(ST77XX_RED);
      } else if (color == "ST77XX_GREEN") {
        tft.fillScreen(ST77XX_GREEN);
      } else if (color == "ST77XX_BLUE") {
        tft.fillScreen(ST77XX_BLUE);
      } else if (color == "ST77XX_WHITE") {
        tft.fillScreen(ST77XX_WHITE);
      } else {
        Serial.println("Unknown color!");
      }
    }
  }
}
