/*  Arduino DC Motor Control - PWM | H-Bridge | L298N
    by Maarten Dequanter
    controls 2 motors by using Hual H-Bridge vI.3 board
    Connect E1 to pin 6,  M1 to 7,  E2 to 5 and M2 to 4
    Connect 2 resistors of 1MOhm or more to VS and ground in serie.  In the middle you should have the voltage devided by to and read by Analog input A0
    Connect 2 motors to Motor connectors
    Connect VD to 5V pin on arduino and GND to GND pin on Arduino.

    The motors should start running and will stop as soon the voltage is less then the value in lowValue.

    Add your own logic to drive PWM signals to in1 and in3 and control direction with in2 and in4.
   
*/

#define in1 6
#define in2 7
#define in3 5
#define in4 4
#define led 12
#define button 4
int rotDirection = 0;
int speed = 0;
int lowBattery = 0;
float MaxVoltage = 7.8;
float lowVoltage = MaxVoltage*0.8;


void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode (led, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(led, HIGH);

}
void loop() {

  
  if (lowBattery == 0 ) {
    for (int i=50;i<150;i++) {
      analogWrite(in1, i); // Send PWM signal to L298N Enable pin
      analogWrite(in3, i); // Send PWM signal to L298N Enable pin
      i=i+10;
      delay(500);    
    }
  } else {
      analogWrite(in1, 0); // Send PWM signal to L298N Enable pin
      analogWrite(in3, 0); // Send PWM signal to L298N Enable pin    
  }
  delay (2000);
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0)*2;
  Serial.println(voltage);
  
  if (voltage <= lowVoltage) {
    lowBattery = 1;
  }

  if (lowBattery ==1 ) {
    digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
  }
  if (voltage >= lowVoltage*1.1) {
    lowBattery = 0;
  }
  
  
  

}
