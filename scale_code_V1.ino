#include <LiquidCrystal_I2C.h>
#include <HX711.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //display definition
const int LOADCELL_DOUT_PIN = 16; //hx711 pin 1
const int LOADCELL_SCK_PIN = 4; //hx711 pin 2

HX711 scale;

void setup() {
  Serial.begin(115200); //serial communication if needed
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); //scale setup
  scale.set_scale(6.12);      // correction factor
  scale.tare();               // tare
  lcd.init();                 //display initiation
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Coloque o peso!");
  delay(5000);
  //lcd.setCursor(0, 0); 
  //lcd.print(material);      //will add in future
}

void loop() {
  float weightg = scale.get_units(10);
  float weightkg = weightg/1000;
  lcd.setCursor(0, 2);
  lcd.print(weightkg)
  lcd.print("Kg")
  scale.power_down();             // scale reset
  delay(50000);
  scale.power_up();
}
