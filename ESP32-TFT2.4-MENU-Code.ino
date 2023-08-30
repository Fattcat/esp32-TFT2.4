#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>

#define TFT_CS     15
#define TFT_DC     2
#define TFT_RST    4

// Farby
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define ORANGE 0xFD20

#define TFT_MAD_BGR 0x00

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int buttonUpPin = 14;    // Tlačidlo pre smer hore
const int buttonDownPin = 32;  // Tlačidlo pre smer dole
const int buttonEnterPin = 33; // Tlačidlo pre ENTER

int selectedOption = 1;
String menuOptions[] = {"Start", "Options", "Beacon", "WiFi Jammer", "EXIT"}; // Zmenené poradie
int numMenuOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

int textHeight = 24; // Adjust this based on your font size
int rowFillHeight = 18; // Adjust this for the row fill height

bool enterPressed = false; // Indikátor, či bolo stlačené ENTER

void setup() {
  Serial.begin(9600);
  tft.invertDisplay(true);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonEnterPin, INPUT_PULLUP);

  tft.init(240, 320); // Inicializácia displeja s rozlíšením 240x320
  tft.setRotation(0); // Nastavenie rotácie displeja
  tft.fillScreen(WHITE); // Vyplnenie displeja čiernou farbou

  updateMenu();
}

void loop() {
  if (digitalRead(buttonUpPin) == LOW) {
    moveSelection(-1);
    delay(300);
  }

  if (digitalRead(buttonDownPin) == LOW) {
    moveSelection(1);
    delay(300);
  }

  if (digitalRead(buttonEnterPin) == LOW && !enterPressed) {
    enterPressed = true;
    clearScreen();
    if (selectedOption == numMenuOptions) {
      tft.fillScreen(WHITE);
      delay(1000);
    } else if (selectedOption == 4) { // Ak je vybratá možnosť WiFi Jammer
      tft.setCursor(0, 0);
      tft.setTextColor(BLACK);
      tft.setTextSize(2);
      tft.println("WiFi name: " + WiFi.SSID());
      tft.println("MAC Address: " + WiFi.macAddress());
      tft.println("CHANNEL: " + String(WiFi.channel()));
      tft.println("Security Type: " + getSecurityType(WiFi.encryptionType(0)));
      delay(2000);
      updateMenu();
    }
  } else if (digitalRead(buttonEnterPin) == HIGH) {
    enterPressed = false;
  }
}

void moveSelection(int dir) {
  selectedOption += dir;
  if (selectedOption < 1) {
    selectedOption = numMenuOptions;
  } else if (selectedOption > numMenuOptions) {
    selectedOption = 1;
  }
  updateMenu();
}

void updateMenu() {
  tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);

  for (int i = 0; i < numMenuOptions; i++) {
    int centerY = 60 + (i - (numMenuOptions - 1) / 2.0) * textHeight; // Zmena vertikálneho zarovnania

    if (i + 1 == selectedOption) {
      tft.setTextColor(RED);
      tft.fillRect(0, centerY - rowFillHeight / 2 - 6, 240, rowFillHeight, ORANGE); // Zmena pozície obdĺžnika
    } else {
      tft.setTextColor(BLACK);
    }

    int xOffset = 0;
    if (i == 0) {
      xOffset = 10;
    }

    tft.setCursor((240 - menuOptions[i].length() * 12) / 2 - xOffset, centerY - textHeight / 2); // Zmena horizontálneho zarovnania
    tft.println(i + 1 + ". " + menuOptions[i]);
  }
}

void clearScreen() {
  tft.fillScreen(BLACK);
}

String getSecurityType(uint8_t encryptionType) {
  switch (encryptionType) {
    case WIFI_AUTH_OPEN:
      return "Open";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA_WPA2";
    default:
      return "Unknown";
  }
}
