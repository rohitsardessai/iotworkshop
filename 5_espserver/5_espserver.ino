#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char* ssid = "";
char* password = "";


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
  Serial.println(toggle);
  int a = toggle.toInt();
  if (a == 1) {
    Serial.println("Received 1"); 
  }
  else if(a == 0) {
    Serial.println("Received 0");
  }
  else {
    Serial.print("Unknown response received: \"");
    Serial.print(a);
    Serial.println("\"");
  }
  server.send(204, ""); //respond with empty message. 204 represents empty response
  delay(10);
}
