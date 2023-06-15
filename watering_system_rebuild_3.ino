#include <Wire.h>
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
Time t;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

//morning
uint8_t m_event_start_hh = 15;
uint8_t m_event_start_mm = 8;
uint8_t m_event_stop_hh = 15;
uint8_t m_event_stop_mm = 9;
//evening
uint8_t e_event_start_hh = 15;
uint8_t e_event_start_mm = 55;
uint8_t e_event_stop_hh = 15;
uint8_t e_event_stop_mm = 56;
int relay_1 = 9;
int relay_2 = 10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  rtc.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Witaj!");
  display.display();
  delay(5000);
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(rtc.getTimeStr(2));
  display.setCursor(0, 16);
  display.println(rtc.getTemp());
  display.display();
  t = rtc.getTime();
  Serial.println(rtc.getTimeStr(2));
  //if (rtc.getDOWStr()=="Monday" || rtc.getDOWStr()=="Wednesday" || rtc.getDOWStr()=="Friday"){
    if (t.hour == m_event_start_hh && t.min == m_event_start_mm){
    Serial.println("podlewam");
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("podlewam");
    digitalWrite(relay_1, HIGH);
    }
    if (t.hour >= m_event_stop_hh && t.min >= m_event_stop_mm){
    Serial.println("Zawór zamknięty");
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.println("Zawór zamknięty");
    digitalWrite(relay_1, LOW);
    }
    if (t.hour == e_event_start_hh && t.min == e_event_start_mm){
    Serial.println("Trwa podlewanie");
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("podlewam");
    digitalWrite(relay_1, HIGH);
    }
    if ((t.hour >= e_event_stop_hh) && (t.min >= e_event_stop_mm)){
    Serial.println("Zawór zamknięty");
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.println("Zawór zamknięty");
    digitalWrite(relay_1, LOW);
    }
  //}
  else {Serial.println("coś innego");}
  delay(1000);
}
