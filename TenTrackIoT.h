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
    Accel32 mpu = Accel32(25, 26); // SDA, SCL for MPU6050
    HardwareSerial neo6m = HardwareSerial(2);  // UART2 16 (RX for ESP32), 17 (TX for ESP32)

    public:
        TenTrackIoT();
        void On(int on_input_pin);
        void Off(int off_input_pin);
        void Blink(int blink_input_pin, long input_interval);
        void InitDHT(int dht_input_pin);
        bool IsPressed(int switch_input_pin);
        bool IsMoved();
        bool Toggle();
        int ReadLDR(int ldr_input_pin);
        int ReadTemperature(int dht_input_pin);
        int ReadHumidity(int dht_input_pin);
        double ReadLatitude();
        double ReadLongitude();
        String ReadRFID();

    private:
        bool blinkState = 0;
        bool pinModeOn = 0;
        bool pinModeOff = 0;
        bool pinModeLdr = 0;
        bool pinModeBlink = 0;
        bool pinModeSwitch = 0;
        bool switchState = 0;
        bool previousState = 0;
        bool toggle = 0;
        byte temperature = 0;
        byte humidity = 0;
        int on_input_pin = 0;
        int off_input_pin = 0;
        int blink_input_pin = 0;
        int dht_input_pin = 0;
        int ldr_input_pin = 0;
        int ldr_value = 0;
        int switch_input_pin = 0;
        unsigned int serNum0 = 0;
        unsigned int serNum1 = 0;
        unsigned int serNum2 = 0;
        unsigned int serNum3 = 0;
        unsigned int serNum4 = 0;
        long input_interval = 1000;
        long blink_previousMillis = 0;
        long switch_previousMillis = 0;
        long dht_previousMillis = 0;
        long rfid_previousMillis = 0;
        long latitude_previousMillis = 0;
        long longitude_previousMillis = 0;
        double latitude = 0.000000;
        double longitude = 0.000000;
        String idNumber = "";
        SimpleDHT11 dht11;
        TinyGPSPlus gps;
	
};

#endif
