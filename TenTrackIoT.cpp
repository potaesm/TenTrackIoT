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

void TenTrackIoT::On(int on_input_pin) {
    if (pinModeOn == 0) {
        pinMode (on_input_pin, OUTPUT);
        pinModeOn = 1;
    }
    digitalWrite(on_input_pin, HIGH);
}

void TenTrackIoT::Off(int off_input_pin) {
    if (pinModeOff == 0) {
        pinMode (off_input_pin, OUTPUT);
        pinModeOff = 1;
    }
    digitalWrite(off_input_pin, LOW);
}

void TenTrackIoT::Blink(int blink_input_pin, long input_interval) {
    if (pinModeBlink == 0) {
        pinMode (blink_input_pin, OUTPUT);
        pinModeBlink = 1;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - blink_previousMillis > input_interval) {
        if (blinkState == 0) {
            digitalWrite(blink_input_pin, HIGH);
            blinkState = 1;
        } else {
            digitalWrite(blink_input_pin, LOW);
            blinkState = 0;
        }
        blink_previousMillis = currentMillis;
    }
}

bool TenTrackIoT::IsPressed(int switch_input_pin) {
    if (pinModeSwitch == 0) {
        pinMode (switch_input_pin, INPUT);
        pinModeSwitch = 1;
    }
    bool switchPressed = false;
	unsigned long currentMillis = millis();
	unsigned int currentState = digitalRead(switch_input_pin);
	if (currentMillis - switch_previousMillis > 100) {
		if (currentState == 1 && previousState == 0) {
			switchPressed = true;
			previousState = currentState;
		}
        if (currentMillis - switch_previousMillis > 1000) {
            previousState = 0;
            switch_previousMillis = currentMillis;
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

int TenTrackIoT::ReadLDR(int ldr_input_pin) {
    if (pinModeLdr == 0) {
        pinMode (ldr_input_pin, INPUT);
        pinModeLdr = 1;
    }
    ldr_value = analogRead(ldr_input_pin);
    return(ldr_value);
}

void TenTrackIoT::InitDHT(int dht_input_pin) {
    unsigned long currentMillis = millis();
    if (currentMillis - dht_previousMillis > 1000) {
        dht11.read(dht_input_pin, &temperature, &humidity, NULL);
        dht_previousMillis = currentMillis;
    }
}

int TenTrackIoT::ReadTemperature(int dht_input_pin) {
    TenTrackIoT::InitDHT(dht_input_pin);
    return((int)temperature);
}

int TenTrackIoT::ReadHumidity(int dht_input_pin) {
    TenTrackIoT::InitDHT(dht_input_pin);
    return((int)humidity);
}

String TenTrackIoT::ReadRFID() {
    unsigned long currentMillis = millis();
    if (currentMillis - rfid_previousMillis > 100) {
        idNumber = "";
        rfid_previousMillis = currentMillis;
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
    if (currentMillis - latitude_previousMillis > 10000) {
        latitude = 0.000000;
        latitude_previousMillis = currentMillis;
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
    if (currentMillis - longitude_previousMillis > 10000) {
        longitude = 0.000000;
        longitude_previousMillis = currentMillis;
    }
    if (neo6m.available()) {
        gps.encode(neo6m.read());
        if (gps.location.isUpdated()) {
            longitude = gps.location.lng();
        }
    }
    return(longitude);
}
