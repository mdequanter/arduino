/*
 * Build for ESP8266.  
 * rosserial Publisher Example
 * Install Library rosserial
 * Control robot via keyboard :  rosrun teleop_twist_keyboard teleop_twist_keyboard.py
 * messages are subscribed to "cmd_vel".
 * Because servo's are mounted in mirror, you need to switch direction. So servo's run in same direction.
 * This intend to connect to a Wifi Access Point
 * and a rosserial socket server.
 * You can launch the rosserial socket server with
 * roslaunch rosserial_server socket.launch
 * The default port is 11411
 * Also the HCSR04 Ultrasonic sensor is build in.  The rover stops automatically when object is les the 10cm in front.
 *
 */
 
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
 
#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <Servo.h>
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>

const int trigPin = D2;
const int echoPin = D1;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;




const char* ssid     = "DINNO";
const char* password = "geheim123$";
// Set the rosserial socket server IP address
IPAddress server(192,168,68,75);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

ros::NodeHandle nh;
// Make a chatter publisher
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

Servo servo1;
Servo servo2;

float distance = 1000;




void roverCallBack(const geometry_msgs::Twist& cmd_vel)
{
  int compensateServo1 = 0;
  int compensateServo2 = 5;

  Serial.print("x:");
  Serial.print(cmd_vel.linear.x);
  Serial.print("y:");
  Serial.print(cmd_vel.linear.y);
  Serial.print("z:");
  Serial.print(cmd_vel.linear.z);



  if(cmd_vel.linear.x > 10.0  and distanceCm > 10.0)
  {
    // forward
    float speed =  floor (cmd_vel.linear.x);
    if (speed >=90.0) {
      speed = 90;
    }
      int valueServo1 = 90 - speed - compensateServo1;
      int valueServo2 = 90 + speed + compensateServo2;
      servo1.write(valueServo1);
      servo2.write(valueServo2);
  }

  if (cmd_vel.linear.x < -10.0)
  {
    // backward
    float speed =  abs(floor (cmd_vel.linear.x));
    if (speed >= 90.0) {
      speed = 90;
    }    
    int valueServo1 = 90 + speed + compensateServo1;
    int valueServo2 = 90 - speed - compensateServo2;
    servo1.write(valueServo1);
    servo2.write(valueServo2);
  }
 
  if (cmd_vel.angular.z > 0.0  and distanceCm > 10.0) {
    float speed =  floor (cmd_vel.angular.z);
    if (speed >=90.0) {
      speed = 90;
    }

    int valueServo1 = 90 - speed - compensateServo1;
    int valueServo2 = 90 - speed - compensateServo2;
    servo1.write(valueServo1);
    servo2.write(valueServo2);
  }    
  
  if (cmd_vel.angular.z < 0.0 and distanceCm > 10.0 ) {
    float speed =  abs(floor (cmd_vel.angular.z));
    if (speed >=90.0) {
      speed = 90;
    }

    int valueServo1 = 90 + speed + compensateServo1;
    int valueServo2 = 90 + speed + compensateServo2; 
    servo1.write(valueServo1);
    servo2.write(valueServo2);
  }    

    if ((cmd_vel.linear.x > -10  and  cmd_vel.linear.x < 10))
  {
      servo1.write(90);
      servo2.write(90);  
  }



}


ros::Subscriber <geometry_msgs::Twist> sub("cmd_vel", roverCallBack);

// Be polite and say hello
char hello[13] = "hello world!";

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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // Start to be polite
  nh.advertise(chatter);
  nh.subscribe(sub);

  servo1.attach(D6);
  servo2.attach(D7);

  servo1.write(90); //set servo angle, should be from 0-180  

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

  if (distanceCm < 10.0) {
      servo1.write(90);
      servo2.write(90);      
  }

  if (nh.connected()) {
    Serial.println("Connected");
    // Say hello
    str_msg.data = hello;
    chatter.publish( &str_msg );
  } else {
    Serial.println("Not Connected");
  }
  nh.spinOnce();
  // Loop exproximativly at 1Hz
  delay(1000);
}
