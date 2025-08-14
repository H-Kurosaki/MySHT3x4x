/*
 * Arduino UNO + SHT4x サンプルプログラム
 * 
 * 接続:
 * SHT4x VCC -> 5V
 * SHT4x GND -> GND
 * SHT4x SDA -> A4 (SDA)
 * SHT4x SCL -> A5 (SCL)
 */
//このプログラムでは値の更新間隔は2秒に1回です
//In this program, the value is updated every 2 seconds.

#include "MySHT3x4x.h"

SHT4x sensor;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // シリアル接続待機
  
  Serial.println("SHT4x Temperature & Humidity Sensor Test");
  Serial.println("=========================================");
  
  // I2C初期化
  Wire.begin();
  
  // センサー初期化 (デフォルトアドレス 0x44)
  if (sensor.begin()) {
    Serial.println("SHT4x sensor initialized successfully!");
    
    // シリアル番号読み取り (オプション)
    uint32_t serialNum;
    if (sensor.readSerialNumber(&serialNum)) {
      Serial.print("Serial Number: 0x");
      Serial.println(serialNum, HEX);
    }
    
    // 測定精度設定 (0:高精度, 1:中精度, 2:低精度)
    sensor.setPrecision(0);
    Serial.print("Precision: ");
    Serial.println(sensor.getPrecision() == 0 ? "High" : 
                   sensor.getPrecision() == 1 ? "Medium" : "Low");
  } else {
    Serial.println("Failed to initialize SHT4x sensor");
    Serial.println("Please check wiring and I2C address");
  }
  
  Serial.println();
  Serial.println("Time(s)\tTemp(°C)\tHumid(%)\tVPD(g/m³)");
  Serial.println("-------\t--------\t--------\t---------");
}

void loop() {
  unsigned long currentTime = millis();
  
  // 1秒間隔で実行
  if (currentTime - lastTime >= 1000) {
    lastTime = currentTime;
    
    // センサー読み取り実行
    bool result = sensor.autoRead1Sec();
    
    if (result) {
      // 成功時：データを表示
      Serial.print(currentTime / 1000);
      Serial.print("\t");
      Serial.print(sensor.temp, 2);
      Serial.print("\t\t");
      Serial.print(sensor.humidity, 2);
      Serial.print("\t\t");
      Serial.print(sensor.humiddiff, 2); // g/m³単位
      Serial.println();
    } else {
      // 失敗時：エラー表示
      Serial.print(currentTime / 1000);
      Serial.print("\tSensor Error (retry: ");
      Serial.print(sensor.retryCount);
      Serial.println(")");
    }
  }
}