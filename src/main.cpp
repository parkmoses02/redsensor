#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup()
{
  Serial.begin(9600); // Set communication speed to 9600

  if (tcs.begin())
  {
    Serial.println("Sensor found. Starting data transmission...");
    // Header is handled in Python script
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }
}

void loop()
{
  // Use c_raw variable to store clear raw values
  uint16_t r_raw, g_raw, b_raw, c_raw;

  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  uint16_t lux = tcs.calculateLux(r_raw, g_raw, b_raw);
  uint16_t colorTemp = tcs.calculateColorTemperature(r_raw, g_raw, b_raw);

  if (c_raw == 0)
  {
    return;
  }
  // Change variable type to uint8_t for memory efficiency and accuracy
  uint8_t r_val = (float)r_raw / c_raw * 255.0;
  uint8_t g_val = (float)g_raw / c_raw * 255.0;
  uint8_t b_val = (float)b_raw / c_raw * 255.0;

  // Constrain function is unnecessary for uint8_t type - removed
  // r_val = constrain(r_val, 0, 255);
  // g_val = constrain(g_val, 0, 255);
  // b_val = constrain(b_val, 0, 255);

  // Add c_raw (clear value) to data output order
  Serial.print(r_val);
  Serial.print(",");
  Serial.print(g_val);
  Serial.print(",");
  Serial.print(b_val);
  Serial.print(",");
  Serial.print(c_raw);
  Serial.print(","); // Output c_raw value
  Serial.print(lux);
  Serial.print(",");
  Serial.println(colorTemp);

  delay(1000);
}