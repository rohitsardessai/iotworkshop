#include <ESP8266WiFi.h>

// url: https://api.thingspeak.com/update?api_key=xxxxxxxxxxfield1=0

const char* ssid = ""; //replace with your own wifi ssid
const char* password = ""; //replace with your own wifi ssid password
const char* host = "api.thingspeak.com";
const String apikey = "xxxxxxxxxx"; //replace with your API key
int value = 0;

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
  delay(50);
  ++value;
  if(value > 9){
    value = 0; //just values to upload
  }
  Serial.print("connecting to ");
  Serial.println(host);


  WiFiClient client;

  const int httpPort = 80; //default port

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  } 

  // We now create a URI for the request
  //this url contains the informtation we want to send to the server
  //if esp8266 only requests the website, the url is empty
  String url = "/update?api_key=";
  String field1 = "&field1=";
  String value1 = String(value);
 
  //Serial.print("\n\nRequesting URL: ");
  //Serial.println("GET " + url + apikey + field1 + value1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  
  client.print("GET " + url + apikey + field1 + value1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  
  unsigned long timeout = millis(); 
  while (client.available() == 0) { //wait for client to respond
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  Serial.println("\nClient response: \n");
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readString();
    Serial.print(line);
  }
  

  Serial.println();
  Serial.println("closing connection");
    delay(12000);
}
