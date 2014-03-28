
#include "SIM900.h"
#include "inetGSM.h"
#include "gprs.h"
#include <string.h>


#define POWER_PIN 46
#define RESET_PIN 47

#define GPRS_BAUDRATE 2400

GPRS::GPRS(char* server, unsigned short port) {
  connected = false;

  _server = server;
  _port = port;

  pinMode(POWER_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
}

void GPRS::power() {
  digitalWrite(POWER_PIN, HIGH);
}

void GPRS::turnoff() {
  digitalWrite(POWER_PIN, LOW);
}

void GPRS::reset() {
  digitalWrite(RESET_PIN, HIGH);
  delay(100);
  digitalWrite(RESET_PIN, LOW);
}

bool GPRS::initialiseConnection() {
  //connected = gsm.begin(2400);
  return connected;
}


int GPRS::sendPacket(Packet packet) {

  char reply[50];
  char dataToSend[50]= "fzefefzgz";
  int answer = inet.httpJsonPOST(_server, _port,"/path",dataToSend,reply, 50);
  return 0;
}

char* GPRS::packetJsonCreator(Packet packet) {
  char json[sizeof(packet)];
  strcpy(json, "{\"type\":\"");
  strcat(json, &packet.type);
  strcpy(json, "\",\"timeDate\":");
  
  char timeDateString[7];
  itoa(packet.timeDate, timeDateString, 6);
  strcpy(json, timeDateString);
  strcpy(json, ",\"sensorDate\":[");
  
  char* sensorDataString;
  for(int i = 0; i < packet.numberOfSensorData; i++) { //sensorData = unsigned short = 2 bytes
    itoa(packet.sensorData[i], sensorDataString, 5);
    
    strcpy(json,sensorDataString);
    
    if(i + 1 <= packet.numberOfSensorData) {
      strcpy(json, ",");
    }
     
  }
  strcpy(json, "]}");
  
  return json;
}

bool GPRS::syncNTPTime() {
  //return inet.syncTimeWithNTP();
}


time_t GPRS::getNTPTime() {
  
}


