#ifndef C2PANEL_H_
#define C2PANEL_H_

#include <ESP8266WiFi.h>
#include "sdk.h"

namespace C2Panel {

struct PACKET {
	int command;
	int param;
	char value[250];
};

char *html =
		"<!doctype html><html><head> <title>C&C SiSB&#9760;X Panel</title> <link rel='stylesheet' href='https://unpkg.com/purecss@0.6.2/build/pure-min.css' integrity='sha384-UQiGfs9ICog+LwheBSRCt1o5cbyKIHbwjWscjemyBMT9YCUMZffs6UqUTd0hObXD' crossorigin='anonymous'> <meta name='viewport' content='width=device-width, initial-scale=1'> <link rel='stylesheet' href='https://unpkg.com/purecss@0.6.2/build/base-min.css'> <meta charset='UTF-8'> <style>body{max-width: 600px; margin: 0 auto; background-color: #878a8f;}header{padding:10px; background-color: #3a3a3c; color: white; text-align: center;}article{background-color: white;}.node{padding: 10px; border: 4px solid #36373a;}.button-warning{background: rgb(223, 67, 46); /* this is an orange */ font-weight: bold; color: white;}</style></head><body><header> <h1>C&C SiSB&#9760;X Panel</h1></header><article> <div class='node'> <h3>SSID beacon flooding</h3> <form class='pure-form' action='flood'> Trajanje u sekundama: <input type='number' name='duration' value='25'> <br><button type='submit' class='button-warning pure-button'>Pokreni</button> </form> </div></article></body></html>";
WiFiServer server(80);
WiFiClient client;

void startServer() {
	server.begin();
}

void stopServer() {
	server.stop();
}

PACKET requestHandler() {
	// Check if a client has connected
	PACKET packet;
	client = server.available();
	if (!client) {
		packet.command = -1;
		return packet;
	}

	// Wait until the client sends some data
	Serial.println("Novi klijent na C2C panelu!");
	while (!client.available()) {
		delay(1);
	}

	// Read the first line of the request
	String request = client.readStringUntil('\r');
	Serial.println(request);
	client.flush();

	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println(""); //  do not forget this one

	if (request.indexOf("/flo") != -1) {
		int numberOfSeconds = request.substring(request.indexOf("=") + 1, request.indexOf(" HTTP")).toInt();
		client.println(html);
		delay(1);
		packet.command = 1;
		packet.param = numberOfSeconds;

	} else {
		client.println(html);
		delay(1);
		packet.command = 0;
	}

	return packet;

}
}

#endif
