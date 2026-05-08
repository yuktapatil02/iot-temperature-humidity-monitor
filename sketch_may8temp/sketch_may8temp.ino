#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN 2        // GPIO2
#define DHTTYPE DHT11    // Or DHT22

const char* ssid = "Patil_5G";
const char* password = "babbu2114";
const char* apiKey = "FIDF67Z1UBZGTIXD";
unsigned long channelID = 3374469;

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  dht.begin();

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int result = ThingSpeak.writeFields(channelID, apiKey);

  if (result == 200) {
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.print("Error sending data. Code: ");
    Serial.println(result);
  }

  delay(10000); // Wait 10 seconds
}
