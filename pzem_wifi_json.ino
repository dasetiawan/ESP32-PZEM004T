#include <ArduinoJson.h>    // ArduinoJson library v5.9.0
#include <WiFi.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>
#define RXD2 16 
#define TXD2 17

const char* ssid = "your_ssid";             // your wifi ssid
const char* password = "your_password";     // your wifi password
const char* mqttServer = "192.168.1.100";   // mqtt server address
const int mqttPort = 1883;                  // mqtt port
const char* mqttUser = "remote_username";     // your mqtt user
const char* mqttPassword = "remote_password"; // your mqtt password

float v;  // variable for voltage
float c;  // variable for current
float p;  // variable for power
float e;  // variable for energy

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
    JSONencoder["voltage"] = v;
    JSONencoder["current"] = c;
    JSONencoder["power"] = p;
    JSONencoder["energy"] = e;

    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
     
    if (client.publish("esp/test", JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
    
    delay(2000);
    client.loop();
}
 
void readPZEM(){
    v = pzem.voltage();
    c = pzem.current();
    p = pzem.power();
    e = pzem.energy();
}
