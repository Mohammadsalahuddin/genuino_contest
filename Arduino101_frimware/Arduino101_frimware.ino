/*******************************************************************
* The Arduino 101 frimware for IoT based Patient Monitoring System *	
*        @author: Mohammad Salah Uddin 				   *
*        @email: akash.bangla@gmail.com				   *
*        Last update : 21 March 2017				   *
*								   *
********************************************************************
*/

#include <SPI.h>
#include <Ethernet.h>
#include <ThingerEthernet.h>
#include <MAX30100_PulseOximeter.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


//Thinger.io Credential (please put your credentials)
#define USERNAME "YOUR_USER_NAME"
#define DEVICE_ID "YOUR_DEVICE-ID"
#define DEVICE_CREDENTIAL "YOUR_DEVICE_CREDENTIAL"

//VARIABLE TO HOLD THE SENSORS DATA
int bpm;
int spo2;
float temp;

//the sea level presure in your region (****)
#define SEALEVELPRESSURE_HPA (1023)
Adafruit_BME280 bme; 		// BME280  Sensnor declaration 
unsigned long currentMillis;   //hold the current time



//pulse oximeter time period (measurment time period)
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

//pulse sensor callback function
void measured_pulse(){
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      bpm=pox.getHeartRate();
      tsLastReport = millis();
    }
    
}

//initialize the thinger.io connection including Ethernet Connection
//(make sure that your DHCP server is enable)
ThingerEthernet thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);


void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 test"));
  Serial.println("Initializing MAX30100");
  // Initialize the PulseOximeter instance and register a beat-detected callback
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);
  
  bool status;
  // default settings
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  pinMode(7, OUTPUT);
  pinMode(A0,INPUT);
  pinMode(8,INPUT);
  pinMode(6,INPUT);

  // pin control example (i.e. turning on/off a light, a relay, etc)
  thing["led"] << digitalPin(7);
  
  
  // resource output example (i.e. reading a sensor value, a variable, etc)
  // push sensors data to IoT cloud (please refer the Thinger.io Documentation)
  thing["millis"] >> outputValue(millis());
  thing["h_rate"] >> outputValue(analogRead(A0));
  thing["button"] >> outputValue(digitalRead(8));
  thing["temperature"] >> [](pson& out){out = pox.getTemperature();};
  thing["spo2"] >> [](pson& out){out= pox.getSpO2();};
  thing["bpm"] >> [](pson& out){out =  bpm ;};
  thing["humidity"] >> [](pson& out){out =  bme.readHumidity() ;};
  thing["movement"] >> [](pson& out){out = digitalRead(6) ;};
 
}

void loop() {
  thing.handle();
  currentMillis = millis();
  measured_pulse();
  
}
