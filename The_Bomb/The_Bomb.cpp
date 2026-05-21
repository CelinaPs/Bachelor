// This code is all AI generated
// Link to the AI conversation: https://claude.ai/share/9a476e6d-5ad3-4a70-acda-8ff167daf167 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // change 0x27 if needed

#define BUTTON_PIN 7

int countdown = 60;
unsigned long lastTick = 0;
bool stopped = false;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("  DEFUSE BOMB!  ");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(countdown);
  lcd.print("s  ");
}

void loop() {
  if (!stopped && digitalRead(BUTTON_PIN) == LOW) {
    stopped = true;
    lcd.setCursor(0, 0);
    lcd.print("   DEFUSED!!    ");
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(countdown);
    lcd.print("s ");
  }

  if (!stopped && millis() - lastTick >= 1000) {
    lastTick = millis();
    countdown--;

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(countdown);
    lcd.print("s  ");

    if (countdown <= 0) {
      stopped = true;
      lcd.setCursor(0, 0);
      lcd.print("  BOOM!! GAME   ");
      lcd.setCursor(0, 1);
      lcd.print("    OVER!!!     ");
    }
  }
}