#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); 
void setup() {
   Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("°C\n");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("%\n");
}
