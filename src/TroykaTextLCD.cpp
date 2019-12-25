#include "TroykaTextLCD.h"

TroykaTextLCD::TroykaTextLCD(TwoWire *wire, uint8_t i2cAddress,
                             uint8_t pinBacklight) {
  _wire = wire;
  _i2cAddress = i2cAddress;
  _pinBacklight = pinBacklight;
}

void TroykaTextLCD::begin(uint8_t cols, uint8_t rows, uint8_t charSize) {
  _cols = cols;
  _rows = rows;
  _charSize = charSize;
  pinMode(_pinBacklight, OUTPUT);
  setBrightness(255);

  _wire->begin();
  delay(40);

  _displayFunction = LCD_8BITMODE;
  if (_rows == 1) {
    _displayFunction |= LCD_1LINE;
  } else if (_rows == 2) {
    _displayFunction |= LCD_2LINE;
  }
  if (charSize == 1) {
    _displayFunction |= LCD_5x8DOTS;
  } else if ((charSize == 2) && (_rows == 2)) {
    _displayFunction |= LCD_5x10DOTS;
  }
  setNormalFunction();
  setExtendFunction();
  command(LCD_EX_SETBIASOSC | LCD_BIAS_1_5 | LCD_OSC::F_183HZ);
  command(LCD_EX_FOLLOWERCONTROL | LCD_FOLLOWER_ON | LCD_RAB::V0_2_00V);
  delay(200);
  setNormalFunction();

  _displayControl = 0x00;
  _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  command(LCD_DISPLAYCONTROL | _displayControl);

  clear();

  _displayMode = 0x00;
  _displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  command(LCD_ENTRYMODESET | _displayMode);
}

void TroykaTextLCD::setContrast(uint8_t contrast) {
  setExtendFunction();
  command(LCD_EX_CONTRASTSETL | (contrast & 0x0f));
  command(LCD_EX_POWICONCONTRASTH | LCD_ICON_ON | LCD_BOOST_ON |
          ((contrast >> 4) & 0x03));
  setNormalFunction();
}

void TroykaTextLCD::setBrightness(uint16_t brightness) {
// Default PWM resolution 10 bit for board ESP8266
// Default PWM resolution 8 bit other boards
#if defined(ESP8266)
  analogWrite(_pinBacklight, brightness << 2);
#else
  analogWrite(_pinBacklight, brightness);
#endif
}

void TroykaTextLCD::setIcon(uint8_t addr, uint8_t bit) {
  setExtendFunction();
  command(LCD_EX_SETICONRAMADDR | (addr & 0x0f));
  write(bit);
  setNormalFunction();
}

void TroykaTextLCD::clear() {
  command(LCD_CLEARDISPLAY);
  delay(2);
}

void TroykaTextLCD::home() {
  command(LCD_RETURNHOME);
  delay(2);
}

void TroykaTextLCD::setCursor(uint8_t col, uint8_t row) {
  const int rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > _rows) {
    row = _rows - 1;
  }
  command(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}

void TroykaTextLCD::noDisplay() {
  _displayControl &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::display() {
  _displayControl |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::noCursor() {
  _displayControl &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::cursor() {
  _displayControl |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::noBlink() {
  _displayControl &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::blink() {
  _displayControl |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void TroykaTextLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void TroykaTextLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void TroykaTextLCD::leftToRight(void) {
  _displayMode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displayMode);
}

void TroykaTextLCD::rightToLeft(void) {
  _displayMode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displayMode);
}

void TroykaTextLCD::autoscroll(void) {
  _displayMode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displayMode);
}

void TroykaTextLCD::noAutoscroll(void) {
  _displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displayMode);
}

void TroykaTextLCD::createChar(uint8_t location, uint8_t charMap[]) {
  location &= 0x7;
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charMap[i]);
  }
}

void TroykaTextLCD::command(uint8_t value) { send(value, 0x00); }

size_t TroykaTextLCD::write(uint8_t value) {
  send(value, 0x40);
  return 1;
}

void TroykaTextLCD::send(uint8_t value, uint8_t mode) {
  _wire->beginTransmission(_i2cAddress);
  _wire->write(mode);
  _wire->write(value);
  _wire->endTransmission();
  delayMicroseconds(27);
}

void TroykaTextLCD::setNormalFunction() {
  command(LCD_FUNCTIONSET | _displayFunction | LCD_NM_INSTRUCTION);
}

void TroykaTextLCD::setExtendFunction() {
  command(LCD_FUNCTIONSET | _displayFunction | LCD_EX_INSTRUCTION);
}
