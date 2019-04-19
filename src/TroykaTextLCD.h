#ifndef __TROYKA_TEXT_LCD_H__
#define __TROYKA_TEXT_LCD_H__

#include <Arduino.h>
#include <stdint.h>
#include <Print.h>
#include <Wire.h>

#define DEFAULT_LCD_I2C_ADDR    0x3E
#define DEFAULT_PIN_BACKLIGHT   7

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode (LCD_ENTRYMODESET)
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control (LCD_DISPLAYCONTROL)
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift (LCD_CURSORSHIFT)
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set (LCD_FUNCTIONSET)
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00
// Normal or Extension instruction select 
#define LCD_NM_INSTRUCTION      0x00
#define LCD_EX_INSTRUCTION      0x01

// bias selection / Internal OSC frequency adjust
#define LCD_EX_SETBIASOSC       0x10
// set ICON RAM address
#define LCD_EX_SETICONRAMADDR   0x40
// power / ICON control / Contrast set (high byte)
#define LCD_EX_POWICONCONTRASTH 0x50
// follower control
#define LCD_EX_FOLLOWERCONTROL  0x60
// contrast set (low byte)
#define LCD_EX_CONTRASTSETL     0x70

// flags for Bias selection
// bias will be 1/4
#define LCD_BIAS_1_4            0x08
// bias will be 1/5
#define LCD_BIAS_1_5            0x00

// flags Power / ICON control / Contrast set(high byte)
// ICON display on
#define LCD_ICON_ON             0x08
// ICON display off
#define LCD_ICON_OFF            0x00
// booster circuit is turn on
#define LCD_BOOST_ON            0x04
// booster circuit is turn off
#define LCD_BOOST_OFF           0x00
#define LCD_OSC_122HZ           0x00
#define LCD_OSC_131HZ           0x01
#define LCD_OSC_144HZ           0x02
#define LCD_OSC_161HZ           0x03
#define LCD_OSC_183HZ           0x04
#define LCD_OSC_221HZ           0x05
#define LCD_OSC_274HZ           0x06
#define LCD_OSC_347HZ           0x07

// flags Follower control
#define LCD_FOLLOWER_ON         0x08
#define LCD_FOLLOWER_OFF        0x00
#define LCD_RAB_1_00            0x00
#define LCD_RAB_1_25            0x01
#define LCD_RAB_1_50            0x02
#define LCD_RAB_1_80            0x03
#define LCD_RAB_2_00            0x04
#define LCD_RAB_2_50            0x05
#define LCD_RAB_3_00            0x06
#define LCD_RAB_3_75            0x07

class TroykaTextLCD : public Print {
public:
    TroykaTextLCD (uint8_t pinBacklight = DEFAULT_PIN_BACKLIGHT, uint8_t i2cAddress = DEFAULT_LCD_I2C_ADDR, TwoWire* wire = &Wire);
    void begin(uint8_t cols = 16, uint8_t rows = 2, uint8_t charSize = 1);
    void setContrast(uint8_t contrast);
    void setBrightness(uint8_t brightness);
    void setIcon(uint8_t address, uint8_t bit);
    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void createChar(uint8_t location, uint8_t charMap[]);
    void setCursor(uint8_t col, uint8_t row);
    virtual size_t write(uint8_t value);
    void command(uint8_t value);

private:
    void setNormalFunction();
    void setExtendFunction();
    void send(uint8_t value, uint8_t mode);
    TwoWire* _wire;
    uint8_t _i2cAddress;
    uint8_t _displayFunction;
    uint8_t _displayControl;
    uint8_t _displayMode;
    uint8_t _initialized;
    uint8_t _rows;
    uint8_t _cols;
    uint8_t _charSize;
    uint8_t _pinBacklight;
};

#endif  // __TROYKA_TEXT_LCD_H__
