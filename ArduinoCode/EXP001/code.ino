
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

char ExperimentID[] = "EXP001";
byte Lserver[] = { 192, 168, 100, 6 };
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 100, 204);
EthernetServer server(80);
EthernetClient client;
String readRequest;
void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  Ethernet.begin(mac, ip);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  
  server.begin();
  Serial.print("Experiment ");
  Serial.print(ExperimentID);
  Serial.print(" started at ");
  Serial.println(Ethernet.localIP());
  //client.setTimeout(3000);
}


void loop() {
  // listen for incoming clients
 Routing();
 
 //LServer();
 delay(1000);
 //while(1);
}


void sendJson(){
 String temp;
    StaticJsonDocument<500> doc;

  // Create the "analog" array
  JsonArray analogValues = doc.createNestedArray("Data");
  for (int pin = 0; pin < 6; pin++) {
    // Read the analog input
    int value = analogRead(pin);

    // Add the value at the end of the array
    analogValues.add(value);
  }

//  // Create the "digital" array
//  JsonArray digitalValues = doc.createNestedArray("digital");
//  for (int pin = 0; pin < 14; pin++) {
//    // Read the digital input
//    int value = digitalRead(pin);
//
//    // Add the value at the end of the array
//    digitalValues.add(value);
//  }
  Serial.println();
  Serial.print(F("Sending: "));
  serializeJson(doc, Serial);
  Serial.println();

  // Write response headers
  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.print(F("Content-Length: "));
  client.println(measureJsonPretty(doc));
  client.println();
  // Write JSON document
  serializeJsonPretty(doc, client);
  
  // Disconnect
  client.stop();
  }

void Reset(){
  client.stop();
  }
void Stop(){
  StaticJsonDocument<200> doc;
  doc["Status"] = "Active";
  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.print(F("Content-Length: "));
  client.println(measureJsonPretty(doc));
  client.println();
  serializeJsonPretty(doc, client);
  client.stop();
  }
void Para(){
  client.stop();
  }
void Start(){
  sendJson();
 // client.stop();
  }    
void Status(){
  
  StaticJsonDocument<200> doc;
  doc["Status"] = "Active";
  
  serializeJson(doc, Serial);
  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.print(F("Content-Length: "));
  client.println(measureJsonPretty(doc));
  client.println();
  serializeJsonPretty(doc, client);
  client.stop();
 } 

 void LServer(){
  if(client.connect(Lserver, 3000)){
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /Start");
    client.println("Host: 192.168.100.6:3000");
    client.println("Connection: close");
    client.println();
    }
  }
   
void NullResponse(){
  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println();
  client.stop();
  }

void Routing(){
client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (readRequest.length() < 100) {
          readRequest += c;          
          }
          Serial.write(c);
        if (readRequest.indexOf("Start")>0 ) {
          //Start();
          NullResponse();
          //client.stop();
          LServer();
          readRequest="";
          break;
        }
        else if (readRequest.indexOf("Stop")>0 ) {
          Stop();
          readRequest="";
          break;
        }
        else if (readRequest.indexOf("Para")>0 ) {
          Para();
          readRequest="";
          break;
        }
        else if (readRequest.indexOf("Reset")>0 ) {
          Reset();
          readRequest="";
          break;
        }
        else if (readRequest.indexOf("Status")>0 ) {
          Status();
          readRequest="";
          break;
        }
      }
    }
  Serial.println("client disconnected");
  }
}