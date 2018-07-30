

#include <IPAddress.h>
#include <Stream.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4

// MQTT_VERSION : Pick the version
#define MQTT_VERSION MQTT_VERSION_3_1
#ifndef MQTT_VERSION
//#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif


//#ifndef MQTT_MAX_PACKET_SIZE
//#define MQTT_MAX_PACKET_SIZE 128
//#endif


int button= D7;
int nilaitombol;
int button1= D8;
int nilaitombol1;
int count;
 
//const int led= D5;
//const int led1= D6;
//const int pin_LED = D5;
//const int pin_LED1 = D6;
//int pin_switch = D7;
//int pin_switch1 = D8;

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;


// variables to hold the new and old switch states
boolean oldSwitchState = LOW;
boolean newSwitchState = LOW;
boolean LEDstatus = LOW;


// WIFI parameters
const char* SSID = "SSID";
const char* PASSWORD = "PASS";


const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;
WiFiClient espClient;
PubSubClient MQTT(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


void setup() {
  initPins();
  initSerial();
  initWiFi();
  initMQTT();

}

void initPins() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  //  pinMode(D2, OUTPUT);
  //  pinMode(D3, OUTPUT);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  //  digitalWrite(D2, LOW);
  //  digitalWrite(D3, LOW);

  //saklar manual
pinMode(button, INPUT);
//pinMode(led, OUTPUT);
pinMode(button1, INPUT);
//pinMode(led1, OUTPUT);
}

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD); // Wifi Connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(SSID);
  Serial.println(" | IP ");
  Serial.println(WiFi.localIP());
}

// MQTT Broker connection
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// Receive messages
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.print("Topic ");
  Serial.print(topic);
  Serial.print(" | ");
  Serial.println(message);

  //relay hiji
  if (message == "ona") {
    digitalWrite(D0, HIGH);
    Serial.println("Chanel 1 Activate by GhostSys");
    ++value;
    snprintf (msg, 75, "Chanel 1 Activate by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }
  if (message == "offa") {
    digitalWrite(D0, LOW);
        Serial.println("Chanel 1 NON-Activate by GhostSys");
        ++value;
    snprintf (msg, 75, "Chanel 1 NON-Activate by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }

  //relay dua
  if (message == "onb") {
    digitalWrite(D1, HIGH);
    Serial.println("Chanel 2 Activate by GhostSys");
    ++value;
    snprintf (msg, 75, "Chanel 2 Activate by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }
  if (message == "offb") {
    digitalWrite(D1, LOW);
            Serial.println("Chanel 2 NON-Activate by GhostSys");
            ++value;
    snprintf (msg, 75, "Chanel 2 NON-Activate by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }

  ////relay tiga
  //  if (message == "1c") {
  //    digitalWrite(D2, HIGH);
  //    }
  //  if (message == "2c") {
  //    digitalWrite(D2, LOW);
  //    }
  //
  ////relay tiga
  //  if (message == "1d") {
  //    digitalWrite(D3, HIGH);
  //    }
  //  if (message == "2d") {
  //    digitalWrite(D3, LOW);
  //}
  //semua off

  if (message == "alloff") {
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    //    digitalWrite(D2, LOW);
    //    digitalWrite(D3, LOW);
                Serial.println("All Chanel Off by GhostSys ");
                ++value;
    snprintf (msg, 75, "All Chanel Off by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }
  //semua nyala

  if (message == "allon") {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    //    digitalWrite(D2, HIGH);
    //    digitalWrite(D3, HIGH);
                 Serial.println("All Chanel On by GhostSys ");
                 ++value;
    snprintf (msg, 75, "All Chanel On by GhostSys #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
  }

  message = "";
  Serial.println();
  Serial.flush();
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Menghubungkan dengan Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (MQTT.connect("Smart Office")) {
      Serial.println("Terhubung");
      MQTT.subscribe("cid/lamp"); 
    snprintf (msg, 75, "System Smartoffice READY !", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
    } else {
      Serial.println("Terjadi kesalahan koneksi");
      Serial.println("Mencoba menghubungkan kembali dalam 2 detik");
      delay(2000);
    }
  }
}

void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}

void loop() {
nilaitombol= digitalRead(button);
 
if(nilaitombol == 1){
count++;
delay(300);
if(count==1){
digitalWrite(D0, LOW);
    Serial.println("Chanel 1 Push by User");
    ++value;
    snprintf (msg, 75, "Chanel 1 Push by User Status NON_Active #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("",msg);
}
if(count==2){
digitalWrite(D0, HIGH);
    Serial.println("Chanel 1 Push by User");
    ++value;
    snprintf (msg, 75, "Chanel 1 Push by User Status Active #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
count=0;
}
}

nilaitombol= digitalRead(button1);
 
if(nilaitombol == 1){
count++;
delay(300);
if(count==1){
digitalWrite(D1, LOW);
    Serial.println("Chanel 2 Push by User");
    ++value;
    snprintf (msg, 75, "Chanel 2 Push by User Status NON_Active #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
}
if(count==2){
digitalWrite(D1, HIGH);
    Serial.println("Chanel 2 Push by User ");
    ++value;
    snprintf (msg, 75, "Chanel 2 Push by User Status Active #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MQTT.publish("smartoffice/cid",msg);
count=0;
}
}

  if (!MQTT.connected()) {
    reconnectMQTT();
  }
  recconectWiFi();
  MQTT.loop();

 
}
