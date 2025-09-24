#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(96000); // 통신 속도를 96000으로 변경

  if (tcs.begin()) {
    Serial.println("Sensor found. Starting data transmission...");
    // 파이썬 스크립트에서 헤더를 처리하므로 여기서는 삭제합니다.
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  // clear 원시 값을 저장할 c_raw 변수 사용
  uint16_t r_raw, g_raw, b_raw, c_raw;

  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  uint16_t lux = tcs.calculateLux(r_raw, g_raw, b_raw);
  uint16_t colorTemp = tcs.calculateColorTemperature(r_raw, g_raw, b_raw);

  if (c_raw == 0) {
    return;
  }
  // 변수 타입을 uint8_t로 변경하여 메모리 효율성 및 정확성 향상
  uint8_t r_val = (float)r_raw / c_raw * 255.0;
  uint8_t g_val = (float)g_raw / c_raw * 255.0;
  uint8_t b_val = (float)b_raw / c_raw * 255.0;
  
  // constrain 함수는 uint8_t 타입에 불필요하므로 제거합니다.
  // r_val = constrain(r_val, 0, 255);
  // g_val = constrain(g_val, 0, 255);
  // b_val = constrain(b_val, 0, 255);

  // 데이터 출력 순서에 c_raw(clear 값) 추가
  Serial.print(r_val);      Serial.print(",");
  Serial.print(g_val);      Serial.print(",");
  Serial.print(b_val);      Serial.print(",");
  Serial.print(c_raw);      Serial.print(","); // c_raw 값을 출력
  Serial.print(lux);        Serial.print(",");
  Serial.println(colorTemp);

  delay(1000);
}