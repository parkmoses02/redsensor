#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define SPEAKER_PIN 8

int tones[] = {440};

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_154MS,
  TCS34725_GAIN_4X
);

// ----------------- 판정 임계값 -----------------
const float RATIO_MIN = 1.31f;
const float RATIO_MAX = 1.37f;

const int R_MIN = 81;    // R 하한
const int R_MAX = 85;    // R 상한

const int G_MIN = 91;    // G 하한
const int G_MAX = 93;    // G 상한

const int B_MIN = 54;    // B 하한
const int B_MAX = 57;    // B 상한
// ------------------------------------------------

void setup() {
  Serial.begin(9600);
  Serial.println("=== TCS34725 센서 + 부저 테스트 시작 ===");

  Wire.begin();
  delay(200);

  if (tcs.begin()) {
    Serial.println("SUCCESS: TCS34725 센서 발견!");
  } else {
    Serial.println("ERROR: TCS34725 센서를 찾을 수 없습니다!");
    while (1) delay(3000);
  }

  pinMode(SPEAKER_PIN, OUTPUT);
  Serial.println("R,G,B,C,ratio,label");
}

void playMelody() {
  tone(SPEAKER_PIN, tones[0]);
  delay(300);
  noTone(SPEAKER_PIN);
}

void loop() {
  uint16_t r_raw, g_raw, b_raw, c;
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c);

  if (c == 0) {
    Serial.println("warning_clear_zero");
    delay(500);
    return;
  }

  // 정규화
  uint8_t r = constrain((float)r_raw / c * 255.0f, 0, 255);
  uint8_t g = constrain((float)g_raw / c * 255.0f, 0, 255);
  uint8_t b = constrain((float)b_raw / c * 255.0f, 0, 255);

  float avg_rb = max(1.0f, ((float)r + (float)b) / 2.0f);
  float ratio = g / avg_rb;

  bool in_box = (r >= R_MIN && r <= R_MAX) &&
                (g >= G_MIN && g <= G_MAX) &&
                (b >= B_MIN && b <= B_MAX);

  bool isYellow = (ratio >= RATIO_MIN && ratio <= RATIO_MAX && in_box);

  Serial.print(r); Serial.print(",");
  Serial.print(g); Serial.print(",");
  Serial.print(b); Serial.print(",");
  Serial.print(c); Serial.print(",");
  Serial.print(ratio, 2); Serial.print(",");
  if (isYellow) Serial.println("YELLOW");
  else Serial.println("NONE");

  // ✅ "YELLOW가 아닐 때"만 부저 울림
  if (!isYellow) {
    playMelody();
  }

  delay(500);
}
