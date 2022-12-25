#include <HID-Project.h>
#include <HID-Settings.h>

//#include <Keyboard.h>
#include <IRremote.hpp>

const byte IR_RECEIVE_PIN = 4;

const byte runExplorer = 36;  //run Explorer
const byte showStart = 13;  //show Start Menu
const byte showNotify = 15;
const byte backspaceButton = 35;
//controls
const byte arrowUp = 30;
const byte arrowDown = 31;
const byte arrowLeft = 32;
const byte arrowRight = 33;
const byte buttonOK = 34;
const byte pageUp = 18;
const byte pageDown = 19;
const byte winTab = 128;
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


void setup()
{
 
   Serial.begin(9600);
   Keyboard.begin();
   Serial.println("IR Receive test");
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
   if (IrReceiver.decode()){
   Serial.println(IrReceiver.decodedIRData.command);
   switch(IrReceiver.decodedIRData.command){
      
      case showStart:
        Keyboard.press(KEY_LEFT_GUI);
      break;
      case showNotify:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('a');
      break;
      // open file explorer
      case runExplorer:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('e');
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
      }
    delay(100);
    Keyboard.releaseAll();
    IrReceiver.resume();
   }
}
