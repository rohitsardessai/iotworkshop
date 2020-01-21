#include <ESP8266WiFi.h>
#include <ArduinoJson.h> //use version 5

// url: http://arduinojson.org/example.json
const char* ssid = "-2_EXT"; //replace with your own wifi ssid
const char* password = ""; //replace with your own wifi ssid password
const char* host = "arduinojson.org";


void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /*Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default, would try to act as both a client and an access-point and could cause network-issues with your other WiFi-devices on your WiFi-network. */
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
  String url = "/";




  Serial.print("\n\nRequesting URL: ");
  Serial.println("GET " + url + "example.json" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  client.print("GET " + url + "example.json" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();

  while (client.available() == 0) { //wait for server to respond
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }


  
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
  }
  
  // use this site to help with json parsing: https://arduinojson.org/v5/assistant/
  StaticJsonBuffer<500> JSONBuffer;
  JsonObject& parsed = JSONBuffer.parseObject(data);

  if (!parsed.success()) {
    Serial.println("Parsing failed");
    return;
  }

  String sensor = parsed["sensor"];
  int timea = parsed["time"];
  float lat = parsed["data"][0];
  float lon = parsed["data"][1];

  Serial.println("\nsensor: " + sensor);
  Serial.print("time: ");
  Serial.println(timea);
  Serial.print("lat: ");
  Serial.println(lat);
  Serial.print("lon: ");
  Serial.println(lon);

}
