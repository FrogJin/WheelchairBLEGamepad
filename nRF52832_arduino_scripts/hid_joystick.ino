/*********************************************************************
Author: Yuming Jin

 MIT license
*********************************************************************/
#include <bluefruit.h>

BLEDis bledis;
BLEHidAdafruit blehid;

bool buttonAStatus = LOW;
int buttonAPin = 15;
int leftStickXPin = A0;
int leftStickYPin = A1;

// defined in hid.h from Adafruit_TinyUSB_ArduinoCore
//hid_gamepad_report_t gp;
//hid_gamepad_button_bm_t bbm;

void setup() 
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 HID Joystick");
  Serial.println();

  // Digital In and Out
  pinMode(buttonAPin, INPUT);

  Bluefruit.begin();
  // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
  Bluefruit.Periph.setConnInterval(9, 16); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52_Joystick");

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather 52");
  bledis.begin();

  // BLE HID
  blehid.begin();

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_JOYSTICK);
//  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  
  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for 'Name' in the advertising packet
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds

//  Serial.print("Attempting to connect");
//  uint8_t i=0;
//  while(! Bluefruit.connected()) { 
//    Serial.print("."); delay(100);
//    if((++i)>50) {
//      i=0; Serial.println();
//    }
//  };
//  delay(1000);//just in case
//  Serial.println("\nConnection successful");

//  Reset buttons
//  Serial.println("No pressing buttons");
//  gp.buttons = 0;
//  gp.x       = 0;
//  gp.y       = 0;
//  gp.z       = 0;
//  gp.rz      = 0;
//  gp.rx      = 0;
//  gp.ry      = 0;
//  gp.hat     = 0;
//  blehid.gamepadReport(0, &gp);
//  delay(2000);
}

void loop()
{
  if ( buttonAStatus == LOW && digitalRead(buttonAPin) == HIGH )
  {
    Serial.println("Button A Pressed");
    blehid.keyPress('w');
//    blehid.gamepadButtonPress(GAMEPAD_BUTTON_A);
//    gp.buttons = GAMEPAD_BUTTON_A;
//    blehid.gamepadReport(0, &gp);
    buttonAStatus = HIGH;
  }
  else if ( buttonAStatus == HIGH && digitalRead(buttonAPin) == LOW )
  {
    Serial.println("Button A Released");
    blehid.keyRelease();
//    blehid.gamepadButtonRelease();
//    gp.buttons = 0;
//    blehid.gamepadReport(0, &gp);
    buttonAStatus = LOW;
  }
//  else if ( analogRead(leftStickXPin)/100 < 3 )
//  {
//    Serial.println("LeftStick right");
//    blehid.keyPress('d');
//    delay(5);
//    blehid.keyRelease();
//  }
//  else if ( analogRead(leftStickXPin)/100 > 7 )
//  {
//    Serial.println("LeftStick left");
//    blehid.keyPress('a');
//    delay(5);
//    blehid.keyRelease();
//  }

  delay(100);
}
