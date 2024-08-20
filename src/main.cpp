#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include "WiFi.h"
#include "ESP.h"


//----------------------------------------------------------------MQTT Message-------------------------------
// float temperature = 23;
// const char *SSID = "IR-GA NETWORK";
// const char *PWD = "indoprima";
const char *SSID = "Redmi Note 10S";
const char *PWD = "sir666sir";


// IPAddress mqttServer(192, 168, 0, 10);
IPAddress mqttServer(34, 122, 22, 232);
// http://34.122.22.232/

int countmqtt = 0;
void callback(char *topic, byte *payload, unsigned int length)
{
  // Serial.println("Callback");
  // Serial.println((char) payload[0]);
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    //   Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }

  if (String(topic) == "IPGP5/TEMP/WCLOUT")
  {

    Serial.println(messageTemp);
    // connectestablished(messageTemp);
  }
}
WiFiClient wifiClient = WiFiClient();
// mqttClient.subscribe("esp32/output");
PubSubClient mqttClient(mqttServer, 1883, callback, wifiClient);

void connectToWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PWD);
  int countconnect = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");

    delay(500);

    countconnect++;
    if (countconnect > 10)
    {
      ESP.restart();
      countconnect = 0;
    }
    // we can even make the ESP32 to sleep
  }

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("Connected - ");
  // Serial.println(WiFi.localIP);
}

void reconnect()
{
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected())
  {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32-TRIAL123";

    if (mqttClient.connect(clientId.c_str()))
    {
      Serial.println("Connected.");
      mqttClient.subscribe("PGP5/TEMP/WCLOUT");
      // subscribe to topic
    }
    else
      countmqtt++;
    if (countmqtt > 50)
      ESP.restart();
  }
}

float temperature = 0;
float data_temp[4] = {0, 0, 0, 0};
String send_temp;
void setup()
{
  // Set up DE/RE pin
 
  Serial.begin(115200);     // Serial monitor
  mqttClient.setCallback(callback);
  connectToWiFi();

  Serial.println("Modbus communication started...");
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED)
    connectToWiFi();

  if (!mqttClient.connected())
    reconnect();
  else
  {
 
    String sendmqtt = String("WCL5T") + "#TEST";
    mqttClient.publish("/test/trial", sendmqtt.c_str());
    delay(1000);
   
  }

  mqttClient.loop();
}
