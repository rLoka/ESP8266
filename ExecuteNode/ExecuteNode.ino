#include <EasyTransfer.h>
#include "sdk.h"

EasyTransfer ET;

struct PACKET {
	int command;
	int param;
	char value[100];
};

PACKET packet;

void setup() {
	Serial.begin(115200);
	ET.begin(details(packet), &Serial);
}

void loop() {
	if (ET.receiveData()) {
		Serial.println("Primljeni sljedeci podaci: ");
		Serial.println(packet.command);
		Serial.println(packet.param);
		Serial.println(packet.value);
	}
	delay(100);
}
