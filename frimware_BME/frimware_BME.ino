//Flash this frimware on arduino 101
#include <SPI.h>
#include <Ethernet.h>
#include <ThingerEthernet.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//the sea level presure in your region (ROME)
#define SEALEVELPRESSURE_HPA (1023)
Adafruit_BME280 bme; // BME280  declaration 

#define USERNAME "YOUR_USER_NAME"
#define DEVICE_ID "YOUR_DEVICE-ID"
#define DEVICE_CREDENTIAL "YOUR_DEVICE_CREDENTIAL"

float temp;
int humi;
//the sea level presure in your region (ROME)
#define SEALEVELPRESSURE_HPA (1023)
unsigned long currentMillis;   //hold the current time




ThingerEthernet thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 test"));

  bool status;
  // default settings
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  
  
  
  // resource output example (i.e. reading a sensor value, a variable, etc)
  thing["temperature"] >> [](pson& out){out = bme.readTemperature();};
  thing["humidity"] >> [](pson& out){out =  bme.readHumidity() ;};
}

void loop() {
  thing.handle();
  currentMillis = millis();
  Serial.println(bme.readTemperature());
  
}
