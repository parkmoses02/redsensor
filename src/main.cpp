#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// TCS34725 센서 객체 생성
// 적분 시간: 614ms, 게인: 1x
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
  // 시리얼 통신 시작
  Serial.begin(9600);
  Serial.println("TCS34725 색상 센서 테스트 시작!");

  // TCS34725 센서 초기화
  if (tcs.begin()) {
    Serial.println("✅ TCS34725 센서를 찾았습니다!");
  } else {
    Serial.println("❌ TCS34725 센서를 찾을 수 없습니다...");
    Serial.println("연결을 확인하세요:");
    Serial.println("VCC → 3.3V");
    Serial.println("GND → GND");
    Serial.println("SCL → A5");
    Serial.println("SDA → A4");
    while (1); // 센서가 없으면 대기
  }
  
  Serial.println("센서 준비 완료! 색상 측정을 시작합니다...");
  Serial.println("R\tG\tB\tC\t색상");
  Serial.println("---\t---\t---\t---\t----");
}

void loop() {
  uint16_t r, g, b, c;
  
  // 원시 색상 데이터 읽기
  tcs.getRawData(&r, &g, &b, &c);
  
  // 색상 값 출력
  Serial.print(r); Serial.print("\t");
  Serial.print(g); Serial.print("\t");
  Serial.print(b); Serial.print("\t");
  Serial.print(c); Serial.print("\t");
  
  // 간단한 색상 판별
  if (r > g && r > b) {
    Serial.println("🔴 빨간색");
  } else if (g > r && g > b) {
    Serial.println("🟢 초록색");
  } else if (b > r && b > g) {
    Serial.println("🔵 파란색");
  } else {
    Serial.println("⚪ 기타");
  }
  
  delay(1000); // 1초 대기
}
