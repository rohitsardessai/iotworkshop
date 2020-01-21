#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// url: http://arduinojson.org/example.json
const char* ssid = ""; //replace with your own wifi ssid
const char* password = ""; //replace with your own wifi ssid password
const char* url = "http://arduinojson.org/example.json";

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);

  Serial.print("connecting to ");
  Serial.println(url);

  // Use WiFiClient class to create TCP connections
  HTTPClient http;
  http.begin(url);
  String jsonString;
  int httpCode = http.GET();
  if (httpCode == 200) {
    jsonString = http.getString();
    http.end();
  }
  else {
    Serial.println("Error on HTTP request in getData().");
    http.end();
  }

   // use this site to help with json parsing: https://arduinojson.org/v5/assistant/
  StaticJsonBuffer<500> JSONBuffer;
  JsonObject& root = JSONBuffer.parseObject(jsonString);

  if (!root.success()) {
    Serial.println("Parsing failed");
    return;
  }

  String sensor = root["sensor"];
  int timea = root["time"];
  float lat = root["data"][0];
  float lon = root["data"][1];

  Serial.println("\nsensor: " + sensor);
  Serial.print("time: ");
  Serial.println(timea);
  Serial.print("lat: ");
  Serial.println(lat);
  Serial.print("lon: ");
  Serial.println(lon);

}
