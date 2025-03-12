#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <DFRobot_LcdDisplay.h>
#include <AccelStepper.h>

// **BLE UUID**
static BLEUUID serviceUUID("2bb8321b-7d5e-446b-a654-8e6fd81f1302");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

DFRobot_Lcd_IIC lcd(&Wire, 0x2C);
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
static boolean doConnect = false;
static boolean connected = false;

// **Stepper Motor Configuration** - Using working pin configuration
#define STEPS_PER_REV 1080  // X27.168 has 0.33° step angle = 1080 steps/rev
#define DEGREES_120 (STEPS_PER_REV / 3)  // Steps for 120 degrees rotation

// Use the FULL4WIRE mode with the pins from your working code
AccelStepper stepper(AccelStepper::FULL4WIRE, D8, D10, D0, D1);

// **State Management**
bool motorRunning = false;    // Track motor state
bool lastTouchState = false;  // Track previous touch state
bool motorDirection = true;   // Track direction (true = clockwise, false = counterclockwise)

enum DisplayState { BACK, LOADING, CARD, TEXT, CLEAR };
DisplayState currentState = CLEAR;  // Initial state is blank screen

// **Screen Parameters**
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define GIF_WIDTH   150
#define GIF_HEIGHT  150
#define TITLE_COLOR    0xFFFF
#define KEYWORD_COLOR  0xFFD0
#define SYMBOL_COLOR   0x07E0
#define TEXT_COLOR     0x0000
#define BORDER_COLOR   0x8410
#define CARD_WIDTH     280
#define CARD_HEIGHT    200
#define TEXT_SIZE      18

// **Screen Update Functions**
void displayBack() {
   lcd.cleanScreen();
   lcd.drawIcon(0, 0, "/tarot/back.png", 256);
   Serial.println("📺 Displaying Back Image");
}

void displayLoading() {
   lcd.cleanScreen();
   int16_t posX = (SCREEN_WIDTH - GIF_WIDTH) / 2;
   int16_t posY = (SCREEN_HEIGHT - GIF_HEIGHT) / 2;
   lcd.drawGif(posX, posY, "/tarot/loading3.gif", 0);
   Serial.println("📺 Displaying Loading Animation");
}

void displayCard() {
   lcd.cleanScreen();
   lcd.drawIcon(0, 0, "/tarot/card1.png", 256);
   Serial.println("📺 Displaying Card Image");
}

void displayText() {
   lcd.cleanScreen();
   lcd.drawRect(20, 10, CARD_WIDTH, CARD_HEIGHT, 2, BORDER_COLOR, 0, 0, 0);
   lcd.drawLine(30, 50, 290, 50, 2, BORDER_COLOR);
   lcd.drawString(50, 20, "PAGE ♦ PENTACLES", TEXT_SIZE, TITLE_COLOR);
   lcd.drawString(40, 60, "Growth | Learning", TEXT_SIZE, KEYWORD_COLOR);
   lcd.drawString(40, 85, "Prosperity | Practice", TEXT_SIZE, KEYWORD_COLOR);
   lcd.drawString(130, 110, "♀", 32, SYMBOL_COLOR);
   lcd.drawString(40, 140, "Start with confidence.", TEXT_SIZE, TEXT_COLOR);
   lcd.drawString(40, 165, "Every step adds value.", TEXT_SIZE, TEXT_COLOR);
   lcd.drawString(40, 190, "Growth takes time.", TEXT_SIZE, TEXT_COLOR);
   Serial.println("📺 Displaying Text Explanation");
}

void clearScreen() {
   lcd.cleanScreen();
   Serial.println("📺 Screen Cleared - Waiting for next touch");
}

// **Initialize stepper motor**
void setupStepper() {
   // Initialize stepper motor with settings from your working code
   stepper.setMaxSpeed(500.0);     // Max speed in steps per second
   stepper.setAcceleration(200.0); // Acceleration in steps per second^2
   stepper.setSpeed(200.0);        // Desired constant speed in steps per second
   Serial.println("🔄 Stepper motor initialized");
}

// **BLE class declaration** (needed before BLE callback)
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("🔍 BLE Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      Serial.println("✅ Target BLE device found!");
    }
  }
};

// **BLE Notification Callback**
void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
   String receivedData = String((char*)pData);
   Serial.print("🔹 Received: ");
   Serial.println(receivedData);

   if (receivedData == "Touched") {
       if (!lastTouchState || currentState == CLEAR) { 
           lastTouchState = true;
           motorRunning = true;
           currentState = BACK;
           displayBack();
           Serial.println("🔄 Motor STARTED!");
       }
   } else if (receivedData == "Untouched") {
       if (lastTouchState) {
           lastTouchState = false;
           motorRunning = false;

           // **Run display sequence**
           currentState = LOADING;
           displayLoading();
           delay(2000);

           currentState = CARD;
           displayCard();
           delay(3000);

           currentState = TEXT;
           displayText();
           delay(10000);

           currentState = CLEAR;
           clearScreen();
           Serial.println("⏹️ Motor STOPPED!");
       }
   }
}

// **BLE Connection**
class MyClientCallback : public BLEClientCallbacks {
   void onConnect(BLEClient* pclient) { Serial.println("✅ Connected to BLE Server"); }
   void onDisconnect(BLEClient* pclient) { connected = false; Serial.println("❌ Disconnected from BLE Server"); }
};

bool connectToServer() {
   Serial.print("🔗 Connecting to ");
   Serial.println(myDevice->getAddress().toString().c_str());

   BLEClient* pClient = BLEDevice::createClient();
   pClient->setClientCallbacks(new MyClientCallback());

   if (!pClient->connect(myDevice)) {
       Serial.println("❌ Connection failed");
       return false;
   }

   BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
   if (!pRemoteService) {
       Serial.println("❌ Service not found");
       pClient->disconnect();
       return false;
   }

   pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
   if (!pRemoteCharacteristic) {
       Serial.println("❌ Characteristic not found");
       pClient->disconnect();
       return false;
   }

   if (pRemoteCharacteristic->canNotify()) {
       pRemoteCharacteristic->registerForNotify(notifyCallback);
   }

   connected = true;
   return true;
}

// **Setup Function**
void setup() {
   Serial.begin(115200);
   delay(1000);  // Small delay to ensure Serial is ready
   
   lcd.begin();
   lcd.cleanScreen();
   lcd.drawString(20, 100, "Scanning BLE...", 24, 0xFFFF);
   
   setupStepper();
   
   BLEDevice::init("DisplayDevice");
   BLEScan* pBLEScan = BLEDevice::getScan();
   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
   pBLEScan->setInterval(1349);
   pBLEScan->setWindow(449);
   pBLEScan->setActiveScan(true);
   pBLEScan->start(5, false);
}

// **Main Loop**
void loop() {
   if (doConnect) {
       if (connectToServer()) {
           Serial.println("✅ Connected to Sensor!");
           lcd.cleanScreen();
           lcd.drawString(20, 100, "Connected!", 24, 0xFFFF);
       }
       doConnect = false;
   }

   if (!connected) {
       BLEDevice::getScan()->start(0);
   }

   // Handle stepper motor movement - 120 degree back and forth
   if (motorRunning) {
       // Move 120 degrees in one direction (1/3 of a full revolution)
       stepper.moveTo(DEGREES_120);
       while (stepper.distanceToGo() != 0) {
           stepper.run();  // Run the stepper until it reaches the target
       }
       delay(500);  // Short pause at the end of movement
       
       // Move back to the starting position
       stepper.moveTo(0);
       while (stepper.distanceToGo() != 0) {
           stepper.run();  // Run the stepper until it reaches the target
       }
       delay(500);  // Short pause at the end of movement
   }

   delay(100);
}
