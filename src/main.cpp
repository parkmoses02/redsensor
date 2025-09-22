// src/main.cpp
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// 색상 정보 구조체
struct Color {
  const char* name;
  float L, a, b;
};

// 기준 색상 목록 (LAB 값)
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

// 센서 초기화
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

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

// 가장 가까운 색상 찾기 함수
const char* findClosestColor(float L_measured, float a_measured, float b_measured) {
  float minDeltaE = 1000.0;
  const char* closestColorName = "Unknown";
  
  for (int i = 0; i < numColors; i++) {
    float deltaL = L_measured - colorMap[i].L;
    float deltaA = a_measured - colorMap[i].a;
    float deltaB = b_measured - colorMap[i].b;
    float deltaE = sqrt(deltaL * deltaL + deltaA * deltaA + deltaB * deltaB);
    
    if (deltaE < minDeltaE) {
      minDeltaE = deltaE;
      closestColorName = colorMap[i].name;
    }
  }
  
  return closestColorName;
}

void setup() {
  Serial.begin(115200);
  Serial.println("==============================");
  Serial.println("TCS34725 센서 테스트 시작");
  Serial.println("==============================");
  
  Wire.begin();
  Serial.println("I2C 초기화 완료");
  
  if (tcs.begin()) {
    Serial.println("✓ TCS34725 센서 감지됨!");
    Serial.println("✓ 센서 정보:");
    Serial.print("  - Integration Time: ");
    Serial.println("154ms");
    Serial.print("  - Gain: ");
    Serial.println("4x");
    Serial.println("✓ 데이터 읽기 시작...");
  } else {
    Serial.println("✗ TCS34725 센서를 찾을 수 없습니다!");
    Serial.println("🔍 연결 상태 확인:");
    Serial.println("  - VCC → 3.3V 또는 5V");
    Serial.println("  - GND → GND");
    Serial.println("  - SDA → A4 (Uno)");
    Serial.println("  - SCL → A5 (Uno)");
    Serial.println("  - 센서 전원 LED가 켜져있는지 확인");
    
    // I2C 스캔 시도
    Serial.println("🔍 I2C 장치 스캔 중...");
    for (byte i = 1; i < 127; i++) {
      Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0) {
        Serial.print("  발견된 I2C 주소: 0x");
        if (i < 16) Serial.print("0");
        Serial.println(i, HEX);
      }
    }
    Serial.println("I2C 스캔 완료");
  }
  Serial.println("==============================");
}

void loop() {
  Serial.println("📊 색상 센서 테스트 중...");
  
  // I2C 스캔부터 시작
  Serial.println("🔍 I2C 장치 스캔:");
  bool deviceFound = false;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("  발견: 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
      deviceFound = true;
    }
  }
  
  if (!deviceFound) {
    Serial.println("  ❌ I2C 장치를 찾을 수 없습니다!");
    Serial.println("  연결을 확인하세요:");
    Serial.println("  - VCC → 3.3V");
    Serial.println("  - GND → GND");  
    Serial.println("  - SDA → A4");
    Serial.println("  - SCL → A5");
    Serial.println("==============================");
    delay(5000);
    return;
  }
  
  // TCS34725 센서 직접 테스트
  Serial.println("🌈 TCS34725 센서 테스트:");
  if (!tcs.begin()) {
    Serial.println("  ❌ 센서 초기화 실패!");
    Serial.println("  센서가 0x29 주소에 있는지 확인하세요.");
    Serial.println("==============================");
    delay(5000);
    return;
  }
  
  Serial.println("  ✅ 센서 초기화 성공!");
  
  // 색상 데이터 읽기
  uint16_t r_raw, g_raw, b_raw, c_raw;
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);
  
  Serial.print("Raw: R=");
  Serial.print(r_raw);
  Serial.print(" G=");
  Serial.print(g_raw);
  Serial.print(" B=");
  Serial.print(b_raw);
  Serial.print(" C=");
  Serial.println(c_raw);
  
  if (c_raw == 0) {
    Serial.println("  ⚠️ Clear 값이 0 - 조도가 너무 낮거나 센서가 가려짐");
    Serial.println("==============================");
    delay(3000);
    return;
  }
  
  // RGB 변환
  uint8_t red   = (uint8_t)min(255, (uint32_t)r_raw * 255 / c_raw);
  uint8_t green = (uint8_t)min(255, (uint32_t)g_raw * 255 / c_raw);
  uint8_t blue  = (uint8_t)min(255, (uint32_t)b_raw * 255 / c_raw);
  
  Serial.print("RGB: ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
  
  // LAB 변환 및 색상 인식
  float L, a, B;
  rgbToLab(red, green, blue, L, a, B);
  
  Serial.print("LAB: L=");
  Serial.print(L, 1);
  Serial.print(" a=");
  Serial.print(a, 1);
  Serial.print(" b=");
  Serial.println(B, 1);
  
  const char* detectedColor = findClosestColor(L, a, B);
  Serial.print("🎯 감지된 색상: ");
  Serial.println(detectedColor);
  
  Serial.println("==============================");
  delay(2000); // 2초 간격으로 더 빠르게
}