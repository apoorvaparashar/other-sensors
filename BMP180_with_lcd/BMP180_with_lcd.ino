#include <Wire.h>
#include <SFE_BMP180.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.

SFE_BMP180 bmp180;

int Altitude = 431; //current altitude in meters

void setup() {
  Serial.begin(115200);
  bool success = bmp180.begin();
  start:
  if (success) {
    Serial.println("BMP180 init success");
  }
  else
    { Serial.println("Fail");
      goto start;}
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
}

void loop() {

  char status;\
  double T, P;
  bool success = false;

  status = bmp180.startTemperature();

  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          float comp = bmp180.sealevel(P, Altitude);
          
          Serial.print("Pressure: ");
          Serial.print(comp);
          Serial.println(" hPa");

          Serial.print("Temperature: ");
          Serial.print(T);
          Serial.println(" C");
  lcd.clear();
  lcd.setCursor(0,0); //First line
  lcd.print("Temp :");
  lcd.print(T);
  lcd.print("C");
  lcd.setCursor(0,1); //Second line
  lcd.print("Press :");
  lcd.print(comp);
  lcd.print("hpa");
        }
      }
    }
  }
}


