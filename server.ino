#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define TOUCH_SENSOR_PIN D3  // 触摸传感器引脚
#define SERVICE_UUID        "2bb8321b-7d5e-446b-a654-8e6fd81f1302"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool lastTouchState = false;
unsigned long lastTouchTime = 0;
const int debounceDelay = 200;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        BLEDevice::startAdvertising();
    }
};

void setup() {
    Serial.begin(115200);
    pinMode(TOUCH_SENSOR_PIN, INPUT);

    BLEDevice::init("TouchSensor");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    pCharacteristic->addDescriptor(new BLE2902());
    pService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();

    Serial.println("BLE Sensor Server Ready!");
}

void loop() {
    bool touchState = digitalRead(TOUCH_SENSOR_PIN) == HIGH;

    if (deviceConnected && touchState != lastTouchState) {
        lastTouchState = touchState;
        lastTouchTime = millis();

        if (touchState) {
            Serial.println("Touch detected!");
            pCharacteristic->setValue("Touched");
            Serial.println("Sent 'Touched' to Client");
        } else {
            Serial.println("Hand removed!");
            pCharacteristic->setValue("Untouched");
            Serial.println("Sent 'Untouched' to Client");
        }

        pCharacteristic->notify();
    }

    delay(50);
}
