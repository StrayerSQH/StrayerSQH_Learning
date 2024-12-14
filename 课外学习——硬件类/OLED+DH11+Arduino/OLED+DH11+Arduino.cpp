#include "DHT.h"
#include <U8g2lib.h>

//配置构造函数
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 11, 10);

#define DHT11PIN 9

DHT dht(DHT11PIN, DHT11);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht.begin();

  u8g2.begin();//启动u8g2驱动程序
  u8g2.clearBuffer(); //清空显示屏缓存
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  //调用OLED显示屏
  OLEDShow(h, t, f);
  
  // Serial.print(F("Humidity: "));
  //Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  // Serial.print(f);
  // Serial.print(F("°F"));
  //Serial.print("\n");
}

void OLEDShow(float h, float t, float f){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_chinese2);//设置字体
  //u8g2.drawUTF8(0, 17, "SCU SQH/L G/C S");
  u8g2.drawUTF8(0, 17, "Hum:");
  u8g2.setCursor(40,17); 
  u8g2.print(h);
  u8g2.drawUTF8(80, 17, "%");
  u8g2.drawUTF8(0, 34, "Tem:");
  u8g2.setCursor(40,34); 
  u8g2.print(t);
  u8g2.drawUTF8(80, 34, "o");

  u8g2.sendBuffer();
}
