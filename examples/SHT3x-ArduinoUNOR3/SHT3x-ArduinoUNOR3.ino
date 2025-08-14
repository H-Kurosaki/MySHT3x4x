/*
 * Arduino UNO + SHT3x サンプルプログラム
 * 
 * 接続:
 * SHT3x VCC -> 5V
 * SHT3x GND -> GND
 * SHT3x SDA -> A4 (SDA)
 * SHT3x SCL -> A5 (SCL)
 */

//注意:SHT4xに使用した場合、エラーは出ませんが正しい値は読み出せません
//Note: When used with SHT4x, no error occurs but the correct value cannot be read.
//このプログラムでは値の更新間隔は2秒に1回です
//In this program, the value is updated every 2 seconds.

#include "MySHT3x4x.h"

SHT3x sensor;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // シリアル接続待機
  
  Serial.println("SHT3x Temperature & Humidity Sensor Test");
  Serial.println("=========================================");
  
  // I2C初期化
  Wire.begin();
  
  // センサー初期化 (デフォルトアドレス 0x45)
  if (sensor.begin()) {
    Serial.println("SHT3x sensor initialized successfully!");
  } else {
    Serial.println("Failed to initialize SHT3x sensor");
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