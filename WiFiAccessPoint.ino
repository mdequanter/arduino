
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = "ESP8266ROBOT";
const char *password = "ESP8266ROBOT";

String left = "S";
String right= "S";
String power = "0";
int servospeed1 = 90;
int servospeed2 = 90;
String url;


#include "Servo.h"

int servo_pin1 = D6;  // for ESP8266 microcontroller
int servo_pin2 = D7;  // for ESP8266 microcontroller


Servo myservo1;
Servo myservo2;

int angle = 0;


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>ESP8266 Robot</h1><br><a href=?left=90&right=90>STOP MOTORS</a><br><a href=?left=0&right=180>FORWARD</a><br><a href=?left=180&right=0>BACKWARD</a><br><a href=?left=0&right=0>LEFT</a><br><a href=?left=180&right=180>RIGHT</a>");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  myservo1.attach(servo_pin1);
  myservo2.attach(servo_pin2);

}

void loop() {
  server.handleClient();

  if (server.hasArg("left") && server.hasArg("right")) {
      if (left!=server.arg("left") or right != server.arg("right")) {
        left = server.arg("left");
        right = server.arg("right");
        servospeed1 = left.toInt();
        servospeed2 = right.toInt();
        myservo1.write(servospeed1);
        myservo2.write(servospeed2);
        url = "<a href=?left=90&right=90>STOP</a>";
        server.send(200, "text/html", url);
      }     
    }

    delay (15);

}
