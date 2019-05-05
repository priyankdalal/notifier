#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <BLEAdvertisedDevice.h>
// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
//#include <display.h>
#include<logo.h>
#include<notification.h>

#include <SPI.h>
#define TFT_CS     16
#define TFT_RST    9  // you can also connect this to the Arduino reset
                       // in which case, set this #define pin to -1!
#define TFT_DC     17
#define TFT_SCLK 5   // set these to be whatever pins you like!
#define TFT_MOSI 23   // set these to be whatever pins you like!
Display dis = Display(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Display *tft=&dis;


// BLE UUIDs
#define GENERIC_DISPLAY 18      //This device will appear as cycling device

static const BLEUUID ALERT_DISPLAY_SERVICE_UUID = BLEUUID("3db02924-b2a6-4d47-be1f-0f90ad62a048");

static const BLEUUID NOTIFICATION_CODE_UUID = BLEUUID("d6afa6d2-b79e-4d80-a517-47e898abb113");
static const BLEUUID NOTIFICATION_TITLE_UUID = BLEUUID("bbb19eb8-17ae-4345-b038-2964a44c30b6");
static const BLEUUID NOTIFICATION_CONVERSATION_TITLE_UUID = BLEUUID("8f1d743d-edf3-4b31-b70a-0781ce7395d3");
static const BLEUUID NOTIFICATION_SUBTEXT_UUID = BLEUUID("944511fb-c208-4922-8e1e-66347cf695d8");
static const BLEUUID NOTIFICATION_TEXT_UUID = BLEUUID("ef9edced-ff56-481e-9ac1-e30a3e731ec1");
static const BLEUUID NOTIFICATION_SELF_DISPLAY_NAME_UUID = BLEUUID("a2fa4dac-891b-499d-8f23-f559fc41f602");
static const BLEUUID NOTIFICATION_INFO_TEXT_UUID = BLEUUID("99c56b26-6b57-4919-8b35-e2ac42309546");
static const BLEUUID NOTIFICATION_ICON_UUID = BLEUUID("93acaf3e-6112-4904-97b1-0602eaf93994");
static const BLEUUID DEVICE_TIME_UUID = BLEUUID("efd6ec74-388b-4f48-9b54-78aa70b85fca");
static const BLEUUID DEVICE_BATTERY_UUID = BLEUUID("9960f7e4-4803-47e2-b97c-2a6bdb409e0f");

#define BUFF_LEN 256


volatile bool connected = false; //0=advertising; 1=connected
volatile bool hasText = false, hasBattText = false;
volatile bool hasTimeText = false;
volatile bool hasLongText = false;
volatile int yScrollPos = 0;
char* payload = new char[BUFF_LEN];
char* time_buffer = new char[BUFF_LEN];
volatile int batt_level = 0; // 
volatile int display_orientation = 0; //0=none;1=flip

Notification* dNotification = new Notification(tft);
//dNotification->setDisplay(tft);

#define SCROLL_DELAY 6
#define MAX_SCROLL_WINDOW_HEIGHT 160

//Callback to handle Time Charcterisitc events
class NotificationCodeCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pTimeCharacteristic) {
    std::string str = pTimeCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(time_buffer)); //clear buffer
    strcpy(payload, str.c_str());
    dNotification->setCode(atoi(payload));
    //tft->writeText(payload);
  }
};
//Callback to handle title Charcterisitc events
class NotificationTitleCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setTitle(payload);
    tft->fillRect(0,20,128,20,ST7735_BLACK);
    tft->writeText(payload,20);
    //tft->writeText(payload);
  }
};
//Callback to handle conversation title Charcterisitc events
class NotificationConverTitleCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setConversationTitle(payload);
    tft->fillRect(0,40,128,20,ST7735_BLACK);
    tft->writeText(payload,40);
    //tft->writeText(payload);
  }
};
//Callback to handle conversation sub text Charcterisitc events
class NotificationSubTextCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setSubText(payload);
    tft->fillRect(0,60,128,20,ST7735_BLACK);
    tft->writeText(payload,60);
    //tft->writeText(payload);
  }
};
//Callback to handle text Charcterisitc events
class NotificationTextCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setText(payload);
    tft->fillRect(0,80,128,20,ST7735_BLACK);
    tft->writeText(payload,80);
    //tft->writeText(payload);
  }
};
//Callback to handle self display name Charcterisitc events
class NotificationSelfDisplayNameCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setSelfDisplayName(payload);
    tft->fillRect(0,100,128,20,ST7735_BLACK);
    tft->writeText(payload,100);
    //tft->writeText(payload);
  }
};
//Callback to handle info text Charcterisitc events
class NotificationInfoTextCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    dNotification->setInfoText(payload);
    tft->fillRect(0,120,128,20,ST7735_BLACK);
    tft->writeText(payload,120);
    dNotification->processNotification();
    //tft->writeText(payload);
  }
};
//Callback to handle device time Charcterisitc events
class DeviceTimeCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    tft->writeText(payload,20);
    //tft->writeText(payload);
  }
};
//Callback to handle device time Charcterisitc events
class DeviceBatteryCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string str = pCharacteristic->getValue(); //read values
    memset(payload, '\0', sizeof(payload));
    strcpy(payload, str.c_str());
    tft->writeText(payload);
    //tft->writeText(payload);
  }
};


class MyServerCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    //todo display `connected` on the screen
    Serial.println("BLE onConnect");
    
    connected = true;
    memset(payload, 0x00, BUFF_LEN);
    hasLongText = false;
    hasText = false;
    hasTimeText = false;
    yScrollPos = 0;
    tft->writeText("connected");
  }

  void onDisconnect(BLEServer* pServer) {
    // todo display `disconnected` on the screen
    Serial.println("BLE onDisconnect");
    connected = false;
    tft->writeText("disconneted");
  }
};

void setupBLEServer() {
  //init BLE server
  BLEDevice::init("Notifier");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallback());
  // Alert Display Service
  BLEService *pService = pServer->createService( ALERT_DISPLAY_SERVICE_UUID );

  // Notification_CODE Characteristic
  BLECharacteristic* pCharacteristicCode = new BLECharacteristic(NOTIFICATION_CODE_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicCode->setCallbacks(new NotificationCodeCallback());
  pService->addCharacteristic(pCharacteristicCode);

  // NOTIFICATION_TITLE_UUID Characteristic
  BLECharacteristic* pCharacteristicTitle = new BLECharacteristic(NOTIFICATION_TITLE_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicTitle->setCallbacks(new NotificationTitleCallback());
  pService->addCharacteristic(pCharacteristicTitle);

  // NOTIFICATION_CONVERSATION_TITLE_UUID Characteristic
  BLECharacteristic* pCharacteristicConverTitle = new BLECharacteristic(NOTIFICATION_CONVERSATION_TITLE_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicConverTitle->setCallbacks(new NotificationConverTitleCallback());
  pService->addCharacteristic(pCharacteristicConverTitle);

  // NOTIFICATION_SUBTEXT_UUID Characteristic
  BLECharacteristic* pCharacteristicSubText = new BLECharacteristic(NOTIFICATION_SUBTEXT_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicSubText->setCallbacks(new NotificationSubTextCallback());
  pService->addCharacteristic(pCharacteristicSubText);

  // NOTIFICATION_TEXT_UUID Characteristic
  BLECharacteristic* pCharacteristicText = new BLECharacteristic(NOTIFICATION_TEXT_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicText->setCallbacks(new NotificationTextCallback());
  pService->addCharacteristic(pCharacteristicText);

  // NOTIFICATION_SELF_DISPLAY_NAME_UUID Characteristic
  BLECharacteristic* pCharacteristicSelfDisplayName = new BLECharacteristic(NOTIFICATION_SELF_DISPLAY_NAME_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicSelfDisplayName->setCallbacks(new NotificationSelfDisplayNameCallback());
  pService->addCharacteristic(pCharacteristicSelfDisplayName);

  // NOTIFICATION_INFO_TEXT_UUID Characteristic
  BLECharacteristic* pCharacteristicInfoText = new BLECharacteristic(NOTIFICATION_INFO_TEXT_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicInfoText->setCallbacks(new NotificationInfoTextCallback());
  pService->addCharacteristic(pCharacteristicInfoText);

  // DEVICE_TIME_UUID Characteristic
  BLECharacteristic* pCharacteristicTime = new BLECharacteristic(DEVICE_TIME_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicTime->setCallbacks(new DeviceTimeCallback());
  pService->addCharacteristic(pCharacteristicTime);

  // DEVICE_BATTERY_UUID Characteristic
  BLECharacteristic* pCharacteristicBattery = new BLECharacteristic(DEVICE_BATTERY_UUID, BLECharacteristic::PROPERTY_WRITE_NR); //Request MTU=500 from client
  pCharacteristicBattery->setCallbacks(new DeviceBatteryCallback());
  pService->addCharacteristic(pCharacteristicBattery);

  pService->start();
     
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
  pAdvertising->setAppearance(GENERIC_DISPLAY); //Generic Display
  pAdvertising->start();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("hello");

  tft->initR(INITR_GREENTAB);   // initialize a ST7735S chip, black tab
  tft->initR(INITR_BLACKTAB);

  tft->fillScreen(ST77XX_WHITE);

  tft->drawRGBBitmap(14,30,splash,100,100);
  //init BLE server
  setupBLEServer();
  delay(5000);

  tft->drawRGBBitmap(14,30,blutooth_splash,100,100);
  
  //tft->fillScreen(ST77XX_BLACK);
  delay(5000);
}
void loop() {
}

