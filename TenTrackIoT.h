#ifndef TenTrackIoT_h
#define TenTrackIoT_h
#include "Arduino.h"
#include "SimpleDHT.h"
#include "SPI.h"
#include "RFID.h"
#include "Accel32.h"

class TenTrackIoT {

    RFID rfid = RFID(21, 5); // SDA, RST
    Accel32 mpu = Accel32(17, 16); // SDA, SCL

    public:
        TenTrackIoT();
        void Blink(int input_pin, long input_interval);
        void InitDHT(int input_pin);
        bool IsPressed(int input_pin);
        bool IsMoved();
        bool Toggle();
        byte temperature = 0;
        byte humidity = 0;
        int ReadLDR(int input_pin);
        int ReadTemperature();
        int ReadHumidity();
        String ReadRFID();

    private:
        bool blinkState = 0;
        bool switchState = 0;
        bool previousState = 0;
        bool toggle = 0;
        int input_pin = 0;
        unsigned int serNum0 = 0;
        unsigned int serNum1 = 0;
        unsigned int serNum2 = 0;
        unsigned int serNum3 = 0;
        unsigned int serNum4 = 0;
        long input_interval = 1000;
        long previousMillis = 0;
        String idNumber = "";
        SimpleDHT11 dht11;
	
};

#endif
