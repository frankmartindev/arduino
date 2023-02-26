#include <Key.h>
#include <Keypad.h>

#include <Adafruit_BMP280.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

#include <HID-Project.h>
#include <HID-Settings.h>
#include <IRremote.hpp>

const byte IR_RECEIVE_PIN = 10;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_BMP280 bmp280;

DS3231  rtc(SDA, SCL);
Time t;
// keypad
const byte ROWS = 1; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','4'},
};

byte rowPins[ROWS] = {8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//end keypad
int power = 0; //default status - off
int led_1 = 9; //define led pin
int led_2 = 14;
// power
const byte powerOnOff = 12;
const byte led_1_control = 93;
const byte led_2_control = 94;
const byte resetLCD = 91;
// others
const byte runExplorer = 36;  //run Explorer
const byte showStart = 13;  //show Start Menu
const byte showNotify = 15;
const byte backspaceButton = 35;
const byte displayOptions = 57;
const byte runCalc = 38;
//controls
const byte arrowUp = 30;
const byte arrowDown = 31;
const byte arrowLeft = 32;
const byte arrowRight = 33;
const byte buttonOK = 34;
const byte pageUp = 18;
const byte pageDown = 19;
const byte winTab = 23;
const byte OSK = 90;
//volume
const byte volumeUp = 16;
const byte volumeDown = 17;
const byte mute = 14;
//web mode
const byte fullScreen = 129;
//media controls
const byte playPause = 110;
const byte fforward = 20;
const byte backward = 21;
const byte justStop = 25;
const byte nextFile = 26;
const byte prevFile = 27;
//num pad
const byte kp_1 = 1;
const byte kp2 = 2;
const byte kp3 = 3;
const byte kp4 = 4;
const byte kp5 = 5;
const byte kp6 = 6;
const byte kp7 = 7;
const byte kp8 = 8;
const byte kp9 = 9;
const byte enterBtn = 11;

char cieplo[14];

void setup()
  {
    Serial.begin(74880);
    Wire.begin();
    //run rtc module
    rtc.begin();
    //RTC settings
    //rtc.setTime(18,10,00);
    //rtc.setDate(31,12,2022);
    //rtc.setDOW(SATURDAY);
    lcd.begin(16, 2);
    bmp280.begin(0x76);
    Keyboard.begin();
    Serial.println("IR Receive test");
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
}
void loop()
{
  char key = keypad.getKey();  
  float temperature = bmp280.readTemperature();
  float pressure    = bmp280.readPressure();
  float altitude_   = bmp280.readAltitude(1013.25);

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr(FORMAT_SHORT));
  lcd.setCursor(6,0);
  lcd.print("  ");
  lcd.setCursor(8,0);
  lcd.print(rtc.getDateStr(FORMAT_SHORT));
  //if (key){
  //  Serial.print("Key Pressed : ");
  //  Serial.println(key);
  //}
  if (key == '1') {
    lcd.clear();
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("DoW:");   
    lcd.setCursor(4,1);
    lcd.print(rtc.getDOWStr());
  }
  else if (key =='2'){
    lcd.clear();
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("Temp:");   
    lcd.setCursor(5,1);
    lcd.print(" ");
    lcd.setCursor(7, 1);
    lcd.print(temperature);
    lcd.setCursor(12, 1);
    lcd.print("*C");
            
  }
  else if (key =='3'){
    lcd.clear();
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("Pres:");   
    lcd.setCursor(5,1);
    lcd.print(" ");
    lcd.setCursor(7, 1);
    lcd.print(pressure/100);
    lcd.setCursor(13, 1);
    lcd.print("hPa");
  }
  else if (key =='4'){
    lcd.clear();
    delay(200);
    
  }

  // IR remote 
  if (IrReceiver.decode()){
   Serial.println(IrReceiver.decodedIRData.command);
   switch(IrReceiver.decodedIRData.command){
    //Serial.println(IrReceiver.decodedIRData.command);
      case powerOnOff:
        if(power == 1){
          digitalWrite(led_1,LOW);
          digitalWrite(led_2,LOW);
          power = 0;
          lcd.setCursor(0, 1);
          lcd.print("Lights OFF");
          delay(500);
          lcd.clear();
          delay(500); 
        } 
          else 
        {
          digitalWrite(led_1,HIGH);
          digitalWrite(led_2,HIGH);
          power = 1;
          lcd.setCursor(0, 1);
          lcd.print("Lights ON");
          delay(500);
          lcd.clear();          
          delay(500);
        }
      break;
      case led_1_control:
        if(power == 0){
          digitalWrite(led_1,HIGH);
          lcd.setCursor(0, 1);
          lcd.print("Light ON");
          delay(500);
          lcd.clear();
        }
      break;
      case led_2_control:
        if(power == 0){
          digitalWrite(led_2,HIGH);
          lcd.setCursor(0, 1);
          lcd.print("Light ON");
          delay(500);
          lcd.clear();
        }
      break;
      case resetLCD:
        delay(500);
        lcd.clear();
      break;
      case showStart:
        Keyboard.press(KEY_LEFT_GUI);
        lcd.setCursor(0, 1);
        lcd.print("Start Menu");
        delay(500);
        lcd.clear();
      break;
      case showNotify:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('a');
        lcd.setCursor(0, 1);
        lcd.print("Centrum powiadomien");
        delay(500);
        lcd.clear();        
      break;
      // open file explorer
      case runExplorer:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('e');
        lcd.setCursor(0, 1);
        lcd.print("Eksplorator Windows");
        delay(500);
        lcd.clear();
      break;
      // Win + Tab
      case winTab:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press(KEY_TAB);
      break;
      case backspaceButton:
        Keyboard.press(KEY_BACKSPACE);
      break;
      case pageUp:
        Keyboard.press(KEY_PAGE_UP);
      break;
      case pageDown:
        Keyboard.press(KEY_PAGE_DOWN);
      break;
      case arrowUp:
        Keyboard.press(KEY_UP_ARROW);
      break;
      case arrowDown:
        Keyboard.press(KEY_DOWN_ARROW);
      break;
      case arrowLeft:
        Keyboard.press(KEY_LEFT_ARROW);
      break;
      case arrowRight:
        Keyboard.press(KEY_RIGHT_ARROW);
      break;
      case buttonOK:
        Keyboard.press(KEY_RETURN);
      break;
      case enterBtn:
        Keyboard.press(KEY_RETURN);
      break;
      case volumeUp:
        Consumer.write(MEDIA_VOLUME_UP);
      break;
      case volumeDown:
        Consumer.write(MEDIA_VOLUME_DOWN);
      break;
      case mute:
        Consumer.write(MEDIA_VOLUME_MUTE);
      break;
      case fullScreen:
        Keyboard.press('f');
      break;
      case justStop:
        Consumer.write(MEDIA_STOP);
      break;
      case playPause:
        Consumer.write(MEDIA_PLAY_PAUSE);
      break;
      case fforward:
        Consumer.write(MEDIA_FAST_FORWARD);
      break;
      case backward:
        Consumer.write(MEDIA_REWIND);
      break;
      case nextFile:
        Consumer.write(MEDIA_NEXT);
      break;
      case prevFile:
        Consumer.write(MEDIA_PREVIOUS);
      break;
      case displayOptions:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('p');
      break;
      case runCalc:
        Consumer.write(CONSUMER_CALCULATOR);
      break;
      case kp_1:
        Keyboard.press('1');
      break;
      case kp2:
        Keyboard.press('2');
      break;
      case kp3:
        Keyboard.press('3');
      break;
      case kp4:
        Keyboard.press('4');
      break;
      case kp5:
        Keyboard.press('5');
      break;
      case kp6:
        Keyboard.press('6');
      break;
      case kp7:
        Keyboard.press('7');
      break;
      case kp8:
        Keyboard.press('8');
      break;
      case kp9:
        Keyboard.press('9');
      break;
      case OSK:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('o');
      break;
      }
    delay(50);
    Keyboard.releaseAll();
    IrReceiver.resume();
   }
}
