#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;     // SSID
char pass[] = SECRET_PASS;     // password

WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#define DHTPIN 2          // Pin
#define DHTTYPE DHT22     // DHT 22

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);  
  dht.begin();          

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) 
  {
    Serial.print("_");
    delay(5000);
  }

  Serial.println("\nConnected to Wi-Fi");
  ThingSpeak.begin(client);  // ThingSpeak
}

void loop() {
  // Reading temperature and humidity data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the reads are out of the expected range
  if (temperature < -40.0 || temperature > 80.0 || humidity < 0.0 || humidity > 100.0) 
  {
    Serial.println("Invalid readings from DHT sensor! Values are out of expected range.");
    return;
  }

  // Print the temperature and humidity to the Serial Monitor
  Serial.print("Current room Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Current room Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Update ThingSpeak field 1 with the temperature data
  ThingSpeak.setField(1, temperature);
  
  // Update ThingSpeak field 2 with the humidity data
  ThingSpeak.setField(2, humidity);

  // Write the data to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) 
  {
    Serial.println("Temperature and Humidity successfully sent to ThingSpeak..");
  } 
  else 
  {
    Serial.println("Error sending data to ThingSpeak. HTTP error code: " + String(x));
  }

  // 60 seconds delay
  delay(60000);
}
