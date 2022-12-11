#include <ESP8266WiFi.h>
#include "DHT.h"
#include <ThingSpeak.h>

const char *ssid =  "abcdef";  
const char *pass =  "pqrst";


WiFiClient client;

long myChannelNumber = 1956120;
const char myWriteAPIKey[] = "DDW5D3FRBUO2XBNR";

#define DHT11_PIN 2
#define DHTTYPE DHT11
#define sensor_pin A0
#define RELAY_PIN 4

DHT dht(DHT11_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());

  pinMode(RELAY_PIN, OUTPUT);

  Serial.println(F("DHTxx test!"));
  dht.begin();

  ThingSpeak.begin(client);
}

void loop() {

  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float moisture_Value;
  float moisture_percentage;

  moisture_Value = analogRead(sensor_pin);
  moisture_percentage = (100-((moisture_Value/1024.00)*100));

  if(moisture_percentage<30)
  digitalWrite(RELAY_PIN, HIGH);
  else
  digitalWrite(RELAY_PIN, LOW);

  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, moisture_percentage, myWriteAPIKey);


  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.println("");
  Serial.print("Soil Moisture(in Voltage) = ");
  Serial.print(moisture_Value);
  Serial.println("");
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  delay(5000);
}
