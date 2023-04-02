//.........................The Chemotron V3 source code built for the Chainlink Hackathon..............................................................................
//Author: Olufemi Victor Tolulope in Team Chemotronix
//Reach out on linkedIn https://www.linkedin.com/in/olufemi-victor-tolulope/

#include <ESP8266WiFi.h> //wifi library is needed
#include <Adafruit_ADS1X15.h> //For the ADC.
#include <Wire.h> // wire library
#include <OneWire.h> // one wire library
#include <DallasTemperature.h> //Temperature Sensor - open to plug in the one wire sensor
#include <WiFiClientSecure.h> // Secure client with SSL for https conversations
#include <ESP8266WebServer.h> // Manage conversations with the web server
#include <ESP8266HTTPClient.h> // Client that handles the requests
#include <NTPClient.h> // catch time from the NTP server
#include <WiFiUdp.h> // 
#include <ArduinoJson.h> // help handle JSON formatting

  


Adafruit_ADS1115 ads;     /* I'm Using thi for the 16-bit version */

//#include "CO2Sensor.h" // download co2sensor library from https://content.instructables.com/ORIG/FLJ/CVDP/K6HPDU8R/FLJCVDPK6HPDU8R.ino
#define MQ7_Digital D4 //Define port D4 for the MQ7 sensor
#define MQ9_Digital D5 //Define port D5 for the MQ9 sensor
#define MG811_Digital D6 //Just in case you'll like to check out digital values of the Co2 sensor
#define MQ5_Digital D7 //Define port D7 for the MQ5 sensor

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
//String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
//String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


//CO2Sensor co2Sensor(A0, 0.99, 10); // Use library to average sensor readings.


String apiKey = " "; // Enter your Write API key from ThingSpeak - Removed secrets
const char *ssid = "Professor"; // replace with your wifi ssid.
const char *pass = "Professor"; //replace with your wifi password
const char* server = "api.thingspeak.com"; // We're using the thingspeak server
const char *host = "api.web3.storage"; // Blockchain Host is web3 storage
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80 - keep this for conversations with https
const float volumetric_parameter = 0.00000000000485; // some const variable that help us estimate the volume in MT
const int watch_time = 3600; // This is the watch time in seconds where the IOT monitors until it pushes readings and starts over

//-----------------------------Setup SHA1 fingerprint for SSL/TLS decoding.....................................

//SHA1 finger print of certificate use web browser to view and copy
const char fingerprint[] PROGMEM = "17 E3 ED FF 61 0A 35 E0 A1 76 6D 7B 19 C1 12 6D 56 49 16 DC";



// Data wire is plugged into port D8 on the ESP8266 for the waterproof temperature sensor
#define oneWireBus D8


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature dsb_sensors(&oneWire);


// ...........................Connecting to WiFi.............................................................................................................................................

WiFiClient client;

void setup()
{
  Serial.begin(115200); // The Baud rate for reading serial monitor
  //while(!Serial) { } // Wait for serial to initialize. 
  
  delay(1000);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot to prevent hack ins

Wire.begin(D2, D1); // Define your clock pins

// Start the DS18B20 sensor
dsb_sensors.begin();

delay(500);
ads.begin();
Serial.println("=== ADS is Initialized ===");
delay(500);

pinMode(MQ5_Digital,INPUT); //Set Pins as Inputs
pinMode(MQ7_Digital,INPUT); //Set Pins as Inputs
pinMode(MQ9_Digital,INPUT); //Set Pins as Inputs
pinMode(MG811_Digital,INPUT);
delay(500);

Serial.println("Connecting to ");
Serial.println(ssid);

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600); // use WAT as the time or GMT+1

WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP());  //IP address assigned to your ESP
delay(500);

}

//.............................................READ SENSOR VALUES................................................................................................................................

//-------------------------- Analog readings ---------------------------------------------

void loop()
{
  
timeClient.update();
String formattedTime = timeClient.getFormattedTime();
Serial.print("Formatted Time: ");
Serial.println(formattedTime);

//Get a time structure
time_t epochTime = timeClient.getEpochTime();
struct tm *ptm = gmtime ((time_t *)&epochTime);

int monthDay = ptm->tm_mday;
int currentMonth = ptm->tm_mon+1;
int currentYear = ptm->tm_year+1900;
String formattedDate = String(monthDay) + "-" + String(currentMonth) +  "-" + String(currentYear) + " ";

Serial.print("Formatted Date: ");
Serial.println(formattedDate);

  
Serial.println("Sensor Caliberated Successfully");

// read your temperature
dsb_sensors.requestTemperatures(); 
float temperatureC = dsb_sensors.getTempCByIndex(0);


//delay(5000);
  int16_t adc0, adc1, adc2, adc3;
  adc0 = ads.readADC_SingleEnded(0); // MQ7 - CO
  
  adc1 = ads.readADC_SingleEnded(1); // MQ9 - CH4, CO
  adc2 = ads.readADC_SingleEnded(2); // MG811 - Co2
  adc3 = ads.readADC_SingleEnded(3); // MQ5 - CH4, CO
  delay(1000);

  float carbon_monoxide = (adc0/344); // calibrate carbon monoxide conc to stay in range
  float methane_inv = adc1/adc3; // calibrate inverted methane conc to stay in range
  float methane = adc3/adc1; // calibrate methane conc to stay in range
  float carbon_dioxide = adc2; //This guy doesn't need calibration

  float total_emissions_co2 = (watch_time+60)*volumetric_parameter*carbon_monoxide;
   

//------------------------------------------- Digital readings -------------------------------------------------------------------------------------------------------------------------

int mq5_sensor_digital = digitalRead(MQ5_Digital); //Digital value for CO sensor(MQ5)
delay(500);
int mq7_sensor_digital = digitalRead(MQ7_Digital); //Digital value for CO sensor(MQ7)
delay(500);
int mq9_sensor_digital = digitalRead(MQ9_Digital); //Digital value for CO sensor(MQ9)
delay(500);
int mg811_sensor_digital = digitalRead(MG811_Digital); //Digital value for CO sensor(MG811)
delay(500);


//--------------------------------- 

//if (isnan(val))
//{
//Serial.println("Failed to read from CO2 sensor!");
//return;
//}

//.......................................... Connecting to Thingspeak.......................................................................................................................
 
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey; //Make Sure You write To your Own API key.
postStr += "&field1=";
postStr += String(adc0);
postStr += "&field2=";
postStr += String(adc1);
postStr += "&field3=";
postStr += String(adc2);
postStr += "&field4=";
postStr += String(adc3);
postStr += "&field5=";
postStr += String(mq5_sensor_digital);
postStr += "&field6=";
postStr += String(mq7_sensor_digital);
postStr += "&field7=";
postStr += String(mq9_sensor_digital);
postStr += "&field8=";
postStr += String(mg811_sensor_digital);


client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

//......................................................SERIAL MONITORING.....................................................................................................................
Serial.print("Readings from the sensors: ");
Serial.println();
Serial.print(temperatureC);
Serial.println("ºC");
Serial.print("Readings from the Mq5 sensor: ");
Serial.println(mq5_sensor_digital);
Serial.print("raw Readings from the MQ7 sensor: ");
Serial.println(mq7_sensor_digital);
Serial.print("Digital Readings from the Mg811 sensor: ");
Serial.println(mg811_sensor_digital);
Serial.print("Digital Readings from the MQ9 sensor: ");
Serial.println(mq9_sensor_digital);
Serial.println("Data Send to Thingspeak");
Serial.print("AIN0 - MQ7: "); 
Serial.println(adc0);
Serial.print("AIN1 - MQ9: "); 
Serial.println(adc1);
Serial.print("AIN2 - MG811: "); 
Serial.println(adc2);
Serial.print("AIN3 - MQ5: "); 
Serial.println(adc3);
Serial.println(" ");
}
delay(500);
client.stop();
Serial.println("Waiting...");
 
// thingspeak needs minimum 15 sec delay between updates.
delay(16000);

//---------------------------------------------------Web 03 Storage Data Pushing-----------------------------------------------------------------------------------------------------//

  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient for https

  Serial.println(host); // Confirm the host for web03 storage as declared earlier

  Serial.printf("Using fingerprint '%s'\n", fingerprint); // print out the SHA 1 fingerprint
  httpsClient.setFingerprint(fingerprint); // give the SHA 1 finger print to the client
  httpsClient.setTimeout(15000); // use 30 Seconds as timeout, can be caused by delay or slow internet connectivity
  delay(1000);
  
  Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){ // we are trying maximum of 30 times before we call it a failed connection
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");  // if we get to 30, let's call it a failed connection and restart.
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  
  //POST Data
  Link = "/upload";      // endpoint for uploading the data

  Serial.print("requesting URL: ");
  Serial.println(host);
  

//------------------------------------------------------------- Current Post Request to web3 storage (JSON) --------------------------------------------------------------------------------------//

 DynamicJsonDocument content(2048);
 
 content["MQ7 (RAW)"] = adc0;
 content["MQ9(RAW)"]=adc1;
 content["MG811(RAW)"]=adc2;
 content["MQ5(RAW)"]=adc3;
 content["MQ5_SENSOR_DIGITAL(DISCRETE)"]=mq5_sensor_digital;
 content["MQ7_SENSOR_DIGITAL(DISCRETE)"]=mq7_sensor_digital;
 content["MQ9_SENSOR_DIGITAL(DISCRETE)"]=mq9_sensor_digital;
 content["MG811_SENSOR_DIGITAL(DISCRETE)"]=mg811_sensor_digital;
 content["CARBON MONOXIDE AVG CONC(PPM)"]=carbon_monoxide;
 content["CARBON DIOXIDE AVG CONC(PPM)"]=carbon_dioxide;
 content["METHANE AVG CONC(PPM)"]=methane;
 content["VOLUME OF CARBON(MT)"]=total_emissions_co2;
 content["IoT Time Stamp"] = formattedDate + formattedTime;


 // Serialize JSON document
 String content_json;
 serializeJson(content, content_json);

  httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: " + "\r\n" +
               "X-NAME:"+ formattedDate + formattedTime + " Chemotron x2" +"\r\n" +
               "Content-Type: application/json"+ "\r\n" +
               "Content-Length:" + measureJson(content) + "\r\n\r\n" +
               content_json + "\r\n" +
               "Connection: close\r\n\r\n");



//--------------------------------------------------------------------serial confirmation of Post------------------------------------------------------------------------------------




  Serial.println("request sent"); 
                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(20000);  //Take a deep breath for 2 seconds, you've done a lot

  // Deep sleep mode for 30 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
  Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
  ESP.deepSleep(watch_time * 1000000);


}
