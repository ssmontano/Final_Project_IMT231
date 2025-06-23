#include "Brainwave.h"
#include "config.h"

// Variables globales que este módulo necesita del archivo principal
extern short velo;
extern SemaphoreHandle_t motorMutex;

// Constructor original
Brainwave::Brainwave()
{
  checksum = 0; 
  generatedChecksum = 0;
  payloadLength = 0;
  for(short i=0;i<64;i++) { payloadData[i]=0; }
  poorQuality = 0;
  attention = 0;
  meditation = 0;
  lastReceivedPacket = 0;
  bigPacket = false;
}

// ReadOneByte original
byte Brainwave::ReadOneByte(Stream &stream) {
  int ByteRead; 
  while(!stream.available());
  ByteRead = stream.read();
  return ByteRead;
}

// generating_res original
void Brainwave::generating_res(Stream &stream, short num)
{
  if(ReadOneByte(stream) == 170) {
    if(ReadOneByte(stream) == 170) {
      payloadLength = ReadOneByte(stream);
      if(payloadLength > 169) return;

      generatedChecksum = 0;
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte(stream);
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte(stream);
      generatedChecksum = 255 - generatedChecksum;

      if(checksum == generatedChecksum) {
        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {
          switch (payloadData[i]) {
            case 2: i++; poorQuality = payloadData[i]; bigPacket = true; break;
            case 4: i++; attention = payloadData[i]; break;
            case 5: i++; meditation = payloadData[i]; break;
            case 0x80: i = i + 3; break;
            case 0x83: i = i + 25; break;
            default: break;
          }
        }

        if(bigPacket) {
          if (xSemaphoreTake(motorMutex, portMAX_DELAY) == pdTRUE) {
            if (attention > 0 && attention <= 100) { 
              velo = (int)(((float)attention/(float)100)*255);
              ledcWrite(ENABLE_PORT, velo * 1.5);
              if (num == 1) {
                digitalWrite(MOTOR_WIRE_1, HIGH);
                digitalWrite(MOTOR_WIRE_2, LOW);
              } else {
                digitalWrite(MOTOR_WIRE_1, LOW);
                digitalWrite(MOTOR_WIRE_2, HIGH);
              }
            } else {
              ledcWrite(ENABLE_PORT, 0); 
            }
            xSemaphoreGive(motorMutex);
          }
        }
        bigPacket = false;
      }
    }
  }
}