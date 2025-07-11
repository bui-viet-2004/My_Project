#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial Zigbee(3, 2);

void setup() {
    Serial.begin(9600);  
    dht.begin();          
    Zigbee.begin(9600); 
    Serial.println("Arduino Zigbee Receiver Ready");
    
}

void loop() {
    float humidity = dht.readHumidity();     
    float temperature = dht.readTemperature(); 
    if (isnan(humidity) || isnan(temperature)) {
        Zigbee.println("Errorrrrr");
        Serial.println("Error");
    } else {
        Zigbee.println(String(temperature, 2) + "," + String(humidity, 0));
        Serial.println(String(temperature, 2) + "," + String(humidity, 0));
    }
    delay(500);
}
