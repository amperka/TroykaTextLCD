#ifndef TROYKA_TEXT_LCD_H
#define TROYKA_TEXT_LCD_H

#include <Arduino.h>
#include <Print.h>
#include <Wire.h>
#include <stdint.h>

constexpr uint8_t DEFAULT_LCD_I2C_ADDR = 0x3E;
constexpr uint8_t DEFAULT_PIN_BACKLIGHT = 7;

// commands
constexpr uint8_t LCD_CLEARDISPLAY = 0x01;
constexpr uint8_t LCD_RETURNHOME = 0x02;
constexpr uint8_t LCD_ENTRYMODESET = 0x04;
constexpr uint8_t LCD_DISPLAYCONTROL = 0x08;
constexpr uint8_t LCD_CURSORSHIFT = 0x10;
constexpr uint8_t LCD_FUNCTIONSET = 0x20;
constexpr uint8_t LCD_SETCGRAMADDR = 0x40;
constexpr uint8_t LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode (LCD_ENTRYMODESET)
constexpr uint8_t LCD_ENTRYRIGHT = 0x00;
constexpr uint8_t LCD_ENTRYLEFT = 0x02;
constexpr uint8_t LCD_ENTRYSHIFTINCREMENT = 0x01;
constexpr uint8_t LCD_ENTRYSHIFTDECREMENT = 0x00;

// flags for display on/off control (LCD_DISPLAYCONTROL)
constexpr uint8_t LCD_DISPLAYON = 0x04;
constexpr uint8_t LCD_DISPLAYOFF = 0x00;
constexpr uint8_t LCD_CURSORON = 0x02;
constexpr uint8_t LCD_CURSOROFF = 0x00;
constexpr uint8_t LCD_BLINKON = 0x01;
constexpr uint8_t LCD_BLINKOFF = 0x00;

// flags for display/cursor shift (LCD_CURSORSHIFT)
constexpr uint8_t LCD_DISPLAYMOVE = 0x08;
constexpr uint8_t LCD_CURSORMOVE = 0x00;
constexpr uint8_t LCD_MOVERIGHT = 0x04;
constexpr uint8_t LCD_MOVELEFT = 0x00;

// flags for function set (LCD_FUNCTIONSET)
constexpr uint8_t LCD_8BITMODE = 0x10;
constexpr uint8_t LCD_4BITMODE = 0x00;
constexpr uint8_t LCD_2LINE = 0x08;
constexpr uint8_t LCD_1LINE = 0x00;
constexpr uint8_t LCD_5x10DOTS = 0x04;
constexpr uint8_t LCD_5x8DOTS = 0x00;
// Normal or Extension instruction select
constexpr uint8_t LCD_NM_INSTRUCTION = 0x00;
constexpr uint8_t LCD_EX_INSTRUCTION = 0x01;

// bias selection / Internal OSC frequency adjust
constexpr uint8_t LCD_EX_SETBIASOSC = 0x10;
// set ICON RAM address
constexpr uint8_t LCD_EX_SETICONRAMADDR = 0x40;
// power / ICON control / Contrast set (high byte)
constexpr uint8_t LCD_EX_POWICONCONTRASTH = 0x50;
// follower control
constexpr uint8_t LCD_EX_FOLLOWERCONTROL = 0x60;
// contrast set (low byte)
constexpr uint8_t LCD_EX_CONTRASTSETL = 0x70;

// flags for Bias selection
// bias will be 1/4
constexpr uint8_t LCD_BIAS_1_4 = 0x08;
// bias will be 1/5
constexpr uint8_t LCD_BIAS_1_5 = 0x00;

// flags Power / ICON control / Contrast set(high byte)
// ICON display on
constexpr uint8_t LCD_ICON_ON = 0x08;
// ICON display off
constexpr uint8_t LCD_ICON_OFF = 0x00;
// booster circuit is turn on
constexpr uint8_t LCD_BOOST_ON = 0x04;
// booster circuit is turn off
constexpr uint8_t LCD_BOOST_OFF = 0x00;

// frame frequency
enum LCD_OSC {
  F_122HZ = 0x00,
  F_131HZ = 0x01,
  F_144HZ = 0x02,
  F_161HZ = 0x03,
  F_183HZ = 0x04,
  F_221HZ = 0x05,
  F_274HZ = 0x06,
  F_347HZ = 0x07
};

// flags Follower control
constexpr uint8_t LCD_FOLLOWER_ON = 0x08;
constexpr uint8_t LCD_FOLLOWER_OFF = 0x00;

// generator amplified ratio
enum LCD_RAB {
  V0_1_00V = 0x00,
  V0_1_25V = 0x01,
  V0_1_50V = 0x02,
  V0_1_80V = 0x03,
  V0_2_00V = 0x04,
  V0_2_50V = 0x05,
  V0_3_00V = 0x06,
  V0_3_75V = 0x07
};

class TroykaTextLCD : public Print {
public:
  TroykaTextLCD(TwoWire *wire = &Wire,
                uint8_t i2cAddress = DEFAULT_LCD_I2C_ADDR,
                uint8_t pinBacklight = DEFAULT_PIN_BACKLIGHT);
  void begin(uint8_t cols = 16, uint8_t rows = 2, uint8_t charSize = 1);
  void setContrast(uint8_t contrast);
  void setBrightness(uint16_t brightness);
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
  size_t write(uint8_t value) override;
  void command(uint8_t value);

private:
  void setNormalFunction();
  void setExtendFunction();
  void send(uint8_t value, uint8_t mode);
  TwoWire *_wire;
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

#endif // TROYKA_TEXT_LCD_H
