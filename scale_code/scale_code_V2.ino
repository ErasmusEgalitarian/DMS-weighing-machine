#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <cassert>	// Required for assert

LiquidCrystal_I2C lcd(0x27, 16, 2);	 // Display definition
const int LOADCELL_DOUT_PIN = 16;	 // HX711 pin 1
const int LOADCELL_SCK_PIN = 4;		 // HX711 pin 2

HX711 scale;
BluetoothSerial SerialBT;

// WiFi Configuration - We should change this to a ESP to ESP connection with
// the mother ESP connected to LAN
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
WiFiServer server(80);

String wastepicker = "";   // Variable to store the name of the waste picker
bool isConnected = false;  // Bluetooth connection status

void setup() {
	Serial.begin(115200);			// Serial communication
	SerialBT.begin("ESP32_Scale");	// Bluetooth device name
	Serial.println("Bluetooth started, waiting for connection...");

	WiFi.begin(ssid, password);
	lcd.init();	 // Display initialization
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

	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // Scale setup
	scale.set_scale(20.21);							   // Correction factor
	scale.tare();									   // Tare

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Waiting BT Conn");

  //Initialising variables for later use
  float weightg = 0;
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

		// Assumption is that data is in JSON format like this -
		//  {"trash_type": "palstic/metal/paper", "wastepicker_name": "some
		//  name", "waste_picker_ID": "22331"}

		if (SerialBT.avaliable()) {
			String json_data = "";
			char current_char = " ";
			while (current_char != "\n") {
				current_char = SerialBT.read();
				json_data += current_char;
			}
			JsonDocument doc;
			deserializeJson(doc, json_data);
			try {
				char* wastepicker_name = doc["wastepicker_name"];
				char* wastepicker_ID = doc["wastepicker_ID"];
				char* trash_type = doc["trash_type"];
				assert(wastepicker_name != "" && wastepicker_ID != "" &&
					   trash_type != "");

				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Welcome:");
				lcd.setCursor(0, 1);
				lcd.print(wastepicker_name);
				delay(5000) lcd.clear();
				lcd.setCursor(0, 0);
			}
		}
		catch {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ERROR: reading BT");
		}
	}

	// Check if there is already weight placed on the scale.
	// If not, LCD will display a message that asks to place weight on the scale
	// to be measured.
	if (weightg < 10000) {
		lcd.print("Place weight...");
	} else {
		// Measure weight
    float oldWeight = weightg;
		float weightg = scale.get_units(10);
		float weightkg = weightg / 1000;

		if (weightg != oldWeight) {
			lcd.clear();
			lcd.setCursor(0, 1);
			lcd.print(weightkg);
			lcd.print(" Kg ");
		}
	}
}
else {
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

// Make HTTP Post Request instead
					// Will send data in JSON format, too, like this -
					// {"trash_type": "palstic/metal/paper", "wastepicker_name":
					// "some name", "waste_picker_ID": "22331", "weight": 123}

// TO DO:
// - HTTP POST request to API
// - Fix Wifi Client handling
// - App via BT sends signal to weight and send data
// - BT response to App about the weight send to API
// - Error handling and responses thru BT to App