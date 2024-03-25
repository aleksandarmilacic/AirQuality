/*
 Name:		AirQuality.ino
 Created:	3/24/2024 10:43:47 PM
 Author:	Aleksandar
*/
#include <Wire.h>
#include <Adafruit_SGP30.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
Adafruit_SGP30 sgp; // Create an SGP30 instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions
#define DHTPIN 2     // What digital pin the DHT22 is connected to
#define DHTTYPE DHT11   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// eCO2 thresholds in ppm
const int eCO2_low = 400;
const int eCO2_normal = 1000;
const int eCO2_moderate = 2000;
// Above eCO2_moderate is considered high

// TVOC thresholds in ppb
const int TVOC_low = 0;
const int TVOC_normal = 50;
const int TVOC_moderate = 200;
// Above TVOC_moderate is considered high

void setup() {
    Serial.begin(9600);
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight

    if (!sgp.begin()) {
        Serial.println("Sensor not found :(");
        while (1);
    }
    dht.begin();

    Serial.println("SGP30 Found!");
    lcd.print("SGP30 Ready");
}
unsigned long lastSwitchTime = 0; // Last time the display switched between values and descriptions
bool showDescription = false; // Start by showing the values

void loop() {
    if (!sgp.IAQmeasure()) {
        Serial.println("Measurement failed");
        return;
    }
    unsigned long currentTime = millis();

    // Switch display mode every 3 seconds
    if (currentTime - lastSwitchTime >= 3000) {
        showDescription = !showDescription;
        lastSwitchTime = currentTime;
        lcd.clear();
    }

    if (showDescription) {
        // Display air quality descriptions
        lcd.setCursor(0, 0);
        lcd.print("eCO2: ");
        lcd.print(getCO2Description(sgp.eCO2));

        lcd.setCursor(0, 1);
        lcd.print("TVOC: ");
        lcd.print(getTVOCDescription(sgp.TVOC));
    }
    else {
        // Display numeric values
        lcd.setCursor(0, 0);
        lcd.print("eCO2: ");
        lcd.print(sgp.eCO2);
        lcd.print(" ppm");

        lcd.setCursor(0, 1);
        lcd.print("TVOC: ");
        lcd.print(sgp.TVOC);
        lcd.print(" ppb");
    }
    delay(4000);
    // Reading temperature and humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // By default, the temperature is in Celsius
    // Display air quality descriptions
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print(" %");
    delay(4000);
}

String getCO2Description(int eCO2) {
    if (eCO2 <= eCO2_normal) return "Normal";
    else if (eCO2 <= eCO2_moderate) return "Moderate";
    else return "High";
}

String getTVOCDescription(int TVOC) {
    if (TVOC <= TVOC_normal) return "Normal";
    else if (TVOC <= TVOC_moderate) return "Moderate";
    else return "High";
}

