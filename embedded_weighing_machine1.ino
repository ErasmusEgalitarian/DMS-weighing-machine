#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

// Sensors Location
int sensor1=A5;
int sensor2=A4;
int sensor3=A3;
int sensor4=A2;

// Weight
int weightsensor1=0;
int weightsensor2=0;
int weightsensor3=0;
int weightsensor4=0;

void setup () {
    Serial.begin(9600); // leave this here
  
  	lcd_1.begin(16, 2);
  	lcd_1.print("Hello World!");
}

void loop () {
  weightsensor1=analogRead(sensor1);
  weightsensor2=analogRead(sensor2);
  weightsensor3=analogRead(sensor3);
  weightsensor4=analogRead(sensor4);
  Serial.println(weightsensor4);
  delay(3000);
  lcd_1.clear();
  delay(3000);
  lcd_1.begin(16, 2);
  lcd_1.print("hello again!");
}


int medium(int w1, int w2, int w3, int w4) {
  return 0;
}