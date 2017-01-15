#include <EasyTransfer.h>
#include "sdk.h"

struct PACKET {
	int command;
	int param;
	char value[100];
};

EasyTransfer *ET;
PACKET *packet;

void setup() {
	packet = new PACKET;
	ET = new EasyTransfer;
	Serial.begin(115200);
	ET->begin(details(packet), &Serial);
}

void loop() {
	if (ET->receiveData()) {
		Serial.println("Primljeni sljedeci podaci: ");
		Serial.println(packet->command);
		Serial.println(packet->param);
		Serial.println(packet->value);
	}
	delay(100);
}
