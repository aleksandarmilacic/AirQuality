/*
 Name:		AirQuality.ino
 Created:	3/24/2024 10:43:47 PM
 Author:	Aleksandar
*/
#include <Wire.h>
#include <Adafruit_SGP30.h>
#include <LiquidCrystal_I2C.h>
Adafruit_SGP30 sgp; // Create an SGP30 instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions
void setup() {
    Serial.begin(9600);
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight

    if (!sgp.begin()) {
        Serial.println("Sensor not found :(");
        while (1);
    }
    Serial.println("SGP30 Found!");
    lcd.print("SGP30 Ready");
}
void loop() {
    if (!sgp.IAQmeasure()) {
        Serial.println("Measurement failed");
        return;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("eCO2: ");
    lcd.print(sgp.eCO2);
    lcd.print(" ppm");

    lcd.setCursor(0, 1);
    lcd.print("TVOC: ");
    lcd.print(sgp.TVOC);
    lcd.print(" ppb");

    delay(1000); // Wait for 1 second before the next reading
}
