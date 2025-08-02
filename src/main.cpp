#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Adafruit TCS34725 라이브러리 객체 생성
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup() {
  // 시리얼 통신 시작
  Serial.begin(9600);

  // 컬러 센서 초기화
  if (tcs.begin()) {
    // =================================================================
    // 시리얼 모니터에 헤더(설명) 출력 (핵심 수정 사항)
    Serial.println("Sensor found. Starting data transmission...");
    Serial.println("Red,Green,Blue,Lux,Color_Temp_K"); 
    // =================================================================
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  uint16_t r_raw, g_raw, b_raw, c_raw;

  // 센서로부터 원시 데이터(Raw Data)를 읽어옴
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  // 조도(Lux)와 색온도(Kelvin) 계산
  uint16_t lux = tcs.calculateLux(r_raw, g_raw, b_raw);
  uint16_t colorTemp = tcs.calculateColorTemperature(r_raw, g_raw, b_raw);

  // 0-255 범위로 R,G,B 값 정규화
  if (c_raw == 0) {
    return;
  }
  int r_val = (float)r_raw / c_raw * 255.0;
  int g_val = (float)g_raw / c_raw * 255.0;
  int b_val = (float)b_raw / c_raw * 255.0;
  
  // 값이 255를 넘지 않도록 제한
  r_val = constrain(r_val, 0, 255);
  g_val = constrain(g_val, 0, 255);
  b_val = constrain(b_val, 0, 255);

  // 파이썬으로 전송할 데이터 포맷: "R,G,B,Lux,Kelvin\n"
  Serial.print(r_val);      Serial.print(",");
  Serial.print(g_val);      Serial.print(",");
  Serial.print(b_val);      Serial.print(",");
  Serial.print(lux);        Serial.print(",");
  Serial.println(colorTemp);

  // 측정 간격 설정
  delay(1000);
}