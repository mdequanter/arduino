/*
 * rosserial Publisher Example
 * Prints "hello world!"
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
#include <ros.h>
#include <std_msgs/String.h>
#include <Servo.h>
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>


const char* ssid     = "xxxxxx";
const char* password = "xxxxxx";
// Set the rosserial socket server IP address
IPAddress server(192,168,0,13);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

ros::NodeHandle nh;
// Make a chatter publisher
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

Servo servo1;
Servo servo2;

void roverCallBack(const geometry_msgs::Twist& cmd_vel)
{

  if(cmd_vel.linear.x > 0 && cmd_vel.angular.z == 0)
  {
    // forward
    Serial.println("1");
    servo1.write(0);
    servo2.write(180);
  }
  else
  {
    if(cmd_vel.linear.x == 0 && cmd_vel.angular.z > 0)
    {
      // left
      servo1.write(0);
      servo2.write(0);
    }
    else
    {
      if(cmd_vel.linear.x == 0 && cmd_vel.angular.z == 0)
      {
        //stop
        servo1.write(90);
        servo2.write(90);
      }
      else
      {
        if(cmd_vel.linear.x == 0 && cmd_vel.angular.z < 0)
        {
          servo1.write(180);
          servo2.write(180);
        }
        else
        {
          if(cmd_vel.linear.x < 0 && cmd_vel.angular.z == 0)
          {
            servo1.write(180);
            servo2.write(0);
          }
          else
          {
            Serial.println("6");
          }
        }
      }
    }
  }
}


ros::Subscriber <geometry_msgs::Twist> sub("cmd_vel", roverCallBack);

// Be polite and say hello
char hello[13] = "hello world!";

void setup()
{ 
  // Use ESP8266 serial to monitor the process
  Serial.begin(115200);
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
