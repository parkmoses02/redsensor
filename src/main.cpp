// src/main.cpp
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// RGB to XYZ 변환 함수
void rgbToXyz(float r, float g, float b, float &x, float &y, float &z) {
  // RGB 값을 0-1 범위로 정규화
  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;
  
  // sRGB to linear RGB
  if (r > 0.04045) r = pow((r + 0.055) / 1.055, 2.4);
  else r = r / 12.92;
  if (g > 0.04045) g = pow((g + 0.055) / 1.055, 2.4);
  else g = g / 12.92;
  if (b > 0.04045) b = pow((b + 0.055) / 1.055, 2.4);
  else b = b / 12.92;
  
  r *= 100;
  g *= 100;
  b *= 100;
  
  // Observer = 2°, Illuminant = D65
  x = r * 0.4124 + g * 0.3576 + b * 0.1805;
  y = r * 0.2126 + g * 0.7152 + b * 0.0722;
  z = r * 0.0193 + g * 0.1192 + b * 0.9505;
}

// XYZ to LAB 변환 함수
void xyzToLab(float x, float y, float z, float &L, float &a, float &B) {
  // Reference white D65
  x = x / 95.047;
  y = y / 100.0;
  z = z / 108.883;
  
  if (x > 0.008856) x = pow(x, 1.0/3.0);
  else x = (7.787 * x) + (16.0/116.0);
  if (y > 0.008856) y = pow(y, 1.0/3.0);
  else y = (7.787 * y) + (16.0/116.0);
  if (z > 0.008856) z = pow(z, 1.0/3.0);
  else z = (7.787 * z) + (16.0/116.0);
  
  L = (116 * y) - 16;
  a = 500 * (x - y);
  B = 200 * (y - z);
}

// RGB to LAB 변환 함수
void rgbToLab(uint8_t r, uint8_t g, uint8_t b, float &L, float &a, float &B) {
  float x, y, z;
  rgbToXyz(r, g, b, x, y, z);
  xyzToLab(x, y, z, L, a, B);
}

<<<<<<< HEAD
void setup() {
  Serial.begin(9600);
  
  if (tcs.begin()) {
    Serial.println("TCS34725 센서를 찾았습니다!");
  } else {
    Serial.println("TCS34725 센서를 찾을 수 없습니다... 연결을 확인하세요");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  
  // Raw 값을 0-255 범위로 변환
  if (c == 0) return; // 0으로 나누기 방지
  
  uint8_t red   = (uint8_t)min(255, (int32_t)r * 255 / c);
  uint8_t green = (uint8_t)min(255, (int32_t)g * 255 / c);
  uint8_t blue  = (uint8_t)min(255, (int32_t)b * 255 / c);
  
  // LAB 값 변수
  float L, a, B;
  
  // RGB를 LAB로 변환
  rgbToLab(red, green, blue, L, a, B);
  
  Serial.print("RGB: (");
  Serial.print(red); Serial.print(", ");
  Serial.print(green); Serial.print(", ");
  Serial.print(blue); Serial.println(")");
  
  Serial.print("LAB: (L=");
  Serial.print(L, 2); Serial.print(", a=");
  Serial.print(a, 2); Serial.print(", b=");
  Serial.print(B, 2); Serial.println(")");
  Serial.println("----------------------------------------");
  
=======
void setup()
{
  Serial.begin(9600); // 통신 속도를 9600으로 변경

  if (tcs.begin())
  {
    Serial.println("Sensor found. Starting data transmission...");
    // 파이썬 스크립트에서 헤더를 처리하므로 여기서는 삭제합니다.
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
  // clear 원시 값을 저장할 c_raw 변수 사용
  uint16_t r_raw, g_raw, b_raw, c_raw;

  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  uint16_t lux = tcs.calculateLux(r_raw, g_raw, b_raw);
  uint16_t colorTemp = tcs.calculateColorTemperature(r_raw, g_raw, b_raw);

  if (c_raw == 0)
  {
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
  Serial.print(r_val);
  Serial.print(",");
  Serial.print(g_val);
  Serial.print(",");
  Serial.print(b_val);
  Serial.print(",");
  Serial.print(c_raw);
  Serial.print(","); // c_raw 값을 출력
  Serial.print(lux);
  Serial.print(",");
  Serial.println(colorTemp);

>>>>>>> 5ee244611f990a980da7f64da68017eba7fc940e
  delay(1000);
}