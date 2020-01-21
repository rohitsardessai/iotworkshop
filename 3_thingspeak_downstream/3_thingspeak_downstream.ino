#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// url: http://api.thingspeak.com/channels/954210/feeds.json?api_key=O75JZJOV3RY5XLFB&results=1
const char* ssid = "845N-2_EXT"; //replace with your own wifi ssid
const char* password = "tintin123"; //replace with your own wifi ssid password 

String url = "http://api.thingspeak.com/channels/954210/feeds.json?api_key=O75JZJOV3RY5XLFB&results=1";

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


  Serial.println("\nClient response: \n");
  Serial.println(jsonString);



  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonString);

  JsonObject& channel = root["channel"];
  long channel_id = channel["id"]; // 954210

  JsonObject& feeds_0 = root["feeds"][0];
  int feeds_0_entry_id = feeds_0["entry_id"];
  int feeds_0_field1 = feeds_0["field1"];
  int feeds_0_field2 = feeds_0["field2"];

  Serial.print(F("\nChannel id: "));
  Serial.println(channel_id);
  Serial.print(F("Entry id: "));
  Serial.println(feeds_0_entry_id);
  Serial.print(F("Temp: "));
  Serial.println(feeds_0_field1);
  Serial.print(F("Hum: "));
  Serial.println(feeds_0_field2);

}
