#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
#define DHTPIN 26     
#define DHTTYPE DHT11   
 
#define ss 5
#define rst 14
#define dio0 2

DHT dht(DHTPIN, DHTTYPE);
float temperature = 0;
float humidity = 0;
 
 #include <AntaresESPHTTP.h>

#define ACCESSKEY "316d65ffa0503404:16e2e400caee5da3"       // Replace with your Antares account access key
#define WIFISSID "Kabeh"         // Replace with your Wi-Fi SSID
#define PASSWORD "pakabeh27"     // Replace with your Wi-Fi password

#define projectName "Lora_database"   // Replace with the Antares application name that was created
#define deviceName "Test"     // Replace with the Antares device name that was created

AntaresESPHTTP antares(ACCESSKEY);


void setup() {    
  Serial.begin(115200);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);
  
  dht.begin();
  while (!Serial);
  Serial.println("LoRa Sender");
 
   //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }

  Serial.println("LoRa Initializing OK!");
}

 void getReadings(){
  temperature = dht.readTemperature();
  humidity= dht.readHumidity();
} 

void loop() 
{
  temp = dht.readTemperature();
  hum = dht.readHumidity();
 
  Serial.println("Sending packet: ");
 
  // send packet
  LoRa.beginPacket();
  LoRa.print("Humidity: ");
  LoRa.print(hum);
  LoRa.print("%");
  LoRa.print(" Temperature:");
  LoRa.print(temp);
  LoRa.print("C");
  
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%");
  Serial.print(" Temperature:");
  Serial.print(temp);
  Serial.println("C");
  Serial.println(""); 
 
  LoRa.endPacket();
  
  getReadings();

  delay(5000);
       // Add variable data to storage buffer
  antares.add("temperature", temperature);
  antares.add("humidity", humidity);

  // Send from buffer to Antares
  antares.send(projectName, deviceName);
  delay(10000); 
}