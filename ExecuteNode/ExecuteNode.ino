#include <EasyTransfer.h>
#include "ssidflood.h"

struct PACKET {
	int command;
	int param;
	char value[100];
};

EasyTransfer ET;
PACKET packet;

void ledSignal() {
	digitalWrite(BUILTIN_LED, LOW);
	delay(100);
	digitalWrite(BUILTIN_LED, HIGH);
	delay(100);
}

void setup() {
	Serial.begin(115200);
	ET.begin(details(packet), &Serial);

	//Inicijalizacija ugrađene LEDice, bit će korištena za signalizaciju komunikacije
	pinMode(BUILTIN_LED, OUTPUT);
	digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
	if (ET.receiveData()) {
		Serial.println("Primljeni sljedeci podaci: ");
		Serial.println(packet.command);
		Serial.println(packet.param);
		Serial.println(packet.value);
		ledSignal();

		switch (packet.command) {
				case 0:
					break;
				case 1:
					digitalWrite(BUILTIN_LED, LOW);
					if(SSIDFlood::startFlooding(packet.param)){
						Serial.println("Zavrseno floodanje!");
					}
					digitalWrite(BUILTIN_LED, HIGH);
					break;
				}
	}
	delay(100);
}
