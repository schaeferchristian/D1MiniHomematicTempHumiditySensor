#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

// DHT22:   PIN 1:  VCC     mit 3,3 oder 5V verbinden (funzte beides)
//          PIN 2:  Data    mit D2 oder D6 funktioniert es, an D8 auch aber dann lässt sich der ESP8266 nicht mehr flashen. Spannungsteiler mit 10K Widerstand
//                          Siehe Zeichnung wemos_d1_mini_dht22_bb.jpg im Verzeichnis dieses Skriptes
//          PIN 3:  NC      bleibt unbelegt
//          PIN 4:  GND     mit Masse / G verbinden

// Typ des Sensors, hier DHT22
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Zugangsdaten zum WLAN:
const char* ssid = "YOUR_WIFI_SSD_HERE";
const char* password = "YOUR_WIFI_PASSWORD_HERE";

// DeviceName
const char wiFiHostname[ ] = "DEFINE_A_CUSTOM_NAME_FOR_YOUR_D1_MINI_HERE";

// Homematic Base config
const char* baseIp = "ADD_THE_IP_OF_YOUR_CCU/RASPBERRYMATIC_HERE";
// Homematic systemVariables - ise_id
const int homematicTempVarId = ADD_THE_ISE_ID_OF_YOUR_SYSTEM_VAR_HERE;
const int homematicHumidityVarId = ADD_THE_ISE_ID_OF_YOUR_SYSTEM_VAR_HERE;

// DHT Sensor
// PIN mit dem "Data" des DHT22 verbunden ist
uint8_t dhtPin = D2; 

// Initialize DHT sensor.
DHT dht(dhtPin, DHTTYPE); 

void setup() {

  Serial.begin(115200);

  pinMode(dhtPin, INPUT);

  dht.begin();

  WiFi.hostname(wiFiHostname);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion

    delay(500);

  }

}

void loop() {

 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   // Temperature
   float temperature = dht.readTemperature(); // Gets the values of the temperature
   logCurrentValue("Temperature", temperature);
   postValueToServer("Temperature",temperature,homematicTempVarId);
   
  // Humidity
   float humidity = dht.readHumidity(); // Gets the values of the temperature
   logCurrentValue("Humidity", humidity);
   postValueToServer("Humidity",humidity,homematicHumidityVarId);

 }else{
    Serial.println("NO WIFI CONNECTION");
 }

  delay(30000);  //Send a request every 30 seconds

}

String assemblePostUrl(float value, int homematicSysVarId){
   String url = "http://";
   url += baseIp;
   url += "/addons/xmlapi/statechange.cgi?ise_id=";
   url += homematicSysVarId;
   url += "&new_value=";
   url += value;
   return url;
}

void logCurrentValue(String valueName, float value){
  String logMsg = "Current ";
   logMsg += valueName;
   logMsg += ": ";
   logMsg += value;
   Serial.println(logMsg);
}

void postValueToServer(String valueName, float value, int homematicSysVarId){
  HTTPClient http;    //Declare object of class HTTPClient
  
   String logMsg = "Sending ";
   logMsg += valueName;
   logMsg += " ...";
   Serial.println(logMsg);
   http.begin(assemblePostUrl(value,homematicSysVarId));
   http.addHeader("Content-Type", "text/plain");
   String postMsg = valueName;
   postMsg += " from ";
   postMsg += wiFiHostname;
   int httpCode = http.POST(postMsg); //Send the request
   String payload = http.getString(); //Get the response payload
   http.end();  //Close connection
   
   Serial.println("Server-Response: ");
   Serial.println(payload);
}
