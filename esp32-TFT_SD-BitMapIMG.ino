#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SD.h>
#include <FS.h>

// Nastavte piny podľa vašej konfigurácie
#define TFT_CS        15
#define TFT_RST       4
#define TFT_DC        2
#define SD_CS         5

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

File bmpFile;

void setup() {
  Serial.begin(115200);

  // Inicializácia TFT displeja
  tft.init(240, 320);  // Šírka x Výška
  tft.setRotation(2); // Voliteľné, nastavenie orientácie
  tft.invertDisplay(0);

  // Inicializácia SD karty
  if (!SD.begin(SD_CS)) {
    Serial.println("Nepodarilo sa inicializovať SD kartu.");
    while (1);
  }

  Serial.println("SD karta inicializovaná.");

  // Načítanie BMP súboru
  bmpFile = SD.open("/Hello.bmp"); // REPLACE WITH YOUR IMG NAME HERE
  if (!bmpFile) {
    Serial.println("Nepodarilo sa otvoriť súbor Hello.bmp.");
    while (1);
  }

  Serial.println("Súbor otvorený, načítavanie obrázku...");
  
  // Zobrazenie obrázku
  drawBitmapFromFile("/Hello.bmp", 0, 0);
  
  bmpFile.close();
}

void loop() {
  // Hlavný cyklus môže byť prázdny, ak nechcete robiť ďalšie akcie
}

// Funkcia na načítanie BMP obrázku zo súboru
void drawBitmapFromFile(const char *filename, int x, int y) {
  if (!bmpFile) {
    Serial.println("Chyba: Súbor nie je otvorený.");
    return;
  }

  // Preskočíme hlavičku BMP súboru
  bmpFile.seek(18); // Skok na riadok s rozmermi obrázku
  uint32_t w = bmpFile.read(); w |= bmpFile.read() << 8; w |= bmpFile.read() << 16; w |= bmpFile.read() << 24;
  uint32_t h = bmpFile.read(); h |= bmpFile.read() << 8; h |= bmpFile.read() << 16; h |= bmpFile.read() << 24;

  bmpFile.seek(28); // Skok na riadok s počtom bitov na pixel
  uint16_t bpp = bmpFile.read(); bpp |= bmpFile.read() << 8;
  if (bpp != 24) {
    Serial.println("Nepodporovaný formát BMP, len 24-bit.");
    return;
  }

  bmpFile.seek(54); // Skok na riadok s dátami obrázku

  // Zobrazenie obrázku
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();
      tft.drawPixel(x + col, y + h - row - 1, tft.color565(r, g, b));
    }
  }
}
