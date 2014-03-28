#include <Time.h>
#include <SIM900.h>
#include <inetGSM.h>
#include <Arduino.h>
#include "packet_manager.h"

#include "SIM900.h"
#include <SoftwareSerial.h>

#define numberOfSensors 50

class GPRS {
private:
    bool connected;
    //SIMCOM900 gsm;
    InetGSM inet;
    char* _server;
    unsigned short _port;
public:
    GPRS(char* server, unsigned short port);
    void power();
    void turnoff();
    void reset();
    bool initialiseConnection();
    void disconnectGPRS();
    void setServerParameters(char server[50], unsigned short port);
    int sendPacket(Packet packet);
    char* packetJsonCreator(Packet packet);
    time_t getNTPTime();
    bool syncNTPTime();
};
