/*

A packet file has following naming convention: DDMMYYXX.Z
DDMMYY: date
XX: type
Z: number of same type on same date (in case of reboot and multiple files)


A packet file consists of:
XX\n
timeDate(4 bytes)sensorData(2 * XX bytes) \n

*/

#include <SD.h>
#include <Time.h>

typedef unsigned char* RawPacket;

typedef struct Packet {
    char type;   //ie houses, meteo. (different time interval) (1byte)
    time_t timeDate;  // (4 byte)
    unsigned short numberOfSensorData;
    unsigned short *sensorData;  //(2 byte * x)
};

class PacketManager {
  private:
    RawPacket rawPacketPiece;
    char tempByte;
    unsigned short numberOfSensorData;
    unsigned short packetSize;
    unsigned int loopCounter;
    unsigned int streamingPacketPosition;
    File streamingFile;
    File writingFile;
    unsigned int sdPosition;
    bool parsePacket(char b);
    Packet *streamPacket;
  public:
    PacketManager();
    void initPacketFile(char type, time_t timeDate, unsigned short numberOfSensorData);
    void writePacket(char type, time_t timeDate, unsigned short sensorData[]);
    Packet createPacket(char type, time_t timeDate, unsigned short sensorData[]);
    bool openFileFromDay(unsigned short sday, unsigned short smonth, unsigned short syear, unsigned char type);
    bool loadNextPacketFromDay(Packet *packet);
    bool streamingBusy;
    int saveToEEPROM(Packet* packet); //warn when EEPROM almost full
    void loadFromEEPROM(Packet packet[]);
    void clearEEPROM();
};
