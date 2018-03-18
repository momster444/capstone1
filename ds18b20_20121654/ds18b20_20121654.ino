#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define ONE_WIRE_BUS D4   

//Time interval between measuring temperature.
int interval = 5000;   // Five Second

// Information to connect private Wifi
const char* ssid     = "chuwifi";
const char* password = "csw20121660";

/*// Information to connect ThingSpeak
const char* host     = "api.thingspeak.com";
String url           = "/update?api_key=I692XH2ONBYAB3LU";   // Your Own Key here
const int httpPort   = 80;
*/

const char* host     = "maker.ifttt.com";
String url           = "/trigger/temperature/with/key/dfvQ0u898ppwpzzx778fD-";   // Your Own Key here
const int httpPort   = 80;

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


/***************************Program Code*****************************/
String working() { 
  double temperature;
  
  Serial.print("Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.println("DONE"); 
  
  Serial.print("Temperature is: "); 
  temperature=sensors.getTempCByIndex(0);   
  // Why "byIndex"? 
  // 0 refers to the first IC on the wire 
  // You can have more than one DS18B20 on the same bus.  
  Serial.println(temperature);  
  return String(temperature);
}

void delivering(String payload) { 
  WiFiClient client;
  Serial.print("\nconnecting to Host: ");
  Serial.println(host);

  //Connect to ThingSpeak Server
  if (client.connect(host, httpPort)) {
    //http call to server by using GET Method.
    //String getheader = "GET "+ String(url) +"&field1="+ String(payload) +" HTTP/1.1";
    String getheader = "GET "+ String(url) +"?value1="+ String(payload) +" HTTP/1.1";
    client.println(getheader);
    client.println("User-Agent: ESP8266");  
    client.println("Host: " + String(host));  
    client.println("Connection: close");  
    client.println();

    Serial.println(getheader);//To Check
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }
  else{
    Serial.print("connection failed to ");
    Serial.println(host);
  }
  Serial.println("Done cycle.");
}

//Connect to WiFi
void connect_ap() {
  Serial.println();
  Serial.print("connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  Serial.begin(115200);
  connect_ap();

  Serial.println();
  Serial.println("Arduino: Measure Temperature"); 
  Serial.println("Dallas Temperature IC Control Library Demo"); 
  // Start up the library 
  sensors.begin(); 
}

unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working();
     delivering(payload);
  }
}
