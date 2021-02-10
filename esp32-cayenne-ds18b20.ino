// This example shows how to connect to Cayenne using an ESP12F or NODEMCU v3 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <OneWire.h>
#include <DallasTemperature.h>


const int oneWireBus = 2;     //pero en la placa ESP32 puede aparecer como pin D4 por las diferentes implementaciones 
// WiFi network info.
OneWire oneWire(oneWireBus);
char ssid[] = "";
char wifiPassword[] = "";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "";
char password[] = "";
char clientID[] = "";
DallasTemperature sensors(&oneWire);
float temperatureC = 0.0;
const int outputLED = LED_BUILTIN;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
   sensors.begin(); //para dallas 18d20
  pinMode(outputLED, OUTPUT); //defino como salida el led
  pinMode(16,OUTPUT);
}

void loop() {
  Cayenne.loop();
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);  
  Serial.print(temperatureC);
  Serial.println("ºC");
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  //Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  Cayenne.celsiusWrite(1, temperatureC);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.

// Canal dos para controlar el LEDBUILTIN
CAYENNE_IN(2)
{
  
  CAYENNE_LOG("Got a value: %s", getValue.asStr());
  int i = getValue.asInt();
  
  if (i == 0)
  {
    Serial.println(i);
    digitalWrite(outputLED, HIGH);
  }
  else
  {
    Serial.println(i);
    digitalWrite(outputLED, LOW);
  }  
  
  
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(4) //canal 4 para digital 16 conectado a D0
{
  
  CAYENNE_LOG("Got a value: %s", getValue.asStr());
  int i = getValue.asInt();
  
  if (i == 0)
  {
    Serial.println(i);
    digitalWrite(16, HIGH);
  }
  else
  {
    Serial.println(i);
    digitalWrite(16, LOW);
  }  
  
  
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
