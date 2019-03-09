#include "Arduino.h"
#include "TenTrackIoT.h"
#include "SimpleDHT.h"
#include "SPI.h"
#include "RFID.h"
#include "Accel32.h"

TenTrackIoT::TenTrackIoT() {
    SPI.begin();
    rfid.init();
}

void TenTrackIoT::Blink(int input_pin, long input_interval) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > input_interval) {
        if (blinkState == 0) {
            digitalWrite(input_pin, HIGH);
            blinkState = 1;
        } else {
            digitalWrite(input_pin, LOW);
            blinkState = 0;
        }
        previousMillis = currentMillis;
    }
}

bool TenTrackIoT::IsPressed(int input_pin) {
    bool switchPressed = false;
	unsigned long currentMillis = millis();
	unsigned int currentState = digitalRead(input_pin);
	if (currentMillis - previousMillis > 100) {
		if (currentState == 1 && previousState == 0) {
			switchPressed = true;
			previousState = currentState;
		}
        if (currentMillis - previousMillis > 1000) {
            previousState = 0;
            previousMillis = currentMillis;
        }
	}
	return(switchPressed);
}

bool TenTrackIoT::Toggle() {
    if (toggle == 0) {
        toggle = 1;
    } else {
        toggle = 0;
    }
    return(toggle);
}

int TenTrackIoT::ReadLDR(int input_pin) {
    return(analogRead(input_pin));
}

void TenTrackIoT::InitDHT(int input_pin) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > 1000) {
        dht11.read(input_pin, &temperature, &humidity, NULL);
        previousMillis = currentMillis;
    }
}

int TenTrackIoT::ReadTemperature() {
    return((int)temperature);
}

int TenTrackIoT::ReadHumidity() {
    return((int)humidity);
}

String TenTrackIoT::ReadRFID() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > 100) {
        idNumber = "";
        previousMillis = currentMillis;
    }
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            idNumber = String(rfid.serNum[0], HEX) + String(rfid.serNum[1], HEX) + String(rfid.serNum[2], HEX) + String(rfid.serNum[3], HEX) + String(rfid.serNum[4], HEX);
            idNumber.toUpperCase();
        }
    }
    rfid.halt();
    return(idNumber);
}

bool TenTrackIoT::IsMoved() {
    return(mpu.Detected());
}
