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
	packet->command = 1;
	packet->param = random(0, 100);
	strcpy(packet->value, "Ovo je poruka!");
	ET->sendData();
	delay(random(1000, 5000));
}
