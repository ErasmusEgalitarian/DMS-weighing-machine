#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Display definition
const int LOADCELL_DOUT_PIN = 16;  // HX711 pin 1
const int LOADCELL_SCK_PIN = 4;   // HX711 pin 2

HX711 scale;
BluetoothSerial SerialBT;

// WiFi Configuration
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
WiFiServer server(80);

String wastepicker = "";  // Variable to store the name of the waste picker
bool isConnected = false; // Bluetooth connection status

void setup() {
  Serial.begin(115200); // Serial communication
  SerialBT.begin("ESP32_Scale"); // Bluetooth device name
  Serial.println("Bluetooth started, waiting for connection...");

  WiFi.begin(ssid, password);
  lcd.init();                 // Display initialization
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);

  server.begin();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // Scale setup
  scale.set_scale(6.12);      // Correction factor
  scale.tare();               // Tare

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting BT Conn");
}

void loop() {
  if (SerialBT.hasClient()) {
    if (!isConnected) {
      isConnected = true;
      Serial.println("Bluetooth connected!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BT Connected!");
    }

    // Check for incoming data from Bluetooth
    if (SerialBT.available()) {
      char receivedChar = SerialBT.read();
      if (receivedChar == '\n') { // End of name input
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome:");
        lcd.setCursor(0, 1);
        lcd.print(wastepicker);
        Serial.println("Name received: " + wastepicker);
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Place weight...");
      } else {
        wastepicker += receivedChar; // Append character to the name
      }
    }

    // Measure weight and send it via Bluetooth
    float weightg = scale.get_units(10);
    float weightkg = weightg / 1000;
    lcd.setCursor(0, 1);
    lcd.print(weightkg);
    lcd.print(" Kg ");
    SerialBT.println("Weight: " + String(weightkg, 2) + " Kg");

    delay(5000); // Update every 5 seconds
  } else {
    if (isConnected) {
      isConnected = false;
      Serial.println("Bluetooth disconnected!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BT Disconnected");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Waiting BT Conn");
    }
  }

  // WiFi client handling
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            float weightg = scale.get_units(10);
            float weightkg = weightg / 1000;

            String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            response += "<html><body><h1>ESP32 Scale</h1>";
            response += "<p>Weight: " + String(weightkg, 2) + " Kg</p>";
            response += "<p>Waste Picker: " + wastepicker + "</p>";
            response += "</body></html>";

            client.print(response);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
