#include <Adafruit_TinyUSB.h>
#include <bluefruit.h>

BLEDis bledis;
BLEHidAdafruit blehid;

void setup() {
  // Start serial port
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 HID Joystick");
  Serial.println();

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
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    // gamepad string map: [lft_btn, rht_btn, mid_btn, left, right, up, down]
    for (int i=0; i<7; i++) {
      // read one byte from serial buffer and save to data_rcvd
      char data_rcvd = Serial.read();
  
      if (data_rcvd == 'y') {
        switch (i) {
          case 0:
            blehid.keyPress('j');
            break;
          case 1:
            blehid.keyPress('k');
            break;
          case 2:
            blehid.keyPress('l');
            break;
          case 3:
            blehid.keyPress('a');
            break;
          case 4:
            blehid.keyPress('d');
            break;
          case 5:
            blehid.keyPress('w');
            break;
          case 6:
            blehid.keyPress('s');
            break;
          default:
            break;
        }
      }
    }

    // Release all keys
    blehid.keyRelease();
  }
}
