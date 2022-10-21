//Kompilacja z dnia 21.10.2022
//W pełni sprawna
//Z obsługą LCD po I2C

#include <DHT.h>
#include <DHT_U.h>
#include <IRremote.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <DS3231.h>
//#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);
//deklaracje
DS3231  rtc(SDA, SCL);
Adafruit_BMP280 bmp280;
DHT dht11(9,DHT11);
Time t;
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//ir 
const byte IR_RECEIVE_PIN = 10;

//stale
// On_Off by hour
const int OnHour = 18;
const int OnMin = 5;
const int OffHour = 18;
const int OffMin = 6; 
// On/Off by temperature
const int hot = 25;
const int hpressure = 999;
char cieplo[14]; 
char cisnienie[15];
char wysokosc[14];

void setup() {
  Serial.begin(9600);
  //start odbiornika IR
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  //LCD
  lcd.begin(16, 2);
  //RTC
  rtc.begin();
  // przycisk
  pinMode(8, INPUT_PULLUP);
  //check bmp280 module
  if (!bmp280.begin(0x76)) //w nawiasie podajemy nasz adres I2C tutaj wpisałem 0x76
  {  // sprawdzamy czy czujnik odpowiada jesli nie wyświetlamy komunikat o błędzie
    Serial.println("Nie znaleziono czujnika - sprawdź połączenie!");
    while (1);
  }
  //lcd - enable backlight
  lcd.backlight();
  
}
void loop(){
  //ir 
  if (IrReceiver.decode())
  {
     Serial.println(IrReceiver.decodedIRData.command);
     IrReceiver.resume();
  }
  //czujniki
  float temperature = bmp280.readTemperature();  // get temperature
  float pressure    = bmp280.readPressure();     // get pressure
  float altitude_   = bmp280.readAltitude(1013.25);
  float hum = dht11.readHumidity();
  float temperature_ = dht11.readTemperature();
  sprintf(cieplo, "%d.%02u%cC  ", (int)temperature, (int)(temperature * 100)%100, 223);
  sprintf(cisnienie, "%u.%02u hPa ", (int)(pressure/100), (int)((uint32_t)pressure % 100));
  int sensorVal = digitalRead(8);
  //diody
  if(pressure > hpressure){
    pinMode(6,OUTPUT);
    digitalWrite(6,HIGH);
  }
  else {
    pinMode(6,OUTPUT);
    digitalWrite(6,LOW);
  }
  if(temperature > hot){
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
  }
  else {
    pinMode(7,OUTPUT);
    digitalWrite(7,LOW);
  }
  // lcd
   if (sensorVal == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print(rtc.getTimeStr(FORMAT_SHORT));
    lcd.setCursor(5, 0);
    lcd.print("  ");
    lcd.setCursor(8, 0);
    lcd.print(rtc.getDateStr(FORMAT_SHORT));
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(2, 1);
    lcd.print(cieplo);
    lcd.setCursor(9, 1);
    lcd.print("  ");
    lcd.setCursor(11, 1);
    lcd.print(rtc.getDOWStr(FORMAT_SHORT));
    lcd.setCursor(14, 1);
    lcd.print("  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print(altitude_);
    lcd.setCursor(6, 0);
    lcd.print("  ");
      lcd.setCursor(8, 0);
      lcd.print(hum);
      lcd.setCursor(13, 0);
      lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(cisnienie);
    lcd.setCursor(11, 1);
    lcd.print(" ");
    lcd.setCursor(13, 1);
    lcd.print(temperature_); 
  }
}
