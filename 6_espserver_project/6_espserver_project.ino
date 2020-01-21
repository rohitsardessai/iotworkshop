#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char* ssid = "Rohit Sardessai";
char* password = "getyourownwifi";


void setup()
{

  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
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

  /*server.on("/", []() {
    server.send(200, "text/plain", "Hello World!");
  }); //listen for request and reply*/\
  server.on("/", handleIndex);
  server.on("/switch", OnOff); //listen for /bat and call function OnOff
  server.begin();
}

void loop()
{
  server.handleClient();
}

void handleIndex() {
  server.send(200, "text/plain", "Hello World!"); //listen for request and reply
  delay(10);
}
void OnOff() {
  String toggle = server.arg("tog"); //look for "tog" in url and save value in string
  //Serial.println(toggle);
  int a = toggle.toInt();
  if (a == 0) {
    Serial.println("Forward"); 
  }
  else if(a == 1) {
    Serial.println("Backward");
  }
  else if(a == 2) {
    Serial.println("Left");
  }
  else if(a == 3) {
    Serial.println("Right");
  }
  else {
    Serial.print("Unknown response received: \"");
    Serial.print(a);
    Serial.println("\"");
  }
  server.send(204, ""); //respond with empty message. 204 represents empty response
  delay(10);
}
