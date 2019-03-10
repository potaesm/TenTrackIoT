#ifndef TenTrackIoT_h
#define TenTrackIoT_h
#include "Arduino.h"
#include "SimpleDHT.h"
#include "SPI.h"
#include "RFID.h"
#include "Accel32.h"
#include "TinyGPS++.h"

class TenTrackIoT {

    RFID rfid = RFID(21, 22); // SDA, RST for RFID
    Accel32 mpu = Accel32(18, 19); // SDA, SCL for MPU6050
    HardwareSerial neo6m = HardwareSerial(2);  // UART2 16 (RX for ESP32), 17 (TX for ESP32)

    public:
        TenTrackIoT();
        void On(int input_pin);
        void Off(int input_pin);
        void Blink(int input_pin, long input_interval);
        void InitDHT(int input_pin);
        bool IsPressed(int input_pin);
        bool IsMoved();
        bool Toggle();
        int ReadLDR(int input_pin);
        int ReadTemperature();
        int ReadHumidity();
        double ReadLatitude();
        double ReadLongitude();
        String ReadRFID();

    private:
        bool blinkState = 0;
        bool pinModeOnOff = 0;
        bool pinModeBlink = 0;
        bool pinModeSwitch = 0;
        bool switchState = 0;
        bool previousState = 0;
        bool toggle = 0;
        byte temperature = 0;
        byte humidity = 0;
        int input_pin = 0;
        unsigned int serNum0 = 0;
        unsigned int serNum1 = 0;
        unsigned int serNum2 = 0;
        unsigned int serNum3 = 0;
        unsigned int serNum4 = 0;
        long input_interval = 1000;
        long previousMillis = 0;
        double latitude = 0.000000;
        double longitude = 0.000000;
        String idNumber = "";
        SimpleDHT11 dht11;
        TinyGPSPlus gps;
	
};

#endif
