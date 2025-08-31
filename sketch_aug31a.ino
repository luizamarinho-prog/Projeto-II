#include <DHT.h>
#include <DHT_U.h>
#include <MQ2_LPG.h>
#define DHTPIN 2 
#define DHTTYPE DHT22
#define MQ2pin 34
DHT dht(DHTPIN, DHTTYPE); 
void setup() {
   Serial.begin(9600);
  Serial.println(F("DHT22 test!"));
  Serial.println("MQ2 warming up!");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float sensorValue = analogRead(MQ2pin);
  Serial.print("\nSensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("°C\n");
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("%\n");
  delay(2000);
}