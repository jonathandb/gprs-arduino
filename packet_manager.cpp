#include "packet_manager.h"
#include "stdlib.h"
#include "string.h"
#include <SD.h>

PacketManager::PacketManager() {
  //streamPacket = (Packet *) malloc(sizeof(Packet));
}

void PacketManager::initPacketFile(char type, time_t timeDate, unsigned short numberOfSensorData) {
  unsigned short fileNr = 0;
  char dd[2];
  char mm[2];
  char yy[2];
  char ty[2];
  itoa(day(), dd, 2);
  itoa(month(), mm, 2);
  itoa(year(), yy, 2);
  itoa(type, ty, 2);
  char txtName[12];
  strcpy(txtName,dd);
  strcat(txtName,mm);
  strcat(txtName,yy);
  strcat(txtName,ty);
  strcat(txtName,".txt");
  
  writingFile = SD.open(txtName, FILE_WRITE);
  
  //create head
  writingFile.write(numberOfSensorData);
  writingFile.write("\n\r");
  
}

void PacketManager::writePacket(char type, time_t timeDate, unsigned short sensorData[]) {  
  char dd[2];
  char mm[2];
  char yy[2];
  char ty[2];
  itoa(day(), dd, 2);
  itoa(month(), mm, 2);
  itoa(year(), yy, 2);
  itoa(type, ty, 2);
  char txtName[12];
  strcpy(txtName,dd);
  strcat(txtName,mm);
  strcat(txtName,yy);
  strcat(txtName,ty);
  strcat(txtName,".txt");
  //check if file exists where to write the packet
  //if file doesn't exist, create it
  if (strcmp(txtName, writingFile.name()) == 0) {
    initPacketFile(type, timeDate, (sizeof(sensorData)/sizeof(sensorData[0])));
  }
  
  char *rawPacket;
  rawPacket = (char*)&createPacket(type, timeDate, sensorData);
  writingFile.write(rawPacket);
  writingFile.write("\n\r");
  
}

Packet PacketManager::createPacket(char type, time_t timeDate, unsigned short sensorData[]) {
  Packet packet;
  packet.type = type;
  packet.timeDate = timeDate;
  
  unsigned short numberOfElements = sizeof(sensorData)/sizeof(sensorData[0]);
  packet.numberOfSensorData = numberOfElements;
  if((packet.sensorData = (unsigned short*)malloc(numberOfElements * sizeof(unsigned short))) == 0)
  {
    //malloc failed!!!!
  }
  
   for (int i=0; i<numberOfElements; i++) {
     packet.sensorData[i] = sensorData[i];
   }

  return packet;
}

//returns false if no files from that day exists
bool PacketManager::openFileFromDay(unsigned short sday, unsigned short smonth, unsigned short syear, unsigned char type) {
  char dd[2]; 
  char mm[2];
  char yy[2];
  char ty[2];
  itoa(day(), dd, 2);
  itoa(month(), mm, 2);
  itoa(year(), yy, 2);
  char txtName[12];
  strcpy(txtName,dd);
  strcat(txtName,mm);
  strcat(txtName,yy);
  strcat(txtName,ty);
  strcat(txtName,".000");
  
  if(!SD.exists(txtName)) {
    return false;
  } else {
    streamingFile = SD.open(txtName, FILE_READ);
    //reset packet stream counters
    sdPosition = 0;
    loopCounter = 0;
    
    streamingBusy = true;
    return true;
  }
}

//returns one packet each time this function is loaded
//before loading each packet, check if there are remaining packets
bool PacketManager::loadNextPacketFromDay(Packet *packet) {
  if(!streamingBusy) {
    return false;
  } else {
    *streamPacket = *packet;
    
    //stream data
    sdPosition = 0;
    loopCounter = 0;
    bool bufferingPacket = true;
    while(streamingFile.available() && bufferingPacket) {
      //parse packet
      bufferingPacket = parsePacket(streamingFile.read());
    }
    
    //close current file and check for other file from the same day if file is parsed completely
    if(!streamingFile.available()) {
      char fileExt[3];
      char txtName[12];
      memcpy(fileExt, &streamingFile.name()[10],3);
      memcpy(txtName, &streamingFile.name()[0],12);
      
      streamingFile.close();
      
      unsigned short fileNr;
      fileNr = atoi(fileExt);
      fileNr += 1;
      itoa(fileNr, fileExt, 3);
      txtName[9] = fileExt[0];
      txtName[10] = fileExt[1];
      txtName[11] = fileExt[2];
      if(!SD.exists(txtName)) {
        streamingBusy = false;
      } else {
        streamingFile = SD.open(txtName, FILE_READ);
      }
    }
    
    return true;
  }
}

//is true when packet is complete
bool PacketManager::parsePacket(char b) {
  switch(sdPosition) {
    case 0:
      tempByte = b;
    case 1:
      numberOfSensorData = (b << 8) | tempByte; //combine first 2 bytes to unsigned short
      packetSize = 5 + (numberOfSensorData << 1);
    case 2:
      break; //skip \n could be error check
    case 3:  
      break; //skip \r could be error check
    default:
      // start parsing packet
      if(loopCounter < packetSize) {
        *rawPacketPiece++ = b;
      } else if(loopCounter >= packetSize) {
        streamPacket = (Packet*)rawPacketPiece;
        loopCounter = 0;
        sdPosition += 2;
        return true;
      }
      loopCounter++;
  }
    sdPosition++;
}


//warn when EEPROM almost full
int PacketManager::saveToEEPROM(Packet* packet) {
 return 0;
}

void PacketManager::loadFromEEPROM(Packet packet[]) {
  return;
}

void PacketManager::clearEEPROM() {
  return;
}
