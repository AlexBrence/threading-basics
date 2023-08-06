#include "packet2.h"

Packet2::Packet2(int id, std::function<void()> func)
  : m_id(id), m_func(func) 
{}


void Packet2::process() 
{
  std::cout << "Processing packet " << m_id << std::endl;

  /* Simulate processing */
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  /* Simulate exception */
  if (m_id % 4 == 0) 
  {
    throw std::runtime_error("Processing error");
  }

  m_func();
}
