#include <WiFi.h>
#include <HttpClient.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <Fonts/FreeSans9pt7b.h>
#include <TFT_eSPI.h>


// BH1750 Light Sensor 
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;


// AWS IoT Core
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Adafruit_AHTX0 aht;
TFT_eSPI tft = TFT_eSPI();

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(200);


  // BH1750 Light Sensor
  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }



  // WiFi
  delay(200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
  
  // Display
  tft.init();
  tft.fillScreen(TFT_BLACK);

  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void loop() {
  // Humidity Temp Sensor
  tft.fillScreen(TFT_BLACK);
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  tft.drawString("Temp: " + String(temp.temperature) + (" C"), 5, 50, 2);
  tft.drawString("Hum: " + String(humidity.relative_humidity) + (" %"), 5, 100, 2);
  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print("Pressure: ");Serial.print(humidity.relative_humidity);Serial.println(" RH %");


  // Light Sensor
  float lux = 0.0;
  if (lightMeter.measurementReady()) {
    lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
    tft.drawString("Light: " + String(lux) + (" lx"), 5, 150, 2);
  }


  // AWS IoT Core Pub
  StaticJsonDocument<200> doc;
  doc["unit2_humidity"] = temp.temperature;
  doc["unit2_temperature"] = humidity.relative_humidity;
  doc["unit2_lux"] = lux;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  client.loop();
  delay(3000);
}