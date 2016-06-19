#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <IRremoteESP8266.h>

WebSocketsServer webSocket = WebSocketsServer(81);
IRsend irsend(0);

const char* ssid = "SmartHome";
const char* password = "smart&cia";
 

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            break;
        case WStype_CONNECTED:
          {
                IPAddress ip = webSocket.remoteIP(num);
          }
            break;
        case WStype_TEXT:
        {
          String text = String((char *) &payload[0]); 
          if(text=="ligarTv"){
            irsend.sendNEC(3249146358, 32);
          }else if(text == "desligarTv"){
            for(int i = 0; i < 2; i++){
              irsend.sendNEC(3249146358, 32);
              delay(500);
            }
          }else if(text == "aumentar"){
            irsend.sendNEC(3249150438, 32);
          }else if(text == "diminuir"){
            irsend.sendNEC(3249183078, 32); 
          }
            break;
        }
        case WStype_BIN:
            hexdump(payload, lenght);
            break;
    }
} 
 
void setup() {
  Serial.begin(115200);
  delay(10);
  irsend.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  webSocket.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
}


