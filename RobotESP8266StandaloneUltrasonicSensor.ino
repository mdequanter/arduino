/*
 * rosserial Publisher Example
 * Install Library rosserial
 * Control robot via keyboard :  rosrun teleop_twist_keyboard teleop_twist_keyboard.py
 * messages are subscribed to "cmd_vel".
 * These commands are used in arduino. See code : esp8266RosServoControl.ino
 * Because servo's are mounted in mirror, you need to switch direction. So servo's run in same direction.
 * This intend to connect to a Wifi Access Point
 * and a rosserial socket server.
 * You can launch the rosserial socket server with
 * roslaunch rosserial_server socket.launch
 * The default port is 11411
 *
 */
 
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
 
#include <ESP8266WiFi.h>
#include <Servo.h>

const int trigPin = D2;
const int echoPin = D1;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;


const char* ssid     = "DINNO";
const char* password = "";

Servo servo1;
Servo servo2;

float distance = 1000;


void setup()
{ 
  // Use ESP8266 serial to monitor the process
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect the ESP8266 the the wifi AP
  /*
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
  
  servo1.attach(D6);
  servo2.attach(D7);

  servo1.write(90); //set servo angle, should be from 0-180  
  servo2.write(90); //set servo angle, should be from 0-180  

}


void loop()
{

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  if (distanceCm < 20.0) {
      servo1.write(90);
      servo2.write(90);
      delay(800);
      servo1.write(50);
      servo2.write(50);
      delay(800);
            
  } else {
      servo1.write(40);
      servo2.write(140);
  }

  delay(250);
}
