// библиотека для работы с дисплеем
#include <TroykaTextLCD.h>

// создаем объект для работы с дисплеем
TroykaTextLCD lcd;

void setup() {
  // открываем монитор Serial-порта
  Serial.begin(9600);
  // устанавливаем количество столбцов и строк экрана
  lcd.begin(16, 2);
  // устанавливаем контрастность в диапазоне от 0 до 63
  lcd.setContrast(27);
  // устанавливаем яркость в диапазоне от 0 до 255
  lcd.setBrightness(255);

}

void loop() {
  // если пришли данные из Serial-порта
  if (Serial.available()) {
    // очищаем экран
    lcd.clear();
    // ожидаем все данные
    while (Serial.available() > 0) {
      // выводим все пришедшие данные на экран
      lcd.write(Serial.read());
    }
  }
}
