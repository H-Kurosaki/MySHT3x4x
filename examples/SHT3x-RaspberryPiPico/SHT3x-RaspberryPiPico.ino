/*
 * Raspberry Pi Pico + SHT3x サンプルプログラム
 * 
 * 接続:
 * SHT3x VCC -> 3.3V (Pin 36)
 * SHT3x GND -> GND (Pin 38)
 * SHT3x SDA -> GP4 (Pin 6)
 * SHT3x SCL -> GP5 (Pin 7)
 */
 
//注意:SHT4xに使用した場合、エラーは出ませんが正しい値は読み出せません
//Note: When used with SHT4x, no error occurs but the correct value cannot be read.
//このプログラムでは値の更新間隔は2秒に1回です
//In this program, the value is updated every 2 seconds.

#include "MySHT3x4x.h"

// Wire0を使用してカスタムピンを指定
SHT3x sensor(Wire);
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // シリアル接続待機
  
  Serial.println("Raspberry Pi Pico + SHT3x Sensor Test");
  Serial.println("======================================");
  
  // I2C初期化 (SDA=GP4, SCL=GP5)
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();
  
  Serial.println("I2C initialized: SDA=GP4, SCL=GP5");
  
  // センサー初期化 (デフォルトアドレス 0x45)
  if (sensor.begin()) {
    Serial.println("SHT3x sensor initialized successfully!");
  } else {
    Serial.println("Failed to initialize SHT3x sensor");
    Serial.println("Please check wiring and I2C address");
    Serial.println("Trying alternative address 0x44...");
    
    if (sensor.begin(0x44)) {
      Serial.println("SHT3x found at address 0x44");
    } else {
      Serial.println("Sensor not found at any address");
    }
  }
  
  Serial.println();
  Serial.println("Time(s)\tTemp(°C)\tHumid(%)\tVPD(g/m³)\tRetry");
  Serial.println("-------\t--------\t--------\t---------\t-----");
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
      Serial.print("\t\t");
      Serial.print(sensor.retryCount);
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