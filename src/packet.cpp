#include "packet.h"


Packet::Packet(int id) : m_id(id) {}


void Packet::process() 
{
  std::cout << "Processing packet " << m_id << std::endl;

  /* Simulate processing */
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  /* Simulate exception */
  if (m_id % 4 == 0) 
  {
    throw std::runtime_error("Processing error");
  }
}
