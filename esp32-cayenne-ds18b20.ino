// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
float temperatureC = 0.0;
// WiFi network info.
char ssid[] = ""; //aqui va tu ssid
char wifiPassword[] = ""; //aqui va tu pass

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "";
char password[] = "";
char clientID[] = "";

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(115200);
 sensors.begin(); //para dallas 18d20
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();
  /// zona ds18d20
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
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
