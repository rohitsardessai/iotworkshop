#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server; //create an object named server

char* ssid = ""; //put you ssid
char* password = ""; //put your password

void setup()
{
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleIndex); //listen for request and call handleIndex() function
  server.on("/send", fun); ////listen for request and call fun() function
  server.begin();
}

//For example, use localhost/send?value=7 to send the number 7 to the server

void loop()
{
  server.handleClient(); //required
}

void handleIndex() {
  server.send(200, "text/plain", "Hello World!"); //listen for request and reply
  delay(10);
}

void fun() {
  String val1 = server.arg("value");
  Serial.println(val1);
  server.send(200, "text/plain", "OK"); //listen for request and reply
  delay(10);
}
