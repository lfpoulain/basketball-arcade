#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int highscore = 0;
int valuescore;
int leds = 4;
int pinphoto = 0;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.cursor_off();
  lcd.blink_off();
  lcd.clear();

  pinMode(leds, OUTPUT);
  for(int i = 0; i < 4; ++i)
  {
    delay(250);
    digitalWrite(leds, LOW);
    delay(250);
    digitalWrite(leds, HIGH);
  }
}

void loop() {

  uint16_t baskets = 0;
  uint16_t print_time = 0;
  boolean in_hoop = false;
  boolean start_game = false;
  lcd.setCursor(0, 0);
  lcd.print("BasketBall LFP");
  lcd.setCursor(0, 1);
  lcd.print("Lance la balle !");

  // Demarrer le jeu lorsque la balle passe
  if (isBallInHoop()) {
    start_game = true;
  }

  if (start_game) {

    // Temps = 1000 * 30s
    for (uint16_t time = 30000; time > 0; time -= 10) {
      // if time == an even second update matrix
      if (time % 1000 == 0) {
        print_time = time / 1000;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temps:");
        lcd.setCursor(8, 0);
        lcd.print(print_time);
        lcd.setCursor(11, 0);
        lcd.print("sec");
        lcd.setCursor(0, 1);
        lcd.print("Balle :");
        lcd.setCursor(8, 1);
        lcd.print(baskets);
        lcd.cursor_off();
        digitalWrite(leds, LOW);

      }

      if (in_hoop) {
        if (!isBallInHoop()) {
          in_hoop = false;
        }
      }

      if (!in_hoop && isBallInHoop()) {
        in_hoop = true;
        baskets++;
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("BUT !!!!");
        flashLEDs(6, 50);

         if (highscore < baskets) {
        EEPROM.write(valuescore, baskets);
      }

      }

     
      delay(10);
    }

    highscore = EEPROM.read(valuescore);
    flashLEDs(4, 100);
    lcd.clear();

    if (highscore > baskets) {
      lcd.print("Score Max :");
      lcd.setCursor(13, 0);
      lcd.print(highscore);
      lcd.setCursor(0, 1);
      printScore();
    }

    else {
      lcd.setCursor(0, 0);
      lcd.print("Nouveau record !");
      lcd.setCursor(0, 1);
      printScore();
    }
    
    delay(7000);
    lcd.clear();
    
    }
    delay(100);
  }

  ///////////////////////////////////////////////////////
  // 
  // Detection du passage de la balle 
  // 
  ///////////////////////////////////////////////////////
  
  boolean isBallInHoop() {
    if (analogRead(pinphoto) <= 600)
    {
      return false;
    }
    return true; 
  }

  void flashLEDs(int nbTime, int delay)
  {
    for(int i = 0; i < nbTime: ++i)
    {
      digitalWrite(leds, HIGH);
      delay(delay);
      digitalWrite(leds, LOW);
      delay(delay);
    }
  }

  void printScore()
  {
    lcd.print("Score :");
    lcd.setCursor(8, 1);
    lcd.print(baskets);
  }
