#include <Arduino.h>
#include <ESP8266WiFi.h>              // Whole thing is about using Wi-Fi networks
#include <BlynkSimpleEsp8266.h>

const char* auth = "auth";
const char* ssid = "ssid";
const char* password = "pass";

void connectWiFi();
void connectBlynk();

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("[ INFO ] ESP 8 channel relay v0.1"));

  connectWiFi();
  connectBlynk();
}

void loop() {
    Blynk.run();
}

void connectWiFi() {
  Serial.printf("[ INFO ] Trying to connect to %u", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("done!");
  }
  Serial.println("Local IP: " + WiFi.localIP());
}

void connectBlynk() {
  Serial.print("[ INFO ] Starting connection to Blynk");
  Blynk.config(auth);
  int sek = 0;
  //delay(500);
  while (!Blynk.connected() && sek <10*2 && WiFi.status() == WL_CONNECTED) {
    Blynk.config(auth);
    Blynk.connect();
    delay(1000);
    sek++;
    Serial.print(F("."));
  }
  if (Blynk.connected()) {
    Serial.println("done.");
    //TODO Blynk notify with ipv4 and 6
    String msg = "RFID Controller rebooted. IPv4 local: " + WiFi.localIP();
    Blynk.notify(String(msg));
    Blynk.run();
  }else{
    Serial.println("failed.");
  }
}
