#pragma once

#include "packetbase.h"

#include <queue>
#include <mutex>
#include <condition_variable> 
#include <iostream>


class PacketProcessor 
{
private:
  std::queue<std::shared_ptr<PacketBase>> m_packetQueue;
  std::mutex              m_mutex;
  std::condition_variable m_condVar;
  bool                    m_isIdle;
  long long int           m_exceptionCount;
  bool                    m_abort;

public:
  PacketProcessor();

  void          addPacket(std::shared_ptr<PacketBase> packet);
  bool          isIdle();
  long long int getExceptionCount();
  void          abort();
  void          processPackets();
};
