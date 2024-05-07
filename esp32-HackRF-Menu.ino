#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS     15 // Definuj pin CS
#define TFT_RST    4 // Definuj pin RST
#define TFT_DC     2 // Definuj pin DC
#define TFT_MOSI   23 // Definuj pin MOSI
#define TFT_SCK    18 // Definuj pin CLK
#define TFT_MISO   19 // Definuj pin MISO
//#define TFT_LED    8 // Definuj pin pre LED podsvietenie

#define BUTTON_UP    14 // Pin pre tlačidlo "up"
#define BUTTON_DOWN  32 // Pin pre tlačidlo "down"
#define BUTTON_LEFT  33 // Pin pre tlačidlo "left"
#define BUTTON_RIGHT 26 // Pin pre tlačidlo "right"
#define BUTTON_ENTER 27 // Pin pre tlačidlo "enter"

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // Inicializuj ST7789 objekt

int selectedRect = 0; // Index vybraného obdĺžnika

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP); // Nastav pin pre tlačidlo "up" ako vstup s pull-up odporom
  pinMode(BUTTON_DOWN, INPUT_PULLUP); // Nastav pin pre tlačidlo "down" ako vstup s pull-up odporom
  pinMode(BUTTON_LEFT, INPUT_PULLUP); // Nastav pin pre tlačidlo "left" ako vstup s pull-up odporom
  pinMode(BUTTON_RIGHT, INPUT_PULLUP); // Nastav pin pre tlačidlo "right" ako vstup s pull-up odporom
  pinMode(BUTTON_ENTER, INPUT_PULLUP); // Nastav pin pre tlačidlo "enter" ako vstup s pull-up odporom

  SPI.begin(); // Inicializuj SPI komunikáciu
  tft.init(TFT_WIDTH, TFT_HEIGHT); // Inicializuj TFT displej s rozlíšením 240x320
  tft.setRotation(2); // Nastav rotáciu displeja (0-3)
  tft.fillScreen(ST77XX_WHITE); // Vypln obrazovku čiernou farbou

  int rectWidth = TFT_WIDTH / 2; // Šírka obdĺžnika
  int rectHeight = TFT_HEIGHT / 8; // Výška obdĺžnika

  drawMenu(); // Vykresli menu s obdĺžnikmi
}

void loop() {
  if (digitalRead(BUTTON_UP) == LOW) {
    moveCursorUp();
  } else if (digitalRead(BUTTON_DOWN) == LOW) {
    moveCursorDown();
  } else if (digitalRead(BUTTON_LEFT) == LOW) {
    moveCursorLeft();
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    moveCursorRight();
  } else if (digitalRead(BUTTON_ENTER) == LOW) {
    displaySelected();
  }
}

void drawMenu() {
  int rectWidth = TFT_WIDTH / 2; // Šírka obdĺžnika
  int rectHeight = TFT_HEIGHT / 8; // Výška obdĺžnika

  for (int i = 0; i < 8; i++) {
    int xPos = (i % 2) * rectWidth; // Vypočítaj x-ovú pozíciu obdĺžnika
    int yPos = (i / 2) * rectHeight; // Vypočítaj y-ovú pozíciu obdĺžnika

    // Nakresli obdĺžnik
    if (i == selectedRect) {
      tft.fillRect(xPos, yPos, rectWidth, rectHeight, ST77XX_GREEN); // Zafarbene vybranej farbou
    } else {
      tft.drawRect(xPos, yPos, rectWidth, rectHeight, ST77XX_WHITE); // Zostavaju ostatne biele
    }
    
    // Vypočítaj pozíciu textu v strede obdĺžnika
    int textX = xPos + (rectWidth - 7 * 6) / 2; // Predpokladajme, že "Test 1" má 7 znakov
    int textY = yPos + (rectHeight - 8) / 2; // Predpokladajme, že veľkosť písma je 8 bodov

    // Vykresli text do obdĺžnika
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(textX, textY);
    tft.print("Test " + String(i + 1));
  }
}

void moveCursorUp() {
  if (selectedRect >= 2) {
    selectedRect -= 2;
    drawMenu();
  }
}

void moveCursorDown() {
  if (selectedRect < 6) {
    selectedRect += 2;
    drawMenu();
  }
}

void moveCursorLeft() {
  if (selectedRect % 2 == 1) {
    selectedRect -= 1;
    drawMenu();
  }
}

void moveCursorRight() {
  if (selectedRect % 2 == 0) {
    selectedRect += 1;
    drawMenu();
  }
}

void displaySelected() {
  tft.fillScreen(ST77XX_WHITE); // Vyčistí displej
  tft.setCursor(0, TFT_HEIGHT / 2 - 10);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Pressed Enter on " + String(selectedRect + 1));
  delay(2000); // Počká 2 sekundy
  selectedRect = 0; // Resetuje vybraný obdĺžnik na prvý
  drawMenu(); // Vykresli menu s obdĺžnikmi
}
