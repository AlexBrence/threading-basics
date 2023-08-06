#pragma once 

#include "packetbase.h"

#include <functional>
#include <iostream>
#include <thread>

/* using std::function */
class Packet2 : public PacketBase 
{
private:
  int m_id;
  std::function<void()> m_func;

public:
  Packet2(int id, std::function<void()>func);

  void process() override;
};
