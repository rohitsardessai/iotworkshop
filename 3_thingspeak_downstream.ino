#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// url: https://api.thingspeak.com/channels/954210/feeds.json?api_key=O75JZJOV3RY5XLFB&results=1
const char* ssid = "Rohit Sardessai"; //replace with your own wifi ssid
const char* password = "getyourownwifi"; //replace with your own wifi ssid password
const char* host = "api.thingspeak.com";
const String apikey = "O75JZJOV3RY5XLFB";
int value = 0;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default, would try to act as both a client and an access-point and could cause network-issues with your other WiFi-devices on your WiFi-network. */
  //WiFi.mode(WIFI_STA);

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
  ++value;
  if (value > 9) {
    value = 0;
  }
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  //this url contains the informtation we want to send to the server
  //if esp8266 only requests the website, the url is empty
  String url = "/channels/954210/feeds.json?api_key=";

  /* url += "?param1=";
    url += param1;
    url += "?param2=";
    url += param2;
  */


  Serial.print("\n\nRequesting URL: ");
  //Serial.println("GET " + url + apikey + "&results=1" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  client.print("GET " + url + apikey + "&results=1" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();

  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  Serial.println("\nClient response: \n");
  // Read all the lines of the reply from server and print them to Serial

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  String data;
  if (client.available()) {
    data = client.readString();
    Serial.print(data);
    //Serial.println(data.length());
  }

  //static char responseBuffer[1024];
  //data.toCharArray(responseBuffer, data.length());
  
  //to send the same string back from serial monitor
  //Serial.println("Send string");
  //while (!Serial.available()) {
  //  delay(50);
  //}
  //String json1 = Serial.readString();
  //Serial.println(json1);
  

  //const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(10) + 290;
  //DynamicJsonBuffer jsonBuffer(capacity);
  StaticJsonBuffer<1024> jsonBuffer;
  //String json = "{\"channel\":{\"id\":954210,\"name\":\"Test01\",\"description\":\"Test for workshop\",\"latitude\":\"0.0\",\"longitude\":\"0.0\",\"field1\":\"Temp\",\"field2\":\"Hum\",\"created_at\":\"2020-01-07T16:37:28Z\",\"updated_at\":\"2020-01-07T16:37:28Z\",\"last_entry_id\":165},\"feeds\":[{\"created_at\":\"2020-01-08T19:30:38Z\",\"entry_id\":165,\"field1\":\"2\",\"field2\":\"4\"}]}";
  //String json = data;

  JsonObject& root = jsonBuffer.parseObject(data);

  JsonObject& channel = root["channel"];
  long channel_id = channel["id"]; // 954210

  JsonObject& feeds_0 = root["feeds"][0];
  int feeds_0_entry_id = feeds_0["entry_id"]; // 165
  int feeds_0_field1 = feeds_0["field1"]; // "2"
  int feeds_0_field2 = feeds_0["field2"]; // "4"

  Serial.print(F("\nChannel id: "));
  Serial.println(channel_id);
  Serial.print(F("Entry id: "));
  Serial.println(feeds_0_entry_id);
  Serial.print(F("Temp: "));
  Serial.println(feeds_0_field1);
  Serial.print(F("Hum: "));
  Serial.println(feeds_0_field2);

  /* StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& root = JSONBuffer.parseObject(data);

    JsonObject& channel = root["channel"];
    int channel_id = channel["id"];

    JsonObject& feeds_0 = root["feeds"][0];
    int entry_id = feeds_0["entry_id"];
    int temp = feeds_0["field1"];
    int hum = feeds_0["field2"];


    if (!root.success()) {
     Serial.println("Parsing failed");
     return;
    }*/

  /*  int channel_id = parsed["channel"]["id"];
    int entry_id = parsed["feeds"]["entry_id"];
    float temp = parsed["feeds"]["field1"];
    float hum = parsed["feeds"]["field2"];

    Serial.println("\nChannel id: ");
    Serial.print(channel_id);
    Serial.print("Entry id: ");
    Serial.println(entry_id);
    Serial.print("Temp: ");
    Serial.println(temp);
    Serial.print("Hum: ");
    Serial.println(hum);*/

}
