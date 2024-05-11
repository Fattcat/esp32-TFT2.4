#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// WEB PRE LEPSIE FARBY KODOV ktore sa definuju : https://rgbcolorpicker.com/565
// napriklad     : #define biela 0xFFFF
// alebo fialova : #define fialova 0x8239
#define fialova 0x8239
// DEF OF TFT PINS for --> "ESP32 Board"
#define TFT_CS    15
#define TFT_DC     2
#define TFT_RST    4
#define TFT_SCK   18
#define TFT_MISO  19
#define TFT_MOSI  23

#define SD_CS     5   // Vyberte volný GPIO pin pro SD_CS

//RESOLUTION OF TFT DISPLAY 2.4" 240x320px
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Initialize TFT
  tft.init(TFT_WIDTH, TFT_HEIGHT);
  tft.invertDisplay(0);
  
  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  
  // Read and display content of datalog.txt
  displayFileContent("/datalog.txt");
}

void loop() {
  // Nothing to do here for this example
}

void displayFileContent(const char* filename) {
  // Open file for reading
  File file = SD.open(filename);
  
  // If the file opened successfully, read and display its content
  if (file) {
    tft.fillScreen(ST77XX_BLACK); // Clear the screen
    tft.setCursor(0, 0); // Set cursor to top-left corner

    tft.setTextColor(ST77XX_GREEN, fialova); // tu je ta fialova
    
    // Read and print each line of the file
    while (file.available()) {
      String line = file.readStringUntil('\n');
      tft.println(line);
    }

    // Close the file
    file.close();
  } else {
    Serial.println("Failed to open file.");
  }
}
