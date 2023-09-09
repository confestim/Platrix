
// Example sketch which shows how to display some patterns
// on a 64x32 LED matrix
//

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define REFRESH 10              // Seconds until refresh
#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;



int hexToRGB(String hex) {

  // TODO: Fix the colors not being accurate
  // Remove the '#' from the start
  hex.remove(0, 1);

  // Extract and convert the red, green, and blue components
  uint8_t r = strtol(hex.substring(0, 2).c_str(), NULL, 16);
  uint8_t g = strtol(hex.substring(2, 4).c_str(), NULL, 16);
  uint8_t b = strtol(hex.substring(4, 6).c_str(), NULL, 16);

  return dma_display->color565(r, g, b);

}


// Pixel putting
void putPixel(int16_t x, int16_t y, int color) {
  dma_display->drawPixel(x, y, color);
}

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";
String serverName = "https://your-domain.com/board/json/";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}




void setup() {
  Serial.begin(9600);
  initWiFi();
  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );


  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90); //0-255
  dma_display->clearScreen();

  // fill the screen with 'black'
  dma_display->fillScreen(dma_display->color444(0, 0, 0));

}


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void loop() {

  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    String payload = "null";
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverPath = serverName;

      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected, trying to reconnect");
      WiFi.reconnect();
    }
    lastTime = millis();

    if (payload != "null") {
      JSONVar board = JSON.parse(payload);
      if (JSON.typeof(board) == "undefined") {
        Serial.println("Parsing failed!");
        return;
      }
      // Parse JSON
      for (int y = 0; y < PANEL_RES_Y; y++) {
        for (int x = 0; x < PANEL_RES_X; x++) {
          String color = board[String(y)][x];

          int colorize;
          if (color.length() < 1) {
            colorize = dma_display->color444(0, 0, 0);
          }
          else {
            colorize = hexToRGB(color);
          }
          putPixel(x, y, colorize);
        }
      }
    }


  }

  delay(REFRESH * 100);
}
