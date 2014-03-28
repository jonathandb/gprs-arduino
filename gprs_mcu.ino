#include "packet_manager.h"
#include "SIM900.h"
#include "inetGSM.h"
#include <Time.h>
#include <arduino.h>
#include <SD.h>
#include <EEPROM.h>
#include "configuration.h"
#include <SoftwareSerial.h>

//Configuration configuration;

void setup() {
    //check configuration
    
    //bool success = configuration.configLoadSuccess;
    
    Serial.begin(9600);
    //Serial.println("load success: ");
    //Serial.print(success);
    
    //Serial.println(configuration.parameters.serverAddress);
    
    
    //start gprs to sync time and start timer
    
    
    //initialise sensor reading
    

    //InetGSM inet;
    //File dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {

    Serial.print("test");

}

void streamPacketsTest() {
  
  PacketManager packetManager;

  time_t current = now();
  
  unsigned short numberOfSensorData = 999;
  packetManager.initPacketFile('X',current,numberOfSensorData);
  
  char type = 'X';
  for(int i = 0; i < 10; i++) {
    current = now();
    
    unsigned short sensorData[4] = {1,3,5,10};
  
    Serial.println("Writting package...");
  
    packetManager.writePacket(type, current, sensorData);
    
    delay(1000);
  }
  
  
  packetManager.openFileFromDay(day(), month(), year(), type);
  
  Packet p;
  Serial.println("Written packets:");
  packetManager.loadNextPacketFromDay(&p);
  if(true) {
    Serial.println("Packet:");

    Serial.println("Type:");
    Serial.println(p.type);
    Serial.println("TimeDate:");
    Serial.println(p.timeDate);
    Serial.println("Sensordata:");
    for(int i = 0; i < p.numberOfSensorData; i++) {
      Serial.println(p.sensorData[i]);
    }
    Serial.print("\n\r");
  }

}
