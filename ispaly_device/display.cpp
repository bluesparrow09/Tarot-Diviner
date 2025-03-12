#include "display.h"
#include "DFRobot_LcdDisplay.h"

extern DFRobot_Lcd_IIC lcd; // 让 lcd 变量与主程序共享

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#define GIF_WIDTH   150
#define GIF_HEIGHT  150

#define TITLE_COLOR    0xFFFF
#define KEYWORD_COLOR  0xFFD0
#define SYMBOL_COLOR   0x07E0
#define TEXT_COLOR     0x0000
#define BORDER_COLOR   0x8410

#define CARD_WIDTH     280
#define CARD_HEIGHT    200
#define TEXT_SIZE      18

void displayCenteredGIF() {
    lcd.cleanScreen();
    int16_t posX = (SCREEN_WIDTH - GIF_WIDTH) / 2;
    int16_t posY = (SCREEN_HEIGHT - GIF_HEIGHT) / 2;
    lcd.drawGif(posX, posY, "/tarot/loading3.gif", 0);
}

void displayCard1() {
    lcd.cleanScreen();
    lcd.drawIcon(0, 0, "/tarot/card1.png", 256);
}

void displayCardInterpretation() {
    lcd.cleanScreen();
    lcd.drawString(50, 20, "PAGE ♦ PENTACLES", TEXT_SIZE, TITLE_COLOR);
    lcd.drawString(40, 60, "Growth | Learning", TEXT_SIZE, KEYWORD_COLOR);
    lcd.drawString(40, 85, "Prosperity | Practice", TEXT_SIZE, KEYWORD_COLOR);
    lcd.drawString(130, 110, "♀", 32, SYMBOL_COLOR);
    lcd.drawString(40, 140, "Start with confidence.", TEXT_SIZE, TEXT_COLOR);
    lcd.drawString(40, 165, "Every step adds value.", TEXT_SIZE, TEXT_COLOR);
    lcd.drawString(40, 190, "Growth takes time.", TEXT_SIZE, TEXT_COLOR);
}
