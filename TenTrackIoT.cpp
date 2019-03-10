#include "Arduino.h"
#include "TenTrackIoT.h"
#include "SimpleDHT.h"
#include "SPI.h"
#include "RFID.h"
#include "Accel32.h"

TenTrackIoT::TenTrackIoT() {
    SPI.begin();
    rfid.init();
    neo6m.begin(9600);
}

void TenTrackIoT::On(int input_pin) {
    if (pinModeOnOff == 0) {
        pinMode (input_pin, OUTPUT);
        pinModeOnOff = 1;
    }
    digitalWrite(input_pin, HIGH);
}

void TenTrackIoT::Off(int input_pin) {
    if (pinModeOnOff == 0) {
        pinMode (input_pin, OUTPUT);
        pinModeOnOff = 1;
    }
    digitalWrite(input_pin, LOW);
}

void TenTrackIoT::Blink(int input_pin, long input_interval) {
    if (pinModeBlink == 0) {
        pinMode (input_pin, OUTPUT);
        pinModeBlink = 1;
    }
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
    if (pinModeSwitch == 0) {
        pinMode (input_pin, INPUT);
        pinModeSwitch = 1;
    }
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

double TenTrackIoT::ReadLatitude() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > 10000) {
        latitude = 0.000000;
        previousMillis = currentMillis;
    }
    if (neo6m.available()) {
        gps.encode(neo6m.read());
        if (gps.location.isUpdated()) {
            latitude = gps.location.lat();
        }
    }
    return(latitude);
}

double TenTrackIoT::ReadLongitude() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > 10000) {
        longitude = 0.000000;
        previousMillis = currentMillis;
    }
    if (neo6m.available()) {
        gps.encode(neo6m.read());
        if (gps.location.isUpdated()) {
            longitude = gps.location.lng();
        }
    }
    return(longitude);
}
