// # Editor     : roker
// # Date       : 05.03.2018

// # Product name: URM V5.0 ultrasonic sensor
// # Product SKU : SEN0001
// # Version     : 1.0

// # Description:
// # The sketch for using the URM37 autonomous  mode from DFRobot
// #   and writes the values to the serialport


// # Connection:
// #       Vcc (Arduino)      -> Pin 1 VCC (URM V5.0)
// #       GND (Arduino)      -> Pin 2 GND (URM V5.0)
// #       Pin 3 (Arduino)    -> Pin 4 ECHO (URM V5.0)
// #       Pin TX1 (Arduino)  -> Pin 8 RXD (URM V5.0)
// #       Pin RX0 (Arduino)  -> Pin 9 TXD (URM V5.0)
// # Working Mode:  Automatic measurement model.

int URECHO = 3; // PWM Output 0－25000US，Every 50US represent 1cm

unsigned int Distance = 0;
uint8_t AutomaticModelCmd[4] = {0x44, 0x02, 0xaa, 0xf0}; // distance measure command

void setup() 
{                                
  Serial.begin(9600);           // Serial initialization           
  delay(5000);                   // wait for sensor setup
  AutomaticModelSetup();        //Automatic measurement model set
}

void loop()
{
  AutomaticMeasurement();
  delay(100);
}                      


void AutomaticModelSetup(void)
{
  pinMode(URECHO, INPUT);                      
  for (int i = 0; i < 4; i++)
  {
    Serial.write(AutomaticModelCmd[i]);// Sending Automatic measurement model command
  }
}


void AutomaticMeasurement(void)
{
  unsigned long DistanceMeasured = pulseIn(URECHO, LOW);
  if (DistanceMeasured >= 50000) // the reading is invalid.
  {
    Serial.print("Invalid");
  }
  else
  {
    Distance = DistanceMeasured / 50;       // every 50us low level stands for 1cm
    Serial.print("Distance=");
    Serial.print(Distance);
    Serial.println("cm");
  }
}
