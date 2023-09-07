
// Example sketch which shows how to display some patterns
// on a 64x32 LED matrix
//

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another
 
//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;

const char *firstLine = "obicham te";
const char *secondLine = "mnogo, bebi";
const char *thirdLine = "bottom text";


uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
  if(pos < 85) {
    return dma_display->color565(pos * 3, 255 - pos * 3, 0);
  } else if(pos < 170) {
    pos -= 85;
    return dma_display->color565(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return dma_display->color565(0, pos * 3, 255 - pos * 3);
  }
}

void drawText(int colorWheelOffset)
{
  
  // draw text with a rotating colour
  dma_display->setTextSize(1);     // size 1 == 8 pixels high
  dma_display->setTextWrap(false); // Don't wrap at end of line - will do ourselves

  dma_display->setCursor(0, 0);    // start at top left
  uint8_t w = 0;
  for (w=0; w<strlen(firstLine); w++) {
    dma_display->setTextColor(colorWheel((w*32)+colorWheelOffset));
    dma_display->print(firstLine[w]);
  }

  dma_display->println();
  for (w=9; w<20; w++) {
    dma_display->setTextColor(colorWheel((w*32)+colorWheelOffset));
    dma_display->print("=");
  }
  
  dma_display->println();

  dma_display->setTextColor(dma_display->color444(15,15,15));
  dma_display->println(secondLine); // 10 char max
  dma_display->println(thirdLine);
}


void setup() {

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
  dma_display->fillScreen(myWHITE);
  
  // fill the screen with 'black'
  dma_display->fillScreen(dma_display->color444(0, 0, 0));

}

uint8_t wheelval = 0;

void loop() {

    // animate by going through the colour wheel for the first two lines
    drawText(wheelval);
    wheelval +=1;

    delay(20); 

  
}
