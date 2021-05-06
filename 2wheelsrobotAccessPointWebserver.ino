/*
*  Source code for uploading to your ESP8266.
*  This program shows will provide a wifi access point and host a simple web page.  
*  When you connect to the accesspoint "ESP8266ROBOT" with password "ESP8266ROBOT", you can surf to the controllers page.
*  Your wifi card will get an IP and a gateway address (ip of the accesspoint).  Browse to http://192.168.4.1 (ip accesspoint)
*  By passing left and right in the URL you can drive 2 servo motors.
*  A standard hobby servo(360 degrees) will run forward for values between 0 and 90, and backward for values between 90 and 180.  When you enter
*  90 the servo will stop.
*  example :  http://192.168.1.4/?left=0&right=0 (servo left and right full speed forward)
*  example :  http://192.168.1.4/?left=180&right=180 (servo left and right full speed backward)
*  example :  http://192.168.1.4/?left=90&right=90 (servo left and right stop)
*  example :  http://192.168.1.4/?left=45&right=10 (servo left half speed and right very slow speed)
*/



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
      }     
    }

    delay (15);

}
