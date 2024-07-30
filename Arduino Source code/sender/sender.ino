#include <SPI.h>
#include <LoRa.h>
#include "DHT.h" 

#define LORA_SS_PIN 10
#define LORA_RST_PIN 9
#define LORA_DI0_PIN 2

#define analogPin A0 
#define measurePin A1
int ledPower = 4;

const int DHTPIN = 7;  
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(ledPower,OUTPUT);
  dht.begin(); 
  
  Serial.println("LoRa Sender");
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa init successful.");
  LoRa.setTxPower(20);

}

void loop(){
  String senderData = "";
  float temperature = temperatureFromDHT11(); 
  float humidity = humidityFromDHT11(); 
  if(temperature == -1 || humidity == -1 ){
    return;
  }else {
    Serial.print("temperature = ");
    Serial.println(temperature);
    Serial.print("humidity = ");
    Serial.println(humidity);
    
    float dustDensity = dustSensor();
    Serial.print("Dust = ");
    Serial.println(dustDensity);
  int rainVal = digitalRead(3);
   
    senderData = String(temperature)+", "+String(humidity)+", "+String(dustDensity)+", "+String(rainVal) ;
  }
  
  LoRa.beginPacket();
  LoRa.print(senderData);
  LoRa.endPacket();
  Serial.println("Data sent from ESP8266: " + senderData);
  delay(1000);
}

float temperatureFromDHT11(){    
  float t = dht.readTemperature();
  float result;
  if (isnan(t) ) {
    result = -1 ;    
  }else{
    result = t;
  }
  return result;
}

float humidityFromDHT11(){
  float h = dht.readHumidity();    
  float result;
  if (isnan(h)) {
    result = -1 ;    
  }else{
    result = h;
  }
  return result;
}


float dustSensor(){
  int samplingTime = 280;
  int deltaTime = 40;
  int sleepTime = 9680;
 
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;
  
  digitalWrite(ledPower,LOW); // Bật IR LED
  delayMicroseconds(samplingTime);  //Delay 0.28ms
  voMeasured = analogRead(measurePin); // Đọc giá trị ADC V0
  delayMicroseconds(deltaTime); //Delay 0.04ms
  digitalWrite(ledPower,HIGH); // Tắt LED
  delayMicroseconds(sleepTime); //Delay 9.68ms
 
  // Tính điện áp từ giá trị ADC
  calcVoltage = voMeasured * (5.0 / 1024); //Điệp áp Vcc của cảm biến (5.0 hoặc 3.3)
 
  dustDensity = 0.17 * calcVoltage - 0.1;
  return dustDensity; 
}
