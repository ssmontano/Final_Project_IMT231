#ifndef BRAINWAVE_H
#define BRAINWAVE_H

#include <Arduino.h>

class Brainwave {
  byte checksum;
  byte generatedChecksum;
  int payloadLength;
  byte payloadData[64];
  byte poorQuality;
  byte meditation;
  byte attention;
  long lastReceivedPacket = 0;
  bool bigPacket = false;

public:
  Brainwave();
  byte ReadOneByte(Stream &stream);
  void generating_res(Stream &stream, short num);
};

#endif // BRAINWAVE_H