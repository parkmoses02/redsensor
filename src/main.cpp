#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup()
{
  Serial.begin(9600);
  Serial.println("=== TCS34725 센서 테스트 시작 ===");
  Serial.println("I2C 초기화 중...");
  
  Wire.begin();
  Serial.println("I2C 초기화 완료");
  
  delay(1000);
  
  if (tcs.begin()) {
    Serial.println("SUCCESS: TCS34725 센서 발견!");
    Serial.println("데이터 전송을 시작합니다...");
  } else {
    Serial.println("ERROR: TCS34725 센서를 찾을 수 없습니다!");
    Serial.println("연결을 확인하세요:");
    Serial.println("VIN -> 5V, GND -> GND, SCL -> A5, SDA -> A4");
    while (1) {
      Serial.println("센서 미연결. 연결 후 리셋하세요.");
      delay(3000);
    }
  }
}

void loop()
{
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  
  uint16_t lux = tcs.calculateLux(r, g, b);
  uint16_t colorTemp = tcs.calculateColorTemperature(r, g, b);
  
  if (c == 0) {
    Serial.println("WARNING: Clear 값이 0입니다. 센서 확인 필요.");
    delay(1000);
    return;
  }
  
  uint8_t r_norm = (uint8_t)((float)r / c * 255.0);
  uint8_t g_norm = (uint8_t)((float)g / c * 255.0);
  uint8_t b_norm = (uint8_t)((float)b / c * 255.0);
  
  // CSV 형식 데이터 출력
  Serial.print(r_norm); Serial.print(",");
  Serial.print(g_norm); Serial.print(",");
  Serial.print(b_norm); Serial.print(",");
  Serial.print(c); Serial.print(",");
  Serial.print(lux); Serial.print(",");
  Serial.println(colorTemp);
  
  delay(1000);
}