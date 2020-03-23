#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>
#define RXD2 16 
#define TXD2 17

const char* ssid = "TOTOLINK_N200RE";
const char* password = "Afif1234"; 
const char* mqttServer = "192.168.1.116";
const int mqttPort = 1883;
const char* mqttUser = "remote_username";
const char* mqttPassword = "remote_password";

float tegangan;
float arus;
float daya;

PZEM004Tv30 pzem(&Serial2);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
 
void loop() {
    readPZEM();
    StaticJsonBuffer <300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
    
    JSONencoder["device"] = "ESP32";
    JSONencoder["sensor type"] = "PZEM004T";
    JSONencoder["tegangan"] = tegangan;
    JSONencoder["arus"] = arus;
    JSONencoder["daya"] = daya;

    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
     
    if (client.publish("esp/test", JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
    //client.publish("daya", String(daya).c_str());
    delay(2000);
    client.loop();
}
 
void readPZEM(){
    tegangan = pzem.voltage();
    arus = pzem.current();
    daya = pzem.power();
}
