#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
// Fixed 8 fields
#define TFT_CS     15  // Pin CS
#define TFT_RST    4   // Pin RST
#define TFT_DC     2   // Pin DC
#define TFT_MOSI   23  // Pin MOSI
#define TFT_SCK    18  // Pin SCK
//#define TFT_LED  8   // Pin pre LED podsvietenie (ak je použitý)

#define BUTTON_UP    14 // Pin pre tlačidlo "up"
#define BUTTON_DOWN  32 // Pin pre tlačidlo "down"
#define BUTTON_LEFT  33 // Pin pre tlačidlo "left"
#define BUTTON_RIGHT 26 // Pin pre tlačidlo "right"
#define BUTTON_ENTER 27 // Pin pre tlačidlo "enter"

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int selectedRect = 0; // Index vybraného obdĺžnika

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);

  SPI.begin(TFT_SCK, -1, TFT_MOSI);  // Inicializuj SPI s definovanými pinmi
  tft.init(240, 320);                // Inicializuj TFT displej s rozlíšením 240x320
  tft.setRotation(2);                // Nastav rotáciu displeja (0-3)
  tft.fillScreen(ST77XX_WHITE);      // Vyplň obrazovku bielou farbou

  drawMenu(); // Vykresli menu s obdĺžnikmi
}

void loop() {
  if (digitalRead(BUTTON_UP) == LOW) {
    moveCursorUp();
    delay(200); // Anti-bounce delay
  } else if (digitalRead(BUTTON_DOWN) == LOW) {
    moveCursorDown();
    delay(200); // Anti-bounce delay
  } else if (digitalRead(BUTTON_LEFT) == LOW) {
    moveCursorLeft();
    delay(200); // Anti-bounce delay
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    moveCursorRight();
    delay(200); // Anti-bounce delay
  } else if (digitalRead(BUTTON_ENTER) == LOW) {
    displaySelected();
    delay(200); // Anti-bounce delay
  }
}

void drawMenu() {
  int rectWidth = TFT_WIDTH / 2;  // Šírka obdĺžnika - 2 obdĺžniky vedľa seba
  int rectHeight = TFT_HEIGHT / 4; // Výška obdĺžnika - 4 riadky obdĺžnikov

  for (int i = 0; i < 8; i++) {
    int xPos = (i % 2) * rectWidth;  // Vypočítaj x-ovú pozíciu obdĺžnika
    int yPos = (i / 2) * rectHeight; // Vypočítaj y-ovú pozíciu obdĺžnika

    if (i == selectedRect) {
      tft.fillRect(xPos, yPos, rectWidth, rectHeight, ST77XX_GREEN); // Vybraný obdĺžnik zelený
    } else {
      tft.fillRect(xPos, yPos, rectWidth, rectHeight, ST77XX_WHITE); // Ostatné biele
      tft.drawRect(xPos, yPos, rectWidth, rectHeight, ST77XX_BLACK); // Čierne okraje
    }

    int textX = xPos + (rectWidth - 6 * 6) / 2; // Pozícia textu
    int textY = yPos + (rectHeight - 8) / 2;

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
  tft.fillScreen(ST77XX_WHITE);
  tft.setCursor(0, TFT_HEIGHT / 2 - 10);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Pressed Enter on " + String(selectedRect + 1));
  delay(2000);
  selectedRect = 0;
  drawMenu();
}
