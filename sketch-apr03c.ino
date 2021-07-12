#include <ESP8266WiFi.h>
#include <string.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       ""
#define WLAN_PASS       ""
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe onoffbutton1= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light1");
Adafruit_MQTT_Subscribe onoffbutton2= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light2");
Adafruit_MQTT_Subscribe onoffbutton3= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light3");
void MQTT_connect();
void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton1);
  mqtt.subscribe(&onoffbutton2);
  mqtt.subscribe(&onoffbutton3);
}

uint32_t x=0;

void loop() {
 
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton1) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton1.lastread);
      uint16_t state =atoi((char *)onoffbutton1.lastread);
      if(strcmp((char *)onoffbutton1.lastread,"ON")==0)
        digitalWrite(16,HIGH);
      else
        digitalWrite(16,LOW);
    }
    else if (subscription == &onoffbutton2) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton2.lastread);
      uint16_t state =atoi((char *)onoffbutton2.lastread);
      if(strcmp((char *)onoffbutton2.lastread,"ON")==0)
        digitalWrite(5,HIGH);
      else
        digitalWrite(5,LOW);
    }
    else if (subscription == &onoffbutton3) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton3.lastread);
      uint16_t state =atoi((char *)onoffbutton3.lastread);
      if(strcmp((char *)onoffbutton3.lastread,"ON")==0)
        digitalWrite(4,HIGH);
      else
        digitalWrite(4,LOW);
    }
  }
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
