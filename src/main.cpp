/*
  Code develop based on Copyright (c) 2023 João Senírio de Sousa Costa,
  link of his work on: https://github.com/JoaoSenirio

  (c) 2023 Gabriel Tomaz de Lemos Garcia
*/


#include <Arduino.h>
#include <AltSoftSerial.h>

#define NACK 0x02
#define ACK 0X04



AltSoftSerial LegoSerial;

int segundoPasso = 0;

void setup()
{
  Serial.begin(57600);
  LegoSerial.begin(2400);
}

bool is_connected = 0;
unsigned long m, received;
void loop()
{

  // Serial.println(LegoSerial.available());
  if (!is_connected)
  {
    Serial.println("Trying to connect");
    byte l[2];
    while (LegoSerial.available())
    {
      l[0] = l[1];
      l[1] = LegoSerial.read();

      if (l[0] == 0XED && l[1] == ACK)
      {
        LegoSerial.write(ACK);
        Serial.println("Starting data trading!");
        LegoSerial.flush();
        LegoSerial.end();
        LegoSerial.begin(57600);
        is_connected = 1;
        m = millis();
        received = m;
      }
    }
  }

  if (is_connected)
  {
    byte checkbyte, Data_Package[3];
    LegoSerial.readBytes(Data_Package, 3);
    checkbyte = Data_Package[0] ^ Data_Package[1] ^ 0xFF;
    if (checkbyte == Data_Package[2])
    {
      Serial.println(Data_Package[1]);
     //received = millis(); In the future, check if sensor is connected
    }

    /* In the future, check if sensor is connected
    if (millis() - received >= 100)
    {
      Serial.println("disconnected");
      is_connected = 0;
    }
    */
    if (millis() - m >= 100)
    {
      LegoSerial.write(NACK);
      m = millis();
    }
  }
}
