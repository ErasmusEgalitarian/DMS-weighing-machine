#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <BluetoothSerial.h> // Biblioteca Bluetooth

LiquidCrystal_I2C lcd(0x27, 16, 2); // Definição do display
const int LOADCELL_DOUT_PIN = 16; // HX711 pin 1
const int LOADCELL_SCK_PIN = 4;  // HX711 pin 2

HX711 scale;
BluetoothSerial SerialBT; // Objeto para comunicação Bluetooth

String wastepicker = ""; // Nome do catador

void setup() {
  Serial.begin(115200); // Comunicação serial
  SerialBT.begin("BalancaESP32"); // Nome do dispositivo Bluetooth

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // Configuração da balança
  scale.set_scale(6.12);      // Fator de correção
  scale.tare();               // Tare

  lcd.init();                 // Inicialização do display
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando BT...");

  // Aguarda conexão Bluetooth
  while (!SerialBT.hasClient()) {
    delay(100);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado!");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Nome do catador:");
  while (wastepicker == "") {
    if (SerialBT.available()) {
      wastepicker = SerialBT.readString(); // Recebe nome do catador
      wastepicker.trim(); // Remove espaços extras
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Catador: ");
      lcd.setCursor(0, 1);
      lcd.print(wastepicker);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Coloque o peso!");
    }
  }
}

void loop() {
  float weightg = scale.get_units(10);
  float weightkg = weightg / 1000;

  lcd.setCursor(0, 1);
  lcd.print(weightkg, 2);
  lcd.print(" Kg   ");

  if (SerialBT.available()) {
    String command = SerialBT.readString();
    command.trim();

    if (command == "GET_WEIGHT") {
      SerialBT.print("Catador: ");
      SerialBT.print(wastepicker);
      SerialBT.print(" | Peso: ");
      SerialBT.print(weightkg, 2);
      SerialBT.println(" Kg");
    }
  }

  scale.power_down(); // Reset da balança
  delay(5000);
  scale.power_up();
}
