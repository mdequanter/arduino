#include <Adafruit_NeoPixel.h>

const int NR_OF_PIXELS = 5;
const int PIN = 3;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NR_OF_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
Serial.begin(9600);
pinMode(PIN, OUTPUT);
strip.begin();
strip.show();
}

void loop() {
for (int i = 0; i < NR_OF_PIXELS; i++)
{
strip.setPixelColor(i, 255, 0, 0); //Zet pixel i op RGB waarde 255, 0, 0
strip.show(); //voor de aanpassing door naar de strip
delay(200);


/* for (int j = 0; j <= 255; j+=50)/*
/* {
for (int k = 0; k <= 255; k+=50)
{

 for (int l = 0; l <= 255; l+=50)
{
strip.setPixelColor(i, j, k, l); //Zet pixel i op RGB waarde 255, 0, 0
strip.show(); //voor de aanpassing door naar de strip
delay(50);
}
}
}*/
strip.setPixelColor(i, 255, 0, 0); //Zet pixel i op RGB waarde 255, 0, 0
strip.show(); //voor de aanpassing door naar de strip
delay(50);

 }
strip.clear(); // zet alle pixels af
delay(1000);
}