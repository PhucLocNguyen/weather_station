#include <SPI.h>
#include <LoRa.h>
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6ttdmKlos"
#define BLYNK_TEMPLATE_NAME "project IOT"
#define BLYNK_AUTH_TOKEN "n4tbnNACySMz04S46m3m_prYlCjawrNh"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "n4tbnNACySMz04S46m3m_prYlCjawrNh";

char ssid[] = "FPTU_Student";
char pass[] = "12345678";

#define SS D8
#define RST D0
#define DIO0 D1

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Blynk.begin(auth, ssid, pass);
  Serial.println("Receiver Host");
  LoRa.setPins(SS,RST,DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa init successful.");
  LoRa.setTxPower(20);
  
}
void loop(){
  Blynk.run();
  float temperature = 0;
  float humidity = 0;
  int rainVal = 0;
  String rainStatus = "";
  float dust = 0;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
  String receivedData ="";
    
  while (LoRa.available()) {
      receivedData +=(char)LoRa.read();
  }
  Serial.println(receivedData);
  char delimiter[] = ", ";
  char* token = strtok((char*)receivedData.c_str(), delimiter);
  
  temperature = atof(token);
  token = strtok(NULL, delimiter);
  humidity = atof(token);
  token = strtok(NULL, delimiter);
  dust = atof(token);
  token = strtok(NULL, delimiter);
  rainVal = atof(token);
  Serial.println(rainVal);
  if(rainVal ==0){
    rainStatus = "It's rainning";
  }else {
    rainStatus = "It's sunny";
  }
  Blynk.virtualWrite(V0,temperature);
  Blynk.virtualWrite(V1,humidity);
  Blynk.virtualWrite(V2,dust);
  Blynk.virtualWrite(V3,rainStatus);
  }
  delay(1);
}
