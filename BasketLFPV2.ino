#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define slave_addr 0x27

LiquidCrystal_I2C lcd(slave_addr, 16, 2);
int highscore = 0,valuescore;

int const leds{4}, pinphoto{0};

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.cursor_off();
  lcd.blink_off();
  lcd.clear();

  pinMode(leds, OUTPUT);
  delayMicroseconds(50);
  
  digitalWrite(leds, LOW);
  delay(250);

  flash_led<leds,7>(4.0);
}

void loop() 
{
  uint16_t baskets {0}, print_time{0};
  bool in_hoop {false}, start_game {false};
  
  lcd.setCursor(0, 0);
  lcd.print("BasketBall LFP");
  lcd.setCursor(0, 1);
  lcd.print("Lance la balle !");

  // Demarrer le jeu lorsque la balle passe
  
  start_game= isBallInHoop<pinphoto>() ? true : start_game;

  if (start_game) {

    // Temps = 1000 * 30s
    for (uint16_t time = 30000; time > 0; time -= 10)
    {
      // if time == an even second update matrix
      if (time % 1000 == 0) 
      {
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


      in_hoop=in_hoop && !isBallInHoop<pinphoto>() ? false : in_hoop;


      if (!in_hoop && isBallInHoop<pinphoto>()) 
      {
        in_hoop = true;
        baskets++;
        
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("BUT !!!!");

        flash_led<leds,10>(20.0);

        if (highscore < baskets) 
          EEPROM.write(valuescore, baskets);
      }

     
      delay(10);
    }

    highscore = EEPROM.read(valuescore);
    
    flash_led<leds,8>(10.0);

    lcd.clear();

    if (highscore > baskets) 
    {
      lcd.print("Score Max :");
      lcd.setCursor(13, 0);
      lcd.print(highscore);
      lcd.setCursor(0, 1);
      lcd.print("Score :");
      lcd.setCursor(8, 1);
      lcd.print(baskets);

    }
    else 
    {
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
  
template<int _pin> inline bool isBallInHoop(void) 
{
    return analogRead(_pin) <= 600 ? false : true ;
}

template<int _pin,long n_puls> void flash_led(float const & freq)
{
  uint16_t inter=1000000.0/freq;
  
  digitalWrite(_pin, HIGH);
  
  for(auto i=1u; i < n_puls ; i++)
  {
    delayMicroseconds(inter);
    digitalWrite(_pin, i%2 );
  }
}


