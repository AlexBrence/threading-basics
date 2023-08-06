#pragma once

#include "packetbase.h"

#include <iostream>
#include <thread>


/* using virtual function */
class Packet : public PacketBase 
{
public:
  Packet(int id);

  void process() override;

private:
  int m_id;
};
