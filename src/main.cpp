#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ColorConverter.h>

// 색상 정보 구조체
struct Color {
  const char* name;
  double L, a, b;
};

// 기준 색상 목록
Color colorMap[] = {
  {"빨강 (Red)",    53.24, 80.09, 67.20},
  {"주황 (Orange)", 62.25, 47.98, 72.33},
  {"노랑 (Yellow)", 97.14, -21.55, 94.48},
  {"초록 (Green)",  46.23, -51.70, 49.90},
  {"파랑 (Blue)",   32.30, 79.19, -107.86},
  {"보라 (Purple)", 29.78, 58.97, -36.49},
  {"검정 (Black)",  0.0,   0.0,   0.0},
  {"회색 (Gray)",   53.59, 0.0,   0.0},
  {"하양 (White)",  100.0, 0.0,   0.0}
};
const int numColors = sizeof(colorMap) / sizeof(Color);

// 센서 및 변환기 객체
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
ColorConverter converter;

void setup() {
  Serial.begin(115200);
  Serial.println("TCS34725 Color Recognition System");
  Serial.println("=================================");

  if (tcs.begin()) {
    Serial.println("Sensor found. Starting color detection...");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  Serial.println();
}.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(115200); // 통신 속도를 115200으로 변경

  if (tcs.begin()) {
    Serial.println("Sensor found. Starting data transmission...");
    // 파이썬 스크립트에서 헤더를 처리하므로 여기서는 삭제합니다.
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  uint16_t r_raw, g_raw, b_raw, c_raw;
  
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);
  
  if (c_raw == 0) {
    Serial.println("No light detected, skipping measurement...");
    delay(1000);
    return;
  }
  
  // RGB 값 계산 (0-255 범위)
  uint8_t r_val = (float)r_raw / c_raw * 255.0;
  uint8_t g_val = (float)g_raw / c_raw * 255.0;
  uint8_t b_val = (float)b_raw / c_raw * 255.0;
  
  // 추가 센서 데이터
  uint16_t lux = tcs.calculateLux(r_raw, g_raw, b_raw);
  uint16_t colorTemp = tcs.calculateColorTemperature(r_raw, g_raw, b_raw);
  
  // LAB 변환
  double L_sensor, a_sensor, b_sensor;
  converter.RgbToLab(r_val, g_val, b_val, L_sensor, a_sensor, b_sensor);
  
  // 가장 가까운 색상 찾기
  double minDeltaE = 1000.0;
  const char* closestColorName = "Unknown";
  
  for (int i = 0; i < numColors; i++) {
    double deltaL = L_sensor - colorMap[i].L;
    double deltaA = a_sensor - colorMap[i].a;
    double deltaB = b_sensor - colorMap[i].b;
    double deltaE = sqrt(deltaL * deltaL + deltaA * deltaA + deltaB * deltaB);
    
    if (deltaE < minDeltaE) {
      minDeltaE = deltaE;
      closestColorName = colorMap[i].name;
    }
  }
  
  // 결과 출력
  Serial.print("Raw RGBC: (");
  Serial.print(r_raw); Serial.print(", ");
  Serial.print(g_raw); Serial.print(", ");
  Serial.print(b_raw); Serial.print(", ");
  Serial.print(c_raw); Serial.println(")");
  
  Serial.print("RGB: (");
  Serial.print(r_val); Serial.print(", ");
  Serial.print(g_val); Serial.print(", ");
  Serial.print(b_val); Serial.println(")");
  
  Serial.print("LAB: (L=");
  Serial.print(L_sensor, 2); Serial.print(", a=");
  Serial.print(a_sensor, 2); Serial.print(", b=");
  Serial.print(b_sensor, 2); Serial.println(")");
  
  Serial.print("Lux: "); Serial.print(lux);
  Serial.print(", Color Temp: "); Serial.print(colorTemp); Serial.println("K");
  
  Serial.print("Delta E: "); Serial.println(minDeltaE, 2);
  Serial.print("==> Detected Color: "); Serial.println(closestColorName);
  
  // CSV 형식 데이터 출력 (Python 분석용)
  Serial.print("CSV: ");
  Serial.print(r_val); Serial.print(",");
  Serial.print(g_val); Serial.print(",");
  Serial.print(b_val); Serial.print(",");
  Serial.print(c_raw); Serial.print(",");
  Serial.print(lux); Serial.print(",");
  Serial.print(colorTemp); Serial.print(",");
  Serial.print(L_sensor, 2); Serial.print(",");
  Serial.print(a_sensor, 2); Serial.print(",");
  Serial.print(b_sensor, 2); Serial.print(",");
  Serial.println(closestColorName);
  
  Serial.println("----------------------------------------");
  delay(2000);
}