#include <SPI.h>
#include <Ethernet.h>

#define ARD_1

#ifdef ARD_1
#define SERVER_PORT 10002
#define CLIENT_PORT 10003
char *message = "I'm ARD_1!\n";
#else
#define SERVER_PORT 10003
#define CLIENT_PORT 10002
char *message = "I'm ARD_2!\n";
#endif

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#ifdef ARD_1
IPAddress ip(192,168,0,177);
#else
IPAddress ip(192,168,0,178);
#endif
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

#ifdef ARD_1
IPAddress otherIP(192,168,0,178);
#else
IPAddress otherIP(192,168,0,177);
#endif

EthernetServer server(SERVER_PORT);
boolean alreadyConnected = false;

EthernetClient client;

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  while(!Serial) { 
  }

  delay(1000);

  boolean connecting = true;

  while(connecting) {
    if(client.connect(otherIP, CLIENT_PORT)) {
      Serial.println("Connected");
      connecting = false;
    } else {
      Serial.println("Connection Failed, trying again");
      delay(1000);
    }
  }
}

void loop()
{
  EthernetClient otherClient = server.available();

  if(otherClient) {
    if(!alreadyConnected) {
      otherClient.flush();
      Serial.println("We have a new client.");
      otherClient.println("Hello client!");
      alreadyConnected = true;
    }

    if(otherClient.available()) {
      char thisChar = otherClient.read();
      server.write(thisChar);
      Serial.write(thisChar);
    }
  }

  if(client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar); 
    }
  }

  if(client.connected()) {
    client.print(message);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    while(true);
  }
}

