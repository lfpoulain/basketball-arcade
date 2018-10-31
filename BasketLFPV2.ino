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


  animLed(leds, 4, 250);
  


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
    // there is a probleme here
    // if you keep scoring,
    // the time of the led animation add up with the total time.
    // if you score 10 points you add (6*2*50ms from led animation ) * 10 point = + 6 sec a playable time 
    // maybe try something with millis() instead of 3000 delay of 10ms
    // too late for me ;)
    // and i can't test the code :'(
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

      if (in_hoop && !isBallInHoop()) {
        in_hoop = false;
      }

      if (!in_hoop && isBallInHoop()) {
        in_hoop = true;
        baskets++;
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("BUT !!!!");

        animLed(leds, 6, 50);
        

        if (highscore < baskets) {
          EEPROM.write(valuescore, baskets);
        }

      }
      delay(10);
    }

    highscore = EEPROM.read(valuescore);

    animLed(leds, 4, 100);
    

    lcd.clear();

    if (highscore > baskets) {
      lcd.print("Score Max :");
      lcd.setCursor(13, 0);
      lcd.print(highscore);
      lcd.setCursor(0, 1);
      lcd.print("Score :");
      lcd.setCursor(8, 1);
      lcd.print(baskets);

    }

    else {
      lcd.setCursor(0, 0);
      lcd.print("Nouveau record !");
      lcd.setCursor(0, 1);
      lcd.print("Score :");
      lcd.setCursor(8, 1);
      lcd.print(baskets);
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
  boolean ret = true
  if (analogRead(pinphoto) <= 600) {

    ret = false;
  }

  return ret; 
}


void animLed(int pinled, int nb, int timing){
  for (int i = 0; i < nb; ++i)
    {
      digitalWrite(pinled, HIGH);
      delay(timing);
      digitalWrite(pinled, LOW);
      delay(timing);
    }
}