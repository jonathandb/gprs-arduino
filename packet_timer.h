/*

A packet file has following naming convention: DDMMYYXX.Z
DDMMYY: date
XX: type
Z: number of same type on same date (in case of reboot and multiple files)


A packet file consists of:
XX\n
timeDate(4 bytes)sensorData(2 * XX bytes) \n

*/

#include <Time.h>
#include "packet_manager.h"

class PacketTimerManager {
  private:
    
  public:
    PacketTimerManager();
    void startTimer(Packet packet);
    void stopTimer(Packet packet);
};
